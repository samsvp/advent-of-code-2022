#ifndef SV_VECTOR_H
#define SV_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stdio.h>

#define VECTOR_INITIAL_LENGTH 8


typedef struct {
    void* data;
    int length;
    int capacity;
    size_t element_size;
} Vector;


/**
 * Creates a new vector container.
 * 
 * @param element_size: the size of the data type the vector will contain 
 *      (e.g. if its a vector of ints then element_size should be sizeof(int))
 * @return the created Vector
 */
Vector vec_init(size_t element_size)
{
    Vector v = {
        .length = 0, 
        .element_size = element_size, 
        .data = malloc(VECTOR_INITIAL_LENGTH * element_size),
        .capacity=VECTOR_INITIAL_LENGTH
    };
    return v;
}


/**
 * Returns a pointer to the ith element of the Vector v.
 * If `i` is negative, then start counting backwards
 * from the last element
 * 
 * @param v: a pointer to the Vector
 * @param i: the position to get the location of
 * @return a pointer to the ith element
 */
void* vec_elem_loc(Vector* v, int i)
{
    if (i < 0) {
        i += v->length;
    }

    if (i >= v->length || i < 0) {
        return NULL;
    }

    return (char*)v->data + (i * v->element_size);
}


bool __vector_realloc(Vector* v, size_t new_capacity)
{
    void* data = realloc(v->data, new_capacity * v->element_size);
    if (data) {
        v->data = data;
        v->capacity = new_capacity;
    } else {
        return false;
    }

    return true;
}


bool __vector_expand(Vector* v)
{
    size_t new_capacity = v->capacity == 0 ? 8 : v->capacity * 2;
    return __vector_realloc(v, new_capacity);
}


/**
 * Appends an element to the end of the given Vector using memcpy.
 * Vectors with negative capacity (e.g. vectors which are other 
 * vectors views) can not receive new elements.
 *
 * @param v: the Vector to append the data to 
 * @param element: a pointer to the element to add
 * @return true if successful, false otherwise
 */
bool vec_append(Vector* v, void* element)
{
    if (v->capacity < 0) {
        return 0;
    }

    if (v->length >= v->capacity) {
        if (!__vector_expand(v)) {
            return false;
        }
    }
    // new element location
    void* loc = (char*)v->data + (v->length*v->element_size);
    memcpy(loc, element, v->element_size);
    v->length++;
    return true;
}


/**
 * Gets the element at the ith location. If i is negative,
 * then start counting backwards from the last element.
 * 
 * @param v: a pointer to the Vector
 * @param i: the position to get the location of
 * @param element: pointer to the variable that will hold the ith element
 * @return true if successful, false otherwise
 * */
bool vec_at(const Vector* v, int i, void* element)
{
    if (i < 0) {
        i += v->length;
    }

    if (i >= v->length || i < 0) {
        return false;
    }

    void* source = (char*)v->data + (i * v->element_size);
    memcpy(element, source, v->element_size);
    return true;
}


/**
 * Sets the element at the ith location with the given element. 
 * If `i` is negative, then start counting backwards from the last 
 * element. If `callback` is not null, then call the callback function 
 * with a pointer to the ith element to it before replacing its contents.
 * Use it to free any resources the old ith element may hold.
 *
 * @param v: a pointer to the Vector
 * @param i: the position to get the location of
 * @param element: pointer to the data that will become the ith element
 * @param callback: callback function to free the old ith element resources
 * @return true if successful, false otherwise
 * */
bool vec_set_at(Vector* v, int i, void* element, void(*callback)(void*))
{
    if (i < 0) {
        i += v->length;
    }

    if (i >= v->length || i < 0) {
        return false;
    }

    void* source = vec_elem_loc(v, i);
    if (callback) {
        callback(source);
    }
    memcpy(source, element, v->element_size);
    return true;
}


/**
 * Deletes the element at the ith location. 
 * If `i` is negative, then start counting backwards from the last 
 * element. If `callback` is not null, then call the callback function 
 * with a pointer to the ith element to it before removing it from the 
 * Vector. Use it to free any resources the old ith element may hold.
 *
 * @param v: a pointer to the Vector
 * @param i: the position to delete the data from
 * @param callback: callback function to free the ith element resources
 * @return true if successful, false otherwise
 * */
bool vec_delete_at(Vector* v, int index, void(*callback)(void*))
{
    if (index < 0) {
        index += v->length;
    }

    if (index >= v->length || index < 0) {
        return false;
    }

    void* source = vec_elem_loc(v, index);
    if (callback) {
        callback(source); 
    }
    if (index != v->length - 1) {
        void* dest = vec_elem_loc(v, index + 1);
        size_t count = vec_elem_loc(v, -1) - dest;
        memmove(source, dest, count);
    }

    v->length--;
    if ((v->length > 0) && (v->length  == v->capacity / 4)) {
        bool result = __vector_realloc(v, v->capacity / 2);
        if (!result) 
            return result;
    }
    return true;
}


/**
 * Returns a slice view of the given vector. If end is bigger than
 * the vector then return a slice up until the last element of the
 * vector. If start is bigger than end or start is bigger than
 * v->length then a empty vector is returned
 */
Vector vec_slice_view(Vector* v, int start, int end)
{
    Vector v_view = vec_init(v->element_size);
    if (start >= end || start >= v->length) {
        return v_view;
    }

    v_view.data = vec_elem_loc(v, start);
    v_view.length = v->length > end ? end - start : v->length - start;
    v_view.capacity = -1;
    return v_view;
}


/**
 * Wrapper around qsort.
 * @param v the vector to take the maximum from
 * @param comp_function comparison function
 * @return a void pointer to the maximum element
 */
void vec_sort(Vector* v, int (*compar)(const void *, const void*))
{
    qsort(v->data, v->length, v->element_size, compar);
}


/**
 * Returns the maximum element over the vector given a comparison function.
 * The comparison function must take two parameters, a and b 
 * and returns a positive integer if a > b and a negative integer
 * if a < b, such as in qsort. E.g
 * int comp_function(void* a, void* b) {
 *      return *(int*)a - *(int*) b;
 * }
 * @param v the vector to take the maximum from
 * @param comp_function comparison function
 * @return a void pointer to the maximum element
 */
void* vec_max(Vector* v, int(*comp_function)(const void*, const void*))
{
    if (v->length == 0) {
        return NULL;
    }

    if (v->length == 1) {
        return vec_elem_loc(v, 0);
    }

    void* max_elem = vec_elem_loc(v, 0);
    for (int i=1; i<v->length; i++) {
        void* curr_elem = vec_elem_loc(v, i);
        if (comp_function(vec_elem_loc(v, i), max_elem) > 0) {
            max_elem = curr_elem;
        }
    }
    return max_elem;
}


/**
 * Returns the maximum element over the vector given a comparison function.
 * The comparison function must take two parameters, a and b 
 * and returns a positive integer if a < b and a negative integer
 * if a > b, such as in qsort. E.g
 * int comp_function(void* a, void* b) {
 *      return *(int*)b - *(int*) a;
 * }
 * @param v the vector to take the maximum from
 * @param comp_function comparison function
 * @return a void pointer to the maximum element
 */
void* vec_min(Vector* v, int(*comp_function)(const void*, const void*))
{
    return vec_max(v, comp_function); // lol
}


/**
 * Frees the given vector. If `callback` is not null, 
 * then call the callback function with a pointer 
 * to every element of the Vector before freeing it. 
 * Use it to free any resources that the elements may hold.
 *
 * @param v: a pointer to the Vector
 * @param callback: callback function to free the old ith element resources
 * */
void vec_free(Vector* v, void(*callback)(void*))
{
    if (callback) {
        callback(v->data);
    }
    free(v->data);
    v->length = 0;
    v->capacity = 0;
}

#endif 
