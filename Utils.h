#include "WSMan.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef WSMAN_UTILS_H
#define WSMAN_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

int WSMan_ConvertFromUnicode(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar,
    LPSTR* lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);

int WSMan_ConvertToUnicode(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte,
    LPWSTR* lpWideCharStr, int cchWideChar);

#ifdef __cplusplus
}
#endif

#endif /* WSMAN_UTILS_H */
