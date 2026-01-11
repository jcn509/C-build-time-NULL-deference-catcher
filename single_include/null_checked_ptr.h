#ifndef NULL_CHECKED_PTR_H
#define NULL_CHECKED_PTR_H

#ifndef NC_PTR_SAFE_FILE_NAME
#error "You must define NC_PTR_SAFE_FILE_NAME before including null_checked_ptr.h"
#endif

/* Private stuff that does not form part of the API: */

#define _PRIVATE_NC_NAME(nc_ptr_name) _PRIVATE_NC_PTR_DO_NOT_TOUCH_ ## nc_ptr_name

#define _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER3(var, file_name, line) null_dereference_of_ ## var ## _possible_in_file_ ## file_name ## _on_line_ ## line
#define _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER2(var, file_name, line) _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER3(var, file_name, line)
#define _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER(var) _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER2(var, NC_PTR_SAFE_FILE_NAME, __LINE__)

#ifdef NC_PTR_DISABLE_NULL_CHECKS

#define _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name)

#else

#define _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name) \
    { \
        void _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER(nc_ptr_name) (); \
        if (_PRIVATE_NC_NAME(nc_ptr_name) == NULL) { \
            _PRIVATE_NULL_DEREFERENCE_POSSIBLE_MARKER(nc_ptr_name) (); \
        } \
    }

#endif

/* Public facing API: */

#define NC_PTR(type, nc_ptr_name) type _PRIVATE_NC_NAME(nc_ptr_name)

#define NC_PTR_IS_NULL(nc_ptr_name) \
    (_PRIVATE_NC_NAME(nc_ptr_name) == NULL)

#define NC_PTR_ASSIGN(nc_ptr_name, value) \
    _PRIVATE_NC_NAME(nc_ptr_name) = value

#define NC_PTR_DEREFERENCE_READ_OFFSET(destination, nc_ptr_name, offset) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        destination = *(_PRIVATE_NC_NAME(nc_ptr_name) + offset); \
    }

#define NC_PTR_DEREFERENCE_WRITE_OFFSET(nc_ptr_name, value, offset) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        *(_PRIVATE_NC_NAME(nc_ptr_name) + offset) = value; \
    }

#define NC_PTR_DEREFERENCE_READ(destination, nc_ptr_name) \
    NC_PTR_DEREFERENCE_READ_OFFSET(destination, nc_ptr_name, 0)

#define NC_PTR_DEREFERENCE_WRITE(nc_ptr_name, value) \
    NC_PTR_DEREFERENCE_WRITE_OFFSET(nc_ptr_name, value, 0)

#define UNSAFE_NC_PTR_CONVERT_TO_RAW_PTR(destination, nc_ptr_name) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        destination = _PRIVATE_NC_NAME(nc_ptr_name); \
    }

#define UNSAFE_NC_PTR_CONVERT_TO_RAW_PTR_NO_NULL_CHECK(nc_ptr_name) (&(*_PRIVATE_NC_NAME(nc_ptr_name)))

#define NC_PTR_PRE_INCREMENT(nc_ptr_name) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        ++_PRIVATE_NC_NAME(nc_ptr_name); \
    }

#define NC_PTR_PRE_DECREMENT(nc_ptr_name) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        --_PRIVATE_NC_NAME(nc_ptr_name); \
    }

#define NC_PTR_POST_INCREMENT(nc_ptr_name) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name)++; \
    }

#define NC_PTR_POST_DECREMENT(nc_ptr_name) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name)--; \
    }

#define NC_PTR_ADDITION_ASSIGNMENT(nc_ptr_name, value_to_add) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name) += value_to_add; \
    }

#define NC_PTR_SUBTRACTION_ASSIGNMENT(nc_ptr_name, value_to_subtract) \
    { \
        _PRIVATE_NC_PTR_ERROR_IF_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name) -= value_to_subtract; \
    }

#define NC_PTR_STRUCT_FIELD_COPY(nc_ptr_name, struct_var, nc_ptr_field_name) { \
    NC_PTR_ASSIGN(nc_ptr_name, (struct_var)._PRIVATE_NC_NAME(nc_ptr_field_name)); \
}

#define NC_PTR_STRUCT_FIELD_ASSIGN(struct_var, nc_ptr_field_name, value) { \
    (struct_var)._PRIVATE_NC_NAME(nc_ptr_field_name) = value; \
}

#endif /* NULL_CHECKED_PTR_H */
