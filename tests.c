#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "sv_list.h"
#include "sv_string.h"
#include "sv_vector.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define COMP_VALUE(x1, x2) x1 == x2 ? \
    printf("%s[COMP VALUE] SAME VALUES: OK\n", KGRN) : \
    printf("%s[COMP VALUE] DIFFERENT VALUES: FAIL\n", KRED);

#define COMP_CHAR_POINTER(c1, c2) strcmp(c1, c2) == 0 ? \
    printf("%s[COMP CHAR PTR] SAME VALUES: OK\n", KGRN) : \
    printf("%s[COMP CHAR PTR] DIFFENT VALUES: FAIL\n", KRED);


void log_title(const char* title) 
{
    printf("%s%s\n", KCYN, title);
}


void comp_string(String* s1, String* s2) 
{
    if (s1->size != s2->size) {
        printf("%s[COMP STRING] DIFFERENT SIZES\n", KRED);
        goto comp_string_failed;
    }
    for (int i=0; i<s1->size; i++) {
        if (s1->c_str[i] != s2->c_str[i]) {
            printf("%s[COMP STRING] FAILED ON %d, %d\n", KRED, s1->c_str[i], s2->c_str[i]);
            goto comp_string_failed;
            return;
        }
    }
    printf("%s[COMP STRING] SAME VALUES: OK\n", KGRN);
    return;

comp_string_failed:
    printf("%s[COMP STRING] DIFFERENT VALUES: FAILED\n", KRED);
    printf("%s[COMP STRING] STRING 1:%s \t STRING 2:%s\n", 
           KRED, str_to_c_str(s1), str_to_c_str(s2));
    printf("%s[COMP STRING] STRING 1 LENGTH:%lu \t STRING 2 LENGTH:%lu\n", 
           KRED, s1->size, s2->size);
    printf("%s\n", KNRM);
    return;
}


typedef struct {
    int* arr;
} S;


void free_scallback(void* s)
{
    free(((S*)s)->arr);
}

int max_comp_func(const void* a, const void* b) 
{
    return *(int*) a - *(int*) b;
}
int min_comp_func(const void* a, const void* b) 
{
    return *(int*) b - *(int*) a;
}
int main(void)
{

    log_title("String init tests");
    {
        char* words[] = {"Hello", "World", "Hi", "String with spaces", 
            "Oh man, this is great", "I'm a \nstring\n with new\n lines;", 
            "Hello\tWorld"};

        size_t words_len = sizeof(words) / sizeof(words[0]);
        for (int i=0; i<words_len; i++) {
            String s = str_init(words[i]);
            COMP_CHAR_POINTER(s.c_str, words[i]);
            COMP_VALUE(s.size, strlen(words[i]));
        }
    }

    printf("\n\n");
    log_title("Slice Tests");
    {
        String s = str_init("Hello world, how are you today?");
        size_t slice_starts[]    = {1,  5,  8, 10, 15, 0, 6, 15, 7,  8};
        size_t slice_ends[]      = {10, 4, 18, 50, 15, 2, 4, 20, 2,  9};
        String expected_slices[] = {
            str_init("ello worl"), 
            str_init(""),
            str_init("rld, how a"),
            str_init("d, how are you today?"), 
            str_init(""), 
            str_init("He"), 
            str_init(""), 
            str_init("w are"), 
            str_init(""), 
            str_init("r")
        };
        size_t len = sizeof(slice_starts) / sizeof(size_t);
        for (int i=0; i<len; i++) {
            String slice = str_slice(&s, slice_starts[i], slice_ends[i]);
            comp_string(&slice, &expected_slices[i]);
        }
    }
    printf("\n\n");

    log_title("Split Tests");
    {
        String s = str_init("the quick brown fox is a very fast, very animal\n"
                               "I hope to catch the fox one day\n"
                               "one day I will catch it!");
        {
            Vector v = str_split(&s, "ll");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
        printf("\n");
        {
            Vector v = str_split(&s, "l");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
        printf("\n");
        {
            Vector v = str_split(&s, "!");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
        printf("\n");
        {
            Vector v = str_split(&s, "fox");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
        printf("\n");
        {
            Vector v = str_split(&s, "very");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
        printf("\n");
        {
            Vector v = str_split(&s, "t");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
        printf("\n");
        {
            Vector v = str_split(&s, " ");
            for (int i=0; i<v.length; i++) {
                String s;
                vec_at(&v, i, &s);
                printf("item %d: %s\n",i , str_to_c_str(&s));
            }
        }
    }
    printf("\n\n");


    String s = str_init("Hello");
    printf("\nAdd tests\n");
    printf("%s\n", s.c_str);

    String slice = str_slice(&s, 1, 4);
    printf("%s\n", str_to_c_str(&slice));
    List* l;
    list_add(&l, &slice);
    list_add(&l, &s);
    printf("%s\n", str_to_c_str((String*)l->data));

    printf("\nSplit tests\n");
    Vector v = str_split(&s, "ll");
    for (int i=0; i<v.length; i++) {
        String s;
        vec_at(&v, i, &s);
        printf("%s\n", str_to_c_str(&s));
    }

    log_title("String in test");
    {
        String s = str_init("hello, how are you today?\n I'm fine, thanks!");
        String s2 = str_init("hello");
        COMP_VALUE(true, str_in(&s, &s2));
        s2 = str_init("dsfsdg");
        COMP_VALUE(false, str_in(&s, &s2));
        s2 = str_init("\n");
        COMP_VALUE(true, str_in(&s, &s2));
        s2 = str_init("\n I'm");
        COMP_VALUE(true, str_in(&s, &s2));
        s2 = str_init("thanks");
        COMP_VALUE(true, str_in(&s, &s2));
        s2 = str_init("tranks");
        COMP_VALUE(false, str_in(&s, &s2));
        s2 = str_init("thanks!!");
        COMP_VALUE(false, str_in(&s, &s2));
        s2 = str_init("hello ");
        COMP_VALUE(false, str_in(&s, &s2));
        s2 = str_init(", ");
        COMP_VALUE(true, str_in(&s, &s2));
    }

    log_title("Vector tests");
    {
        log_title("Append tests");
        Vector v = vec_init(sizeof(int));

        int N = 100;
        int ii[N];
        srand(0);
        for (size_t i=0; i<N; i++) {
            ii[i] = rand();
            vec_append(&v, (void*)&ii[i]);
        }
        printf("completed\n");
        for (size_t i=0; i<N; i++) {
            int vi = 0;
            vec_at(&v, i, &vi);
            COMP_VALUE(ii[i], vi);
        }

        log_title("Set at tests");
        int n1 = -10;
        int n2 = -152;
        int n3 = -200;
        vec_set_at(&v, N/2, &n1, NULL);
        vec_set_at(&v, 0, &n2, NULL);
        vec_set_at(&v, -1, &n3, NULL);
        int vn1;
        int vn2;
        int vn3;
        vec_at(&v, N/2, &vn1);
        vec_at(&v, 0, &vn2);
        vec_at(&v, -1, &vn3);
        COMP_VALUE(n1, vn1);
        COMP_VALUE(n2, vn2);
        COMP_VALUE(n3, vn3);

        log_title("Delete tests");
        int vnhalf;
        vec_delete_at(&v, N / 2, NULL);
        vec_at(&v, N/2, &vnhalf);
        COMP_VALUE(vnhalf, ii[N / 2+1]);
        
        vec_delete_at(&v, -1, NULL);
        int vnm1;
        vec_at(&v, -1, &vnm1);
        COMP_VALUE(vnm1, ii[N-2]);

        vec_delete_at(&v, 0, NULL);
        vec_at(&v, 0, &vnhalf);
        COMP_VALUE(vnhalf, ii[1]);
        vec_at(&v, N/2, &vnhalf);
        COMP_VALUE(vnhalf, ii[N / 2+2]);
       
        // check if delete fails when we are out of bounds
        COMP_VALUE(vec_delete_at(&v, N, NULL), false);
        COMP_VALUE(vec_delete_at(&v, -N, NULL), false);
        // compare length. we removed 3 items
        COMP_VALUE(v.length, N-3);

        vec_free(&v, NULL);

        log_title("Free tests");
        /** Do some test with structs and memory allocs*/
        S* ss = malloc(N*sizeof(S));
        Vector vs = vec_init(sizeof(S));
        for (int i=0; i<N; i++)
        {
            ss[i].arr = malloc(N*sizeof(int));
            vec_append(&vs, (void*)&ss[i]);
        }
        free(ss);
        vec_free(&vs, free_scallback);
        vec_delete_at(&vs, N / 2, free_scallback);
        vec_delete_at(&vs, -1, free_scallback);
        printf("%s[FREE CALLBACK] OK\n", KGRN);
    }

    {
        log_title("max test");
        Vector vi = vec_init(sizeof(int));
        int max_n = 0;
        int min_n = -1;
        for (int i = 0; i<10; i++) {
            int r = rand();
            if (r > max_n) {
                max_n = r;
            }
            if (r < min_n || min_n == -1) {
                min_n = r;
            }
            vec_append(&vi, (void*)&r);
        }
        int my_max = *(int*)vec_max(&vi, max_comp_func);
        COMP_VALUE(max_n, my_max);
        int my_min = *(int*)vec_min(&vi, min_comp_func);
        COMP_VALUE(min_n, my_min);
    }

    {
        log_title("Vector of char test");
        char* words[] = {"Hello", "World", "Hi", "String with spaces", 
            "Oh man, this is great", "I'm a \nstring\n with new\n lines;", 
            "Hello\tWorld"};
        Vector v = vec_init(sizeof(char*));
        for (size_t i=0; i<sizeof(words)/sizeof(words[0]); i++) {
            vec_append(&v, (void*)&words[i]);
        }
        for (size_t i=0; i<sizeof(words)/sizeof(words[0]); i++) {
            char* w;
            vec_at(&v, i, &w);
            COMP_CHAR_POINTER(words[i], w);
        }
    }
    return 0;
}
