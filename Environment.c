
#include "Environment.h"

#ifdef _WIN32
#include <UserEnv.h>
#endif

bool WSMan_SetEnv(const char* name, const char* value)
{
	return SetEnvironmentVariableA(name, value) ? true : false;
}

char* WSMan_GetEnv(const char* name)
{
	uint32_t size;
	char* env = NULL;

	size = GetEnvironmentVariableA(name, NULL, 0);

	if (!size)
		return NULL;

	env = (char*) malloc(size);

	if (!env)
		return NULL;

	if (GetEnvironmentVariableA(name, env, size) != size - 1)
	{
		free(env);
		return NULL;
	}

	return env;
}

bool WSMan_EnvExists(const char* name)
{
	if (!name)
		return false;

	return GetEnvironmentVariableA(name, NULL, 0) ? true : false;
}

bool WSMan_GetEnvBool(const char* name, bool defaultValue)
{
	char* env;
	bool value = defaultValue;

	env = WSMan_GetEnv(name);

	if (!env)
		return value;

	if ((strcmp(env, "1") == 0) || (_stricmp(env, "TRUE") == 0))
		value = true;
	else if ((strcmp(env, "0") == 0) || (_stricmp(env, "FALSE") == 0))
		value = false;

	free(env);

	return value;
}

int WSMan_GetEnvInt(const char* name, int defaultValue)
{
	char* env;
	int value = defaultValue;

	env = WSMan_GetEnv(name);

	if (!env)
		return value;

	value = atoi(env);

	free(env);

	return value;
}
