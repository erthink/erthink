#include "erthink_defs.h"

#ifndef LIBERTHINK_API
#if defined(LIBERTHINK_EXPORTS)
#define LIBERTHINK_API __dll_export
#elif defined(LIBERTHINK_IMPORTS)
#define LIBERTHINK_API __dll_import
#else
#define LIBERTHINK_API
#endif
#endif /* LIBERTHINK_API */

#ifdef __cplusplus
#if defined(__clang__) || __has_attribute(type_visibility)
#define LIBERTHINK_API_TYPE                                                    \
  LIBERTHINK_API __attribute__((type_visibility("default")))
#else
#define LIBERTHINK_API_TYPE LIBERTHINK_API
#endif
#else
#define LIBERTHINK_API_TYPE
#endif /* LIBERTHINK_API_TYPE */

#if defined(LIBERTHINK_IMPORTS)
#define LIBERTHINK_VERINFO_API __dll_import
#else
#define LIBERTHINK_VERINFO_API __dll_export
#endif /* LIBERTHINK_VERINFO_API */
