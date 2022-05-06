
#include "Logger.h"

#include <stdio.h>
#include <stdlib.h>

#include "Environment.h"

static bool g_LogInitialized = false;

static FILE* g_LogFile = NULL;
static bool g_LogEnabled = true;
static char g_LogFilePath[WSMAN_MAX_PATH] = { 0 };

static uint32_t g_LogLevel = WSMAN_LOG_DEBUG;

#define WSMAN_LOG_MAX_LINE    8192

bool WSMan_IsLogLevelActive(uint32_t logLevel)
{
    if (!g_LogEnabled)
        return false;

    if (g_LogLevel == WSMAN_LOG_OFF)
        return false;

    return logLevel >= g_LogLevel;
}

bool WSMan_LogVA(const char* format, va_list args)
{
    if (!g_LogFile)
        return true;

    char message[WSMAN_LOG_MAX_LINE];
    vsnprintf_s(message, WSMAN_LOG_MAX_LINE - 1, _TRUNCATE, format, args);
    strcat_s(message, WSMAN_LOG_MAX_LINE - 1, "\n");

    if (g_LogFile) {
        fprintf(g_LogFile, message);
        fflush(g_LogFile); // WARNING: performance drag
    }

    return true;
}

bool WSMan_Log(const char* format, ...)
{
	bool status;
	va_list args;
	va_start(args, format);
	status = WSMan_LogVA(format, args);
	va_end(args);
	return status;
}

void WSMan_LogHexDump(const uint8_t* data, size_t size)
{
    int i, ln, hn;
	const uint8_t* p = data;
    size_t width = 16;
    size_t offset = 0;
    size_t chunk = 0;
    char line[512];
    char* bin2hex = "0123456789ABCDEF";

    while (offset < size) {
        chunk = size - offset;

        if (chunk >= width)
            chunk = width;

        for (i = 0; i < chunk; i++)
        {
            ln = p[i] & 0xF;
            hn = (p[i] >> 4) & 0xF;

            line[i * 2] = bin2hex[hn];
            line[(i * 2) + 1] = bin2hex[ln];
        }

        line[chunk * 2] = ' ';

        for (i = (int) chunk; i < width; i++) {
            line[i * 2] = ' ';
            line[(i * 2) + 1] = ' ';
        }

        char* side = &line[(width * 2) + 1];

        for (i = 0; i < chunk; i++)
        {
            char c = ((p[i] >= 0x20) && (p[i] < 0x7F)) ? p[i] : '.';
            side[i] = c;
        }
        side[i] = '\n';
        side[i+1] = '\0';

        if (g_LogFile) {
            fwrite(line, 1, strlen(line), g_LogFile);
        }

        offset += chunk;
        p += chunk;
    }
}

void WSMan_LogEnvInit()
{
    char* envvar;

    if (g_LogInitialized)
        return;

    bool logEnabled = WSMan_GetEnvBool("WSMAN_LOG_LEVEL", false);

    if (logEnabled) {
        // only set if true to avoid overriding current value
        WSMan_SetLogEnabled(true);
    }

    envvar = WSMan_GetEnv("WSMAN_LOG_LEVEL");

    if (envvar) {
        int ival = atoi(envvar);

        if ((ival >= 0) && (ival <= 6)) {
            WSMan_SetLogLevel((uint32_t) ival);
        }
        else {
            if (!strcmp(envvar, "TRACE")) {
                WSMan_SetLogLevel(WSMAN_LOG_TRACE);
            }
            else if (!strcmp(envvar, "DEBUG")) {
                WSMan_SetLogLevel(WSMAN_LOG_DEBUG);
            }
            else if (!strcmp(envvar, "INFO")) {
                WSMan_SetLogLevel(WSMAN_LOG_INFO);
            }
            else if (!strcmp(envvar, "WARN")) {
                WSMan_SetLogLevel(WSMAN_LOG_WARN);
            }
            else if (!strcmp(envvar, "ERROR")) {
                WSMan_SetLogLevel(WSMAN_LOG_ERROR);
            }
            else if (!strcmp(envvar, "FATAL")) {
                WSMan_SetLogLevel(WSMAN_LOG_FATAL);
            }
            else if (!strcmp(envvar, "OFF")) {
                WSMan_SetLogLevel(WSMAN_LOG_OFF);
            }
        }
    }

    free(envvar);

    envvar = WSMan_GetEnv("WSMAN_LOG_FILE_PATH");

    if (envvar) {
        WSMan_SetLogFilePath(envvar);
    }

    free(envvar);

    g_LogInitialized = true;
}

void WSMan_LogOpen()
{
    WSMan_LogEnvInit();

    if (!g_LogEnabled)
        return;

    if (g_LogFilePath[0] == '\0') {
        ExpandEnvironmentStringsA("%TEMP%\\WSMan.log", g_LogFilePath, WSMAN_MAX_PATH);
    }

    g_LogFile = fopen(g_LogFilePath, "wb");
}

void WSMan_LogClose()
{
    if (g_LogFile) {
        fclose(g_LogFile);
        g_LogFile = NULL;
    }
}

void WSMan_SetLogEnabled(bool logEnabled)
{
    g_LogEnabled = logEnabled;
}

void WSMan_SetLogLevel(uint32_t logLevel)
{
    g_LogLevel = logLevel;
}

void WSMan_SetLogFilePath(const char* logFilePath)
{
    strcpy_s(g_LogFilePath, WSMAN_MAX_PATH, logFilePath);
}
