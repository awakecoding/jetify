
#include "ApiHooks.h"

#include "Logger.h"
#include "Utils.h"

#include <winhttp.h>

#include <detours.h>

#define WSMAN_DETOUR_ATTACH(_realFn, _hookFn) \
	if (_realFn) DetourAttach((PVOID*)(&_realFn), _hookFn);

#define WSMAN_DETOUR_DETACH(_realFn, _hookFn) \
	if (_realFn) DetourDetach((PVOID*)(&_realFn), _hookFn);

#define WSMAN_GETPROCADDRESS(_funcPtr, _funcType, _hModule, _funcName) \
	_funcPtr = ( _funcType ) GetProcAddress(_hModule, _funcName);

static bool g_InjectConfigTest = false;

HINTERNET(WINAPI * Real_WinHttpOpen)(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags) = WinHttpOpen;

HINTERNET Hook_WinHttpOpen(LPCWSTR pszAgentW, DWORD dwAccessType, LPCWSTR pszProxyW, LPCWSTR pszProxyBypassW, DWORD dwFlags)
{
    HINTERNET hInternet;
    char* pszAgentA = NULL;
    char* pszProxyA = NULL;
    char* pszProxyBypassA = NULL;

    if (g_InjectConfigTest) {
        dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
        pszProxyW = L"IT-HELP-GW.ad.it-help.ninja:808";
    }
    
    if (pszAgentW)
        WSMan_ConvertFromUnicode(CP_UTF8, 0, pszAgentW, -1, &pszAgentA, 0, NULL, NULL);

    if (pszProxyW)
        WSMan_ConvertFromUnicode(CP_UTF8, 0, pszProxyW, -1, &pszProxyA, 0, NULL, NULL);

    if (pszProxyBypassW)
        WSMan_ConvertFromUnicode(CP_UTF8, 0, pszProxyBypassW, -1, &pszProxyBypassA, 0, NULL, NULL);

    WSMan_LogPrint(DEBUG, "WinHttpOpen(dwAccessType: %d, dwFlags: 0x%08X)", dwAccessType, dwFlags);
    WSMan_LogPrint(DEBUG, "pszAgent: \"%s\"", pszAgentA ? pszAgentA : "");
    WSMan_LogPrint(DEBUG, "pszProxy: \"%s\" pszProxyBypass: \"%s\"",
        pszProxyA ? pszProxyA : "", pszProxyBypassA ? pszProxyBypassA : "");

    hInternet = Real_WinHttpOpen(pszAgentW, dwAccessType, pszProxyW, pszProxyBypassW, dwFlags);

    if (pszAgentA)
        free(pszAgentA);

    if (pszProxyA)
        free(pszProxyA);

    if (pszProxyBypassA)
        free(pszProxyBypassA);

    return hInternet;
}

HINTERNET(WINAPI* Real_WinHttpConnect)(HINTERNET hSession, LPCWSTR pszServerNameW, INTERNET_PORT nServerPort, DWORD dwReserved) = WinHttpConnect;

HINTERNET Hook_WinHttpConnect(HINTERNET hSession, LPCWSTR pszServerNameW, INTERNET_PORT nServerPort, DWORD dwReserved)
{
    HINTERNET hInternet;
    char* pszServerNameA = NULL;

    if (pszServerNameW)
        WSMan_ConvertFromUnicode(CP_UTF8, 0, pszServerNameW, -1, &pszServerNameA, 0, NULL, NULL);

    WSMan_LogPrint(DEBUG, "WinHttpConnect(hSession: %p, pszServerName: %s nServerPort: %d)",
        hSession, pszServerNameA ? pszServerNameA : "", (int)nServerPort);
    
    hInternet = Real_WinHttpConnect(hSession, pszServerNameW, nServerPort, dwReserved);
    
    if (pszServerNameA)
        free(pszServerNameA);

    return hInternet;
}

BOOL(WINAPI* Real_WinHttpSetOption)(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength) = WinHttpSetOption;

BOOL Hook_WinHttpSetOption(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength)
{
    BOOL success;
    
    WSMan_LogPrint(DEBUG, "WinHttpSetOption(hInternet: %p, dwOption: %d, dwBufferLength: %d)",
        hInternet, dwOption, dwBufferLength);
    
    success = Real_WinHttpSetOption(hInternet, dwOption, lpBuffer, dwBufferLength);
    
    return success;
}

HINTERNET(WINAPI* Real_WinHttpOpenRequest)(HINTERNET hConnect, LPCWSTR pszVerbW,
    LPCWSTR pszObjectNameW, LPCWSTR pszVersionW, LPCWSTR pszReferrerW,
    LPCWSTR* ppszAcceptTypesW, DWORD dwFlags) = WinHttpOpenRequest;

HINTERNET Hook_WinHttpOpenRequest(HINTERNET hConnect, LPCWSTR pszVerbW,
    LPCWSTR pszObjectNameW, LPCWSTR pszVersionW, LPCWSTR pszReferrerW,
    LPCWSTR* ppszAcceptTypesW, DWORD dwFlags)
{
    HINTERNET hRequest;

    WSMan_LogPrint(DEBUG, "WinHttpOpenRequest(hConnect: %p)",
        hConnect);

    hRequest = Real_WinHttpOpenRequest(hConnect, pszVerbW, pszObjectNameW, pszVersionW, pszReferrerW, ppszAcceptTypesW, dwFlags);

    return hRequest;
}

BOOL (WINAPI* Real_WinHttpSendRequest)(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength,
    LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext) = WinHttpSendRequest;

BOOL Hook_WinHttpSendRequest(HINTERNET hRequest, LPCWSTR lpszHeaders, DWORD dwHeadersLength,
    LPVOID lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext)
{
    BOOL success;

    WSMan_LogPrint(DEBUG, "WinHttpSendRequest(hRequest: %p)",
        hRequest);

    if (g_InjectConfigTest) {
        WCHAR* pszUserNameW = L"anonymous";
        WCHAR* pszPasswordW = L"none";
        WinHttpSetCredentials(hRequest, WINHTTP_AUTH_TARGET_PROXY,
            WINHTTP_AUTH_SCHEME_BASIC, pszUserNameW, pszPasswordW, NULL);
    }

    success = Real_WinHttpSendRequest(hRequest, lpszHeaders, dwHeadersLength,
        lpOptional, dwOptionalLength, dwTotalLength, dwContext);

    return success;
}

BOOL(WINAPI* Real_WinHttpCloseHandle)(HINTERNET hInternet) = WinHttpCloseHandle;

BOOL Hook_WinHttpCloseHandle(HINTERNET hInternet)
{
    BOOL success;
    WSMan_LogPrint(DEBUG, "WinHttpCloseHandle(hInternet: %p)", hInternet);
    success = Real_WinHttpCloseHandle(hInternet);
    return success;
}

uint32_t WSMan_AttachHooks()
{
    LONG error;
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    WSMAN_DETOUR_ATTACH(Real_WinHttpOpen, Hook_WinHttpOpen);
    WSMAN_DETOUR_ATTACH(Real_WinHttpConnect, Hook_WinHttpConnect);
    WSMAN_DETOUR_ATTACH(Real_WinHttpSetOption, Hook_WinHttpSetOption);
    WSMAN_DETOUR_ATTACH(Real_WinHttpOpenRequest, Hook_WinHttpOpenRequest);
    WSMAN_DETOUR_ATTACH(Real_WinHttpCloseHandle, Hook_WinHttpCloseHandle);
    error = DetourTransactionCommit();
    return (uint32_t) error;
}

uint32_t WSMan_DetachHooks()
{
    LONG error;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    WSMAN_DETOUR_DETACH(Real_WinHttpOpen, Hook_WinHttpOpen);
    WSMAN_DETOUR_DETACH(Real_WinHttpConnect, Hook_WinHttpConnect);
    WSMAN_DETOUR_DETACH(Real_WinHttpSetOption, Hook_WinHttpSetOption);
    WSMAN_DETOUR_DETACH(Real_WinHttpOpenRequest, Hook_WinHttpOpenRequest);
    WSMAN_DETOUR_DETACH(Real_WinHttpCloseHandle, Hook_WinHttpCloseHandle);
    error = DetourTransactionCommit();
    return (uint32_t)error;
}
