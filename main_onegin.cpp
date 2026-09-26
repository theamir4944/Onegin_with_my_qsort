#include "TXLib.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


#ifndef COLORS
#define COLORS
#include "colors.h"
#endif // COLORS


#include "onegin_sorting.cpp"


#ifndef ONEG_STR
#include "onegin_string_funcs.cpp"
#define ONEG_STR
#endif // ONEG_STR


#ifndef LINEDATA
#define LINEDATA
struct line_data {
    char* pointer;
    size_t line_len;
};
#endif // LINEDATA


#ifndef FILEINFO
#define FILEINFO
struct file_info {
    line_data* string_pointers; // free at the end
    char* zero_buffer; // free at the end
    char* buffer; 
    char* p_buffer; 
    FILE* file_read_from;
    size_t num_char_in_file; 
    size_t lines_in_text; 
    size_t addr_index; 
};
#endif // FILEINFO


size_t open_and_stat(char* file_read_name, file_info* data);
int text_to_buffer(char* file_read_name, file_info* data);
void forming_pointers(file_info* data);


int main(int argc, char* argv[]) { // TODO - DONE: argc + argv

    // Reading from file

    if (argc < 2) {
        printf("Write the name of file for reading the text (with .txt):\n");
    }

    char* file_read_name = (argc < 2) ? get_file_name() : argv[1];

    struct file_info data = { };

    assert(text_to_buffer(file_read_name, &data) == 0);
    forming_pointers(&data);

    fclose(data.file_read_from); // file was opened in function "text_to_buffer"

    // Writing to file

    if (argc < 3) {
        printf("Write the name of file for writing the sorted text (with .txt):\n");
    }

    char* file_write_name = (argc < 3) ? get_file_name() : argv[2];
    FILE* file_write = fopen(file_write_name, "w");

    qsort(data.string_pointers, data.lines_in_text, sizeof(line_data), &struct_compare_beg);
    struct_output_text(file_write, BEGIN_SORT, data.string_pointers, data.lines_in_text);

    full_qsort(data.string_pointers, 0, data.lines_in_text - 1, sizeof(line_data), &struct_compare_end);
    struct_output_text(file_write, END_SORT, data.string_pointers, data.lines_in_text);

    full_qsort(data.string_pointers, 0, data.lines_in_text - 1, sizeof(line_data), &struct_compare_orig);
    struct_output_text(file_write, ORIG_SORT, data.string_pointers, data.lines_in_text); // TODO - DONE: just output buffer

    buffer_output(file_write, SEC_ORIG_SORT, data.zero_buffer + 1);

    fclose(file_write);
    free(data.zero_buffer);
    free(data.string_pointers);

    return 0;

}


size_t open_and_stat(char* file_read_name, file_info* data) { // TODO - DONE: func for stat
    // TODO: check documentation for stat (not size_t/ssize_t --> long)
    struct stat txt_inf = { };
    data->file_read_from = fopen(file_read_name, "r"); // TODO: return long int + return EOF if file is closed

    if (data->file_read_from == NULL) {
        return EOF;
    }

    stat(file_read_name, &txt_inf);

    return txt_inf.st_size / sizeof(char);

}


int text_to_buffer(char* file_read_name, file_info* data) {

    assert(file_read_name != NULL);
    assert(data != NULL);

    data->num_char_in_file = open_and_stat(file_read_name, data);
    assert(data->num_char_in_file != EOF);

    data->zero_buffer = (char*) calloc (data->num_char_in_file + 2, sizeof(char));
    data->buffer = (data->zero_buffer) + 1;

    data->num_char_in_file = fread((void*)(data->buffer), sizeof(char), data->num_char_in_file, data->file_read_from); // TODO - DONE: fread -> EOF (assert) - DONE
    assert(data->num_char_in_file != EOF); // TODO: assert --> if (Verify) - for always checking - look at txlib

    data->p_buffer = (char*) realloc ((void*)(data->zero_buffer), sizeof(char) * (data->num_char_in_file + 1)) + 1;

    if (data->p_buffer == NULL) {
        printf(RED "ERROR:\n" WHT);
        printf("File: main_onegin.cpp ; Function: int main() ; Line: 122\n"); // TODO - DONE: __FILE__, __LINE__, __FUNCTION__ + Verify (look at prev TODO)
        printf("%s", strerror(EADDRNOTAVAIL));
        return EADDRNOTAVAIL;
    }
    else {
        data->buffer = data->p_buffer;
        data->p_buffer = NULL;
    }

    return 0;

}


void forming_pointers(file_info* data) {

    assert(data != NULL);

    data->lines_in_text = count_lines(data->buffer);

    data->string_pointers = (line_data*) calloc (data->lines_in_text, sizeof(line_data));

    data->addr_index = 0;
    (data->string_pointers)[(data->addr_index)++].pointer = (data->buffer)++;

    for (size_t index_in_str = 0; index_in_str < data->num_char_in_file; index_in_str++) {

        if (*(data->buffer) == '\n' && *(data->buffer + 1) != '\0') {
            (data->string_pointers)[data->addr_index - 1].line_len = (size_t)(data->buffer - (data->string_pointers)[data->addr_index - 1].pointer);
            *((data->buffer)++) = '\0'; 
            (data->string_pointers)[(data->addr_index)++].pointer = (data->buffer);
            // printf("Addr_index: %zu\n", data->addr_index); // Don't forget about Gods of debug ;)
            // printf("Buffer: %p\n\n", data->buffer);
        }

        else if (*(data->buffer) == '\n') {
            (data->string_pointers)[data->addr_index - 1].line_len = (size_t)(data->buffer - (data->string_pointers)[data->addr_index - 1].pointer);
            *((data->buffer)++) = '\0';
        }

        else { 
            (data->buffer)++;
        }

    }

}
// TODO - DONE: readme
// TODO: Sir Hoar (not just Hoar)
