#include <stdlib.h>

#include "../utils.h"

#include "../sv_string.h"
#include "../sv_vector.h"


int compare (const void * a, const void * b)
{
    return *(int*)b - *(int*)a;
}


int main(void)
{
    char* c = read_file("1.txt");
    String s = str_init(c);
    Vector vs = str_split(&s, "\n");

    Vector vi = vec_init(sizeof(long));
    long acc = 0;
    for (int i=0; i<vs.length; i++) {
        String* s = (String*)vec_elem_loc(&vs, i);
        if (s->size == 0) {
            vec_append(&vi, (void*)&acc);
            acc = 0;
            continue;
        }
        acc += strtol(str_to_c_str(s), NULL, 10);
    }

    printf("\n");
    vec_sort(&vi, compare);
    Vector vview = vec_slice_view(&vi, 0, 3);
    printf("hi\n");
    printf("%p, %p\n", vview.data, vi.data);
    acc = 0;
    for (int i=0; i<vview.length; i++) {
        long elem = *(long*)vec_elem_loc(&vview, i);
        printf("elem %d: %ld\n", i, elem);
        acc += elem;
    }
    printf("first three sum: %ld\n", acc);

    return 0;
}
