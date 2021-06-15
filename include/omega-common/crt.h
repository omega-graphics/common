#include <stddef.h>
#include <stdint.h>

#ifndef OMEGA_COMMON_CRT_H
#define OMEGA_COMMON_CRT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __OmegaObject OmegaRTObject;
typedef int64_t OmegaObjectType;
typedef const char * OmegaCString;


typedef enum : int {
    FALSE = 0x00,
    TRUE = 0x01,
} OmegaCommonBool;

OmegaRTObject * omega_common_alloc(void *data,size_t size);

OmegaCommonBool omega_common_exists(OmegaRTObject *obj);

OmegaCommonBool omega_common_typecheck(OmegaRTObject *obj,OmegaObjectType type);

void omega_common_free(OmegaRTObject *obj);


#ifdef __cplusplus
}
#endif

#endif