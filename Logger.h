#include "WSMan.h"

#ifndef WSMAN_LOGGER_H
#define WSMAN_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#define WSMAN_LOG_TRACE   0
#define WSMAN_LOG_DEBUG   1
#define WSMAN_LOG_INFO    2
#define WSMAN_LOG_WARN    3
#define WSMAN_LOG_ERROR   4
#define WSMAN_LOG_FATAL   5
#define WSMAN_LOG_OFF     6

bool WSMan_IsLogLevelActive(uint32_t logLevel);

#define WSMan_LogPrint(_log_level, ...)                                                \
	do                                                                                 \
	{                                                                                  \
		if (WSMan_IsLogLevelActive(WSMAN_LOG_ ## _log_level))                          \
		{                                                                              \
			WSMan_Log(__VA_ARGS__);                                                    \
		}                                                                              \
	} while (0)

#define WSMan_LogDump(_log_level, ...)                                                 \
	do                                                                                 \
	{                                                                                  \
		if (WSMan_IsLogLevelActive(WSMAN_LOG_ ## _log_level))                          \
		{                                                                              \
			WSMan_LogHexDump(__VA_ARGS__);                                             \
		}                                                                              \
	} while (0)

bool WSMan_Log(const char* format, ...);
void WSMan_LogHexDump(const uint8_t* data, size_t size);

void WSMan_LogOpen();
void WSMan_LogClose();
void WSMan_SetLogEnabled(bool logEnabled);
void WSMan_SetLogLevel(uint32_t logLevel);
void WSMan_SetLogFilePath(const char* logFilePath);

#ifdef __cplusplus
}
#endif

#endif /* WSMAN_LOGGER_H */
