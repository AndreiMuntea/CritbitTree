#ifndef __UNICODE_STRING_H__ 
#define __UNICODE_STRING_H__ 

#include <Windows.h>

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

#endif //__UNICODE_STRING_H__ 