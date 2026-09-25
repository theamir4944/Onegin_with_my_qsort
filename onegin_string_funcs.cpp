#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>


#ifndef COLORS
#define COLORS
#include "colors.h"
#endif // COLORS


#define MEM_BASE 10 // For getline()
#define sep_line "\n-----------------------------------------------------------------------------------------------------------------\n\n"


const int FORWARD = 1;
const int BACKWARD = -1;


char BEGIN_SORT[] = "Sorting by the beginning of the strings\n\n";
char END_SORT[] = "Sorting by the ending of the strings\n\n";
char ORIG_SORT[] = "Orig Onegin\n\n";
char SEC_ORIG_SORT[] = "Another Orig Onegin\n\n"; // Second orig sort


typedef unsigned char uchar;


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
    line_data* string_pointers; //
    char* zero_buffer; //
    char* buffer; //
    char* p_buffer; //
    FILE* file_read_from;
    size_t num_char_in_file; //
    size_t lines_in_text; //
    size_t addr_index; //
};
#endif // FILEINFO


char* my_strcpy(char* str_copy_from, char* str_copy_to) {

    assert(str_copy_from != NULL && str_copy_to != NULL);

    char* copy_t = str_copy_to;

    while (*str_copy_from != '\0') {

        *(str_copy_to++) = *(str_copy_from++);

    }

    return copy_t;

}


ssize_t my_getline(char** lineptr, size_t* bufsize) {

    assert(lineptr != NULL);
    assert(bufsize != NULL);

    *bufsize = (bufsize == NULL || *bufsize == 0) ? MEM_BASE : *bufsize;
    char* p = (lineptr == NULL || *lineptr == NULL) ? (char*)calloc(*bufsize, sizeof(char)) : *lineptr;
    char* p_copy = p;

    assert(p != NULL);

    int c = 0;
    ssize_t wrote_syms = 0;

    while ((c = getchar()) != EOF && c != '\n') {

        if (*p != '\0') {
            *bufsize *= 2;
            p = (char*) realloc (p, *bufsize); // td: realloc and recalloc (realloc + memset) - DONE

            if (p != NULL) {
                p_copy = p;
            }
            else {
                printf(RED "ERROR:\n" WHT);
                printf("File: onegin_string_funcs.cpp ; Function: ssize_t my_getline() ; Line: 95\n");
                printf("%s", strerror(EADDRNOTAVAIL));
                return -1;
            }

            p += wrote_syms;
        }

        *(p++) = c;
        wrote_syms++;

        if (*p == '\n') { break; }

    } 

    *lineptr = p_copy;
    return wrote_syms;

}


int alpha_strcmp(char* str_1, char* str_2, int direction) {

    assert(str_1 != NULL && str_2 != NULL);

    if (direction < 0) {
        while (*(str_1 + 1) != '\0') { str_1++; }
        while (*(str_2 + 1) != '\0') { str_2++; }
    }

    while (*str_1 != '\0' && *str_2 != '\0') {

        while (*str_1 != '\0' && ! isalpha((uchar)(*str_1))) { str_1 += direction; }
        while (*str_2 != '\0' && ! isalpha((uchar)(*str_2))) { str_2 += direction; }

        if (*str_1 == '\0' || *str_2 == '\0') { break; }

        if (tolower((uchar)(*str_1)) == tolower((uchar)(*str_2))) { 
            str_1 += direction; str_2 += direction; continue; 
        }

        if (tolower((uchar)(*str_1)) != tolower((uchar)(*str_2))) {
            return tolower((uchar)(*str_1)) - tolower((uchar)(*str_2)); 
        } 
    }

    while (*str_1 != '\0' && ! isalpha((uchar)(*str_1))) { str_1 += direction; }
    while (*str_2 != '\0' && ! isalpha((uchar)(*str_2))) { str_2 += direction; }

    if (*str_1 == '\0') { return (*str_2 == 0) ? 0 : -1; }
    if (*str_2 == '\0') { return 1; }

    return 0;

}


void my_fputss(char** strings_adress, size_t lines_number, FILE* stream) {

    assert(strings_adress != NULL);
    assert(stream != NULL);

    for (int dur_str = 0; dur_str < lines_number; dur_str++) {
        fputs(strings_adress[dur_str], stream);
        fputc('\n', stream);
    }

}


size_t count_lines(char* text) {

    assert(text != NULL);

    size_t result = 0;
    while (*text != '\0') {
        if (*text == '\n') { result++; }
        text++;
    }
    return result;

}


void output_text(FILE* file_write, char* cliche_text, char** string_address, size_t lines_num) {

    assert(file_write != NULL);
    assert(string_address != NULL);

    fputs(sep_line, file_write);

    if (cliche_text != NULL) {
        fputs(cliche_text, file_write);
    }

    my_fputss(string_address, lines_num, file_write);

}


char* get_file_name(void) {

    char* file_name = (char*) calloc (20, sizeof(char));
    size_t bufsize_for_getline = sizeof(file_name) / sizeof(char);
    my_getline(&file_name, &bufsize_for_getline);

    return file_name;

}


int alpha_structcmp(line_data* data_1, line_data* data_2, int direction) {

    assert(data_1 != NULL && data_2 != NULL);

    char* str_1 = (direction < 0) ? (data_1->pointer + data_1->line_len - 1) : (data_1->pointer); // td: duration -> direction - DONE
    char* str_2 = (direction < 0) ? (data_2->pointer + data_2->line_len - 1) : (data_2->pointer);

    while (*str_1 != '\0' && *str_2 != '\0') {

        while (*str_1 != '\0' && ! isalpha((uchar)(*str_1))) { str_1 += direction; }
        while (*str_2 != '\0' && ! isalpha((uchar)(*str_2))) { str_2 += direction; }

        if (*str_1 == '\0' || *str_2 == '\0') { break; }

        if (tolower((uchar)(*str_1)) == tolower((uchar)(*str_2))) {
            str_1 += direction; str_2 += direction; continue;
        }

        if (tolower((uchar)(*str_1)) != tolower((uchar)(*str_2))) {
            return tolower((uchar)(*str_1)) - tolower((uchar)(*str_2));
        } 
    }

    while (*str_1 != '\0' && ! isalpha((uchar)(*str_1))) { str_1 += direction; }
    while (*str_2 != '\0' && ! isalpha((uchar)(*str_2))) { str_2 += direction; }

    if (*str_1 == '\0') { return (*str_2 == 0) ? 0 : -1; }
    if (*str_2 == '\0') { return 1; }

    return 0;

}


void my__fputss(char* string, size_t lines_number, FILE* stream) {

    assert(string != NULL);
    assert(stream != NULL);

    for (int dur_str = 0; dur_str < lines_number; dur_str++) {
        fputs(string, stream);
        fputc('\n', stream);
    }

}


void struct_output_text(FILE* file_write, char* cliche_text, line_data* data, size_t lines_num) {

    assert(file_write != NULL);
    assert(data != NULL);

    fputs(sep_line, file_write);

    if (cliche_text != NULL) {
        fputs(cliche_text, file_write);
    }

    line_data* linedata = data;

    for (size_t dur_str = 0; dur_str < lines_num; dur_str++) {

        fputs(linedata[dur_str].pointer, file_write);
        fputc('\n', file_write);

    }

}


void buffer_output(FILE* file_write, char* cliche_text, char* buffer) {

    assert(file_write != NULL);
    assert(buffer != NULL);

    fputs(sep_line, file_write);

    if (cliche_text != NULL) {
        fputs(cliche_text, file_write);
    }

    while (*buffer != '\0' || *(buffer + 1) != '\0') {

        if (*buffer == '\0') {
            fputc('\n', file_write);
            buffer++;
            continue;
        }
        else {
            fputc(*(buffer++), file_write);
        }

    }

}
