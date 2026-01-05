#include "uniform.h"
#include "state.h"


Uniform uniform_set_data(void *value, Uniform_Type type) {
	Uniform uniform;
	uniform.type = type;

	switch (uniform.type) {
		case UNIFORM_FLOAT1:
			memcpy(&uniform.value.f1, value, sizeof(float) * 1);
			break;
		case UNIFORM_FLOAT2:
			memcpy(&uniform.value.f2, value, sizeof(float) * 2);
			break;
		case UNIFORM_FLOAT3:
			memcpy(&uniform.value.f3, value, sizeof(float) * 3);
			break;
		case UNIFORM_FLOAT4:
			memcpy(&uniform.value.f4, value, sizeof(float) * 4);
			break;
		case UNIFORM_INT1:
			memcpy(&uniform.value.i1, value, sizeof(int) * 1);
			break;
		case UNIFORM_INT2:
			memcpy(&uniform.value.i2, value, sizeof(int) * 2);
			break;
		case UNIFORM_INT3:
			memcpy(&uniform.value.i3, value, sizeof(int) * 3);
			break;
		case UNIFORM_INT4:
			memcpy(&uniform.value.i4, value, sizeof(int) * 4);
			break;
		case UNIFORM_MAT2:
			memcpy(&uniform.value.m2, value, sizeof(float) * 4);
			break;
		case UNIFORM_MAT3:
			memcpy(&uniform.value.m3, value, sizeof(float) * 9);
			break;
		case UNIFORM_MAT4:
			memcpy(&uniform.value.m3, value, sizeof(float) * 16);
			break;
		default:
			break;

	}

	return uniform;
}


void uniform_send_to_gpu(Uniform *uniform, unsigned int program, char *name) {
	uniform->location = glGetUniformLocation(program, name);

	switch (uniform->type) {
		case UNIFORM_FLOAT1:
			glUniform1f(uniform->location, uniform->value.f1);
			break;
		case UNIFORM_FLOAT2:
			glUniform2fv(uniform->location, 1, uniform->value.f2);
			break;
		case UNIFORM_FLOAT3:
			glUniform3fv(uniform->location, 1, uniform->value.f3);
			break;
		case UNIFORM_FLOAT4:
			glUniform4fv(uniform->location, 1, uniform->value.f4);
			break;
		case UNIFORM_INT1:
			glUniform1i(uniform->location, uniform->value.i1);
			break;
		case UNIFORM_INT2:
			glUniform2iv(uniform->location, 1, uniform->value.i2);
			break;
		case UNIFORM_INT3:
			glUniform3iv(uniform->location, 1, uniform->value.i3);
			break;
		case UNIFORM_INT4:
			glUniform4iv(uniform->location, 1, uniform->value.i4);
			break;
		case UNIFORM_MAT2:
			glUniformMatrix2fv(uniform->location, 1, false, (float *)uniform->value.m2);
			break;
		case UNIFORM_MAT3:
			glUniformMatrix3fv(uniform->location, 1, false, (float *)uniform->value.m3);
			break;
		case UNIFORM_MAT4:
			glUniformMatrix4fv(uniform->location, 1, false, (float *)uniform->value.m4);
			break;
		default:
			break;

	}
}
