#pragma once 

#include "common\defines.h"

enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGTH
};

KAPI void* _darray_create(u64 length, u64 stride);
KAPI void _darray_destroy(void* array);

KAPI u64 _darray_field_get(void* array, u64 field);
KAPI void _darray_field_set(void* array, u64 field, u64 val);

KAPI void* _darray_resize(void* array);

KAPI void* _darray_push(void* array, const void* val_ptr);
KAPI void _darray_pop(void* array, void* dest);

KAPI void* _darray_pop_at(void* array,u64 idx, void* dest);
KAPI void* _darray_insert_at(void* array, u64 idx, const void* val_ptr);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#define darray_create(type) \
    _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))
    
#define darray_reserve(type, capacity) \
    _darray_create(capacity, sizeof(type))

#define darray_destroy(array) _darray_destroy(array);

#define darray_push(array, val)                     \
    {                                               \
        typeof(val) temp = val;                     \
        array = _darray_push(array, &temp);         \
    }

#define darray_pop(array, val_ptr) \
    _darray_pop(array, val_ptr)

#define darray_insert_at(array, idx, val)             \
    {                                                   \
        typeof(val) temp = val;                         \
        array = _darray_push_at(array, idx, &temp)      \
    }

#define darray_pop_at(array, idx, val_ptr)              \
    _darray_pop_at(array, idx, val_ptr)

#define darray_zero(array) \
    _darray_field_set(array, DARRAY_LENGTH, 0)

#define darray_capacity(array) \
    _darray_field_get(array, DARRAY_CAPACITY)

#define darray_length(array) \
    _darray_field_get(array, DARRAY_LENGTH)

#define darray_stride(array) \
    _darray_field_get(array, DARRAY_STRIDE)

#define darray_length_set(array, val) \
    _darray_field_set(array, DARRAY_LENGTH, val)