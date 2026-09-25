#include <time.h>
#include <stdlib.h>
#include <assert.h>


#ifndef COLORS
#define COLORS
#include "colors.h"
#endif // COLORS


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


void byte_swap(char* list, ssize_t ind_1, ssize_t ind_2, size_t size_of) {

    assert(list != NULL);
    assert(size_of >= 1);

    if (ind_1 == ind_2) { return; }

    for (size_t delta = 0; delta < size_of; delta++) {

        char temp = *(list + ind_1 * size_of + delta);
        *(list + ind_1 * size_of + delta) = *(list + ind_2 * size_of + delta);
        *(list + ind_2 * size_of + delta) = temp;

    }

    return;

}


void quick_sort(void* list, ssize_t left, ssize_t right, size_t size_of, int (* comparator)(const void*, const void*)) {

    assert(list != NULL);
    assert(size_of >= 1);
    assert(comparator != NULL);

    if (left >= right) { return; }

    ssize_t ind_compare = left + (rand() % (1 + right - left));
    
    char* p_list = (char*)list;
    byte_swap(p_list, left, ind_compare, size_of);
    ssize_t last = left;
    
    for (ssize_t i = left + 1; i <= right; i++) {

        const void* pt_i = (const void*)(p_list + i * size_of);
        const void* pt_left = (const void*)(p_list + left * size_of);
        
        if ((*comparator)(pt_i, pt_left) < 0) { 
            last++;
            byte_swap(p_list, i, last, size_of); 
        }
        
    }
    
    byte_swap(p_list, left, last, size_of);
    
    quick_sort(list, left, last - 1, size_of, comparator);
    quick_sort(list, last + 1, right, size_of, comparator);

}


void full_qsort(void* list, ssize_t left, ssize_t right, size_t size_of, int (* comparator)(const void*, const void*)) {

    assert(list != NULL);
    assert(size_of >= 1);
    assert(comparator != NULL);

    time_t seed = time(NULL);
    srand(seed);
    quick_sort(list, left, right, size_of, comparator);

}


int str_compare_beg(const void* p1, const void* p2) {

    assert(p1 != NULL && p2 != NULL);

    char* str_1 = *(char**) p1;
    char* str_2 = *(char**) p2;

    return alpha_strcmp(str_1, str_2, FORWARD);

}


int str_compare_end(const void* p1, const void* p2) {

    assert(p1 != NULL && p2 != NULL);

    char* str_1 = *(char**) p1;
    char* str_2 = *(char**) p2;

    return alpha_strcmp(str_1, str_2, BACKWARD);

}


int str_compare_orig(const void* p1, const void* p2) {

    assert(p1 != NULL && p2 != NULL);

    long long index_1 = (long long)*(char**)p1;
    long long index_2 = (long long)*(char**)p2;

    return (index_1 - index_2 > 0) ? 1 : -1;

}


int struct_compare_beg(const void* p1, const void* p2) {

    assert(p1 != NULL && p2 != NULL);

    line_data* data_1 = (line_data*) p1;
    line_data* data_2 = (line_data*) p2;

    return alpha_structcmp(data_1, data_2, FORWARD);

}


int struct_compare_end(const void* p1, const void* p2) {

    assert(p1 != NULL && p2 != NULL);

    line_data* data_1 = (line_data*) p1;
    line_data* data_2 = (line_data*) p2;

    return alpha_structcmp(data_1, data_2, BACKWARD);

}


int struct_compare_orig(const void* p1, const void* p2) {

    assert(p1 != NULL && p2 != NULL);

    line_data* data_1 = (line_data*) p1;
    line_data* data_2 = (line_data*) p2;

    long long index_1 = (long long)(data_1->pointer);
    long long index_2 = (long long)(data_2->pointer);

    return index_1 - index_2;

}
