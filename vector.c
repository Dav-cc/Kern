#include "vector.h"
#include <stdio.h>


vector* vector_init(int element_size){
    vector* vec = (vector*) malloc(sizeof(vector));
    vec->element_size = element_size;
    vec->element_number = 0;
    vec->capacity = 1;
    vec->items = (void**) malloc(vec->capacity * sizeof(void*));
    return vec;
}


void vector_push(vector* vec, void* item){
    if(vec->capacity == vec->element_number){
        size_t new_cap = vec->capacity;
        void* new_element = realloc(vec->items, sizeof(void*));
        if(!new_element){
            fprintf(stderr, "error in allocating space for realloc\n");
            return; 
        }
        vec->items = new_element;
        vec->capacity = new_cap;
    }
    vec->items[vec->element_number++] = item;
}


void* vector_get_index(vector* vec, int index){
    if(index > vec->capacity){
        printf("try to access elemenet more than capacity");
        return NULL;
    }
    void* r = vec->items[index];
    return r;
}
