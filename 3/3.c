#include "../utils.h"
#include "../sv_string.h"
#include "../sv_vector.h"
#include <time.h>


int char_priority(char c)
{
    return c >= 'a' ? c - 'a' + 1 : c - 'A' + 'z' - 'a' + 2;
}


int main(void)
{
    clock_t begin = clock();

    char* contents = read_file("3.txt");
    String s = str_init(contents);
    Vector lines = str_split(&s, "\n");
    Vector lines_items = vec_init(sizeof(Vector));
    for (int i=0; i<lines.length; i++) 
    {
        String rucksack;
        vec_at(&lines, i, &rucksack);
        String view1 = str_slice(&rucksack, 0, rucksack.size / 2);
        String view2 = str_slice(&rucksack, rucksack.size / 2, rucksack.size);
        Vector v = vec_init(sizeof(String));
        vec_append(&v, (void*)&view1);
        vec_append(&v, (void*)&view2);
        vec_append(&lines_items, (void*)&v); 
    }

    // part one
    unsigned long acc = 0;
    for (int i=0; i<lines_items.length; i++) 
    {
        Vector rucksack_items;
        vec_at(&lines_items, i, &rucksack_items);
        String first_item, second_item;
        vec_at(&rucksack_items, 0, &first_item);
        vec_at(&rucksack_items, 1, &second_item);
        for (int j = 0; j<first_item.size; j++) {
            char c[2] = "\0";
            c[0] = first_item.c_str[j];
            if (str_cstr_in(&second_item, c)) {
                acc += char_priority(c[0]);
                break;
            }
        }
    }

    //part two
    unsigned long acc2 = 0;
    for (int i = 0; i<lines.length; i+=3)
    {
        Vector view = vec_slice_view(&lines, i, i+3);
        String s1, s2, s3;
        vec_at(&view, 0, &s1);
        vec_at(&view, 1, &s2);
        vec_at(&view, 2, &s3);
        for (int j = 0; j < s1.size; j++) {
            char c[2] = "\0";
            c[0] = s1.c_str[j];
            if (str_cstr_in(&s2, c) && str_cstr_in(&s3, c)) {
                acc2 += char_priority(c[0]);
                break;
            }
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%lu\n", acc);
    printf("%lu\n", acc2);
    printf("time taken: %f\n", time_spent);
    return 0;
}
