#include "WSMan.h"

#ifndef WSMAN_ENVIRONMENT_H
#define WSMAN_ENVIRONMENT_H

#ifdef __cplusplus
extern "C" {
#endif

bool WSMan_SetEnv(const char* name, const char* value);
char* WSMan_GetEnv(const char* name);
bool WSMan_EnvExists(const char* name);
bool WSMan_GetEnvBool(const char* name, bool defaultValue);
int WSMan_GetEnvInt(const char* name, int defaultValue);

#ifdef __cplusplus
}
#endif

#endif /* WSMAN_ENVIRONMENT_H */
