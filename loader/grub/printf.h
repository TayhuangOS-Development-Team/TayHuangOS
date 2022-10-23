#include <tayhuang/attributes.h>
#include <tayhuang/types.h>
#include <stdarg.h>

PUBLIC int vsprintf(char *buffer, const char *format, va_list args);
PUBLIC int sprintf(char *buffer, const char *format, ...);