#ifndef NULL_CHECKED_PTR_H
#define NULL_CHECKED_PTR_H

#ifndef NC_PTR_SAFE_FILE_NAME
#error "You must define NC_PTR_SAFE_FILE_NAME before including null_checked_ptr.h"
#endif

/* Private stuff that does not form part of the API: */

#define _PRIVATE_NC_NAME(nc_ptr_name) _private_null_checked_ptr_##nc_ptr_name

#define _NULL_DEREFERENCE_POSSIBLE_MARKER3(var, file_name, line) null_dereference_of_ ## var ## _possible_in_file_ ## file_name ## _on_line_ ## line
#define _NULL_DEREFERENCE_POSSIBLE_MARKER2(var, file_name, line) _NULL_DEREFERENCE_POSSIBLE_MARKER3(var, file_name, line)
#define _NULL_DEREFERENCE_POSSIBLE_MARKER(var) _NULL_DEREFERENCE_POSSIBLE_MARKER2(var, NC_PTR_SAFE_FILE_NAME, __LINE__)

#ifdef DISABLE_NC_CHECKS

#define _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name)
#else

#define _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name) \
    { \
        void _NULL_DEREFERENCE_POSSIBLE_MARKER(nc_ptr_name) (); \
        if (_PRIVATE_NC_NAME(nc_ptr_name) == NULL) { \
            _NULL_DEREFERENCE_POSSIBLE_MARKER(nc_ptr_name) (); \
        } \
    }
#endif

/* Public facing API: */

#define NULL_CHECKED_PTR(type, nc_ptr_name) type _PRIVATE_NC_NAME(nc_ptr_name)

#define IS_NC_PTR_NULL(nc_ptr_name) \
    (_PRIVATE_NC_NAME(nc_ptr_name) == NULL)

#define ASSIGN_NC_PTR(nc_ptr_name, value) \
    _PRIVATE_NC_NAME(nc_ptr_name) = value

#define DEREFERENCE_NC_PTR_READ_OFFSET(destination, nc_ptr_name, offset) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        destination = *(_PRIVATE_NC_NAME(nc_ptr_name) + offset); \
    }

#define DEREFERENCE_NC_PTR_WRITE_OFFSET(nc_ptr_name, value, offset) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        *(_PRIVATE_NC_NAME(nc_ptr_name) + offset) = value; \
    }

#define DEREFERENCE_NC_PTR_READ(destination, nc_ptr_name) \
    DEREFERENCE_NC_PTR_READ_OFFSET(destination, nc_ptr_name, 0)

#define DEREFERENCE_NC_PTR_WRITE(nc_ptr_name, value) \
    DEREFERENCE_NC_PTR_WRITE_OFFSET(nc_ptr_name, value, 0)

#define UNSAFE_CONVERT_NC_PTR_TO_RAW_PTR(destination, nc_ptr_name) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        destination = _PRIVATE_NC_NAME(nc_ptr_name); \
    }

#define UNSAFE_CONVERT_NC_PTR_TO_RAW_PTR_NO_NULL_CHECK(nc_ptr_name) (&(*_PRIVATE_NC_NAME(nc_ptr_name)))

#define PRE_INCREMENT_NC_PTR(nc_ptr_name) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        ++_PRIVATE_NC_NAME(nc_ptr_name); \
    }

#define PRE_DECREMENT_NC_PTR(nc_ptr_name) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        --_PRIVATE_NC_NAME(nc_ptr_name); \
    }

#define POST_INCREMENT_NC_PTR(nc_ptr_name) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name)++; \
    }

#define POST_DECREMENT_NC_PTR(nc_ptr_name) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name)--; \
    }

#define INCREASE_NC_PTR(nc_ptr_name, value_to_add) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name) += value_to_add; \
    }

#define DECREASE_NC_PTR(nc_ptr_name, value_to_subtract) \
    { \
        _ERROR_IF_NC_PTR_COULD_BE_NULL(nc_ptr_name); \
        _PRIVATE_NC_NAME(nc_ptr_name) -= value_to_subtract; \
    }

#endif /* NULL_CHECKED_PTR_H */
