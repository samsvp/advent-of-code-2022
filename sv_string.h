#ifndef SV_STRING_H
#define SV_STRING_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include "sv_vector.h"


/**
 * String struct that holds a char array and the string length
 */
typedef struct {
    char* c_str;
    size_t size;
} String;


/**
 * Creates a new String
 * @param c: char array that holds the string content
 * @return a string with c as data
 */
String str_init(char* c) 
{
    String s;
    size_t c_len = strlen(c);
    s.c_str = malloc((c_len + 1) * sizeof(char));
    s.c_str = c;
    s.size = c_len;
    return s;
}


/**
 * Gets the slice of a String. If `end` is smaller than `start` then
 * a string of size 0 (empty string) is returned. The contents of the
 * sliced String are a reference to the original String `s`
 * @param s: the string to take the slice from
 * @param start: the start of the string slice
 * @param end: the end of the string slice
 * @return a String containing the sliced data
 */
String str_slice(String* s, size_t start, size_t end)
{
    size_t length = end > start ? end - start : 0;
    size_t max_length = s->size > start ? s->size - start : 0;
    String sub_s;
    if (length > 0 && max_length > 0) {
        sub_s.c_str = s->c_str + start;
        sub_s.size = length <= max_length ? length : max_length;
    } else {
        sub_s.c_str = "";
        sub_s.size = 0;
    }
    return sub_s;
}


/**
 * Returns a c string representation of the String s.
 * @param s: the string to convert the data from.
 * @return a c string representation of the String s.
 */
char* str_to_c_str(String* s)
{
    char* c = malloc((s->size + 1) * sizeof(char));
    strncpy(c, s->c_str, s->size);
    return c;
}


/**
 * Gets the char at the given location. If `i` is negative 
 * then counting starts from the last element (e.g. string_at(&s, -1)
 * returns the last char of the String). If `i` is bigger than 
 * the string size then '\0' is returned.
 *
 * @param s: the String to take the char from
 * @param i: the location to take the char from
 * @return the char at the given location
 */
char str_at(String* s, int i)
{
    if (i < 0) {
        i = s->size + i;
    }

    if (i >= s->size || i < 0) return '\0';
    return s->c_str[i];
}


/**
 * Checks if two strings are the same by comparing each character.
 * @param s1: the first String
 * @param s2: the second String
 * @return true if the strings are the same, false otherwise
 */
bool str_comp(String* s1, String* s2)
{
    if (s1->size != s2->size) {
        return false;
    }
    for (int i=0; i<s1->size; i++) {
        if (str_at(s1, i) != str_at(s2, i)) {
            return false;
        }
    }
    return true;
}


/**
 * Copies a String
 * @param string: the String to copy
 * @return a copy of the passed String
 */
String* str_copy(String* string)
{
    String* ss = malloc(sizeof(String));
    ss->c_str = str_to_c_str(string);
    ss->size = string->size;
    return ss;
}


/**
 * Splits a String into a vector
 * @param c: the split separator
 * @return a vector of substrings using `c` as delimiter
 */
Vector str_split(String* s, char* c) 
{
    Vector v = vec_init(sizeof(String));
    size_t c_length = strlen(c);
    if (s->size < strlen(c)) { 
        return v;
    }

    String cs = str_init(c);
    size_t begining = 0;
    for (size_t i=0; i < s->size - c_length + 1; i++)
    {
        String string_slice = str_slice(s, i, i + c_length);
        if (str_comp(&string_slice, &cs)) {
            String string = str_slice(s, begining, i);
            vec_append(&v, (void*)&string);
            i+= c_length - 1;
            begining = i + 1;
        }
    }
    if (begining != s->size) {
        String string = str_slice(s, begining, s->size);
        vec_append(&v, (void*)&string);
    }
    return v;
}


#endif
