#include "../utils.h" 
#include "../sv_string.h"
#include "../sv_vector.h"


int main(void)
{
    char* c = read_file("4.txt");
    String s = str_init(c);
    Vector lines = str_split(&s, "\n");
    
    long acc = 0;
    long acc2 = 0;
    for (int i=0; i<lines.length; i++)
    {
        String* line = (String*)vec_elem_loc(&lines, i);
        Vector pairs = str_split(line, ",");
        String* first_elf = (String*)vec_elem_loc(&pairs, 0);
        String* second_elf = (String*)vec_elem_loc(&pairs, 1);
        Vector fe_sections = str_split(first_elf, "-");
        Vector se_sections = str_split(second_elf, "-");
        long fe1 = strtol(
            str_to_c_str((String*)vec_elem_loc(&fe_sections, 0)), 
            NULL, 10);
        long fe2 = strtol(
            str_to_c_str((String*)vec_elem_loc(&fe_sections, 1)), 
            NULL, 10);
        long se1 = strtol(
            str_to_c_str((String*)vec_elem_loc(&se_sections, 0)), 
            NULL, 10);
        long se2 = strtol(
            str_to_c_str((String*)vec_elem_loc(&se_sections, 1)), 
            NULL, 10);

        acc += ((se1 >= fe1) && (se2 <= fe2)) || ((se1 <= fe1) && (se2 >= fe2));
        acc2 += !((se2 < fe1) || (fe2 < se1));
    }
    printf("%ld\n", acc);
    printf("%ld\n", acc2);

    return 0;
}
