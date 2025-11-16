#pragma once
#include <string.h>
#include "glad/glad.h"
#include "state.h"


typedef enum {
    UNIFORM_FLOAT1,
    UNIFORM_FLOAT2,
    UNIFORM_FLOAT3,
    UNIFORM_FLOAT4,
    UNIFORM_INT1,
    UNIFORM_INT2,
    UNIFORM_INT3,
    UNIFORM_INT4
} Uniform_Type;

typedef union {
	float f4[4];
	float f3[3];
	float f2[2];
	float f1;

	int i4[4];
	int i3[3];
	int i2[2];
	int i1;
} Uniform_Members;

typedef struct {
	int location;
	Uniform_Type type;
	Uniform_Members values;
} Uniform;

Uniform uniform_init(State *state, char *name, void *value, Uniform_Type type);
void uniform_send(Uniform *uniform);

