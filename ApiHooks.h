#include "WSMan.h"

#ifndef WSMAN_API_HOOKS_H
#define WSMAN_API_HOOKS_H

#ifdef __cplusplus
extern "C" {
#endif

uint32_t WSMan_AttachHooks();
uint32_t WSMan_DetachHooks();

#ifdef __cplusplus
}
#endif

#endif /* WSMAN_API_HOOKS_H */
