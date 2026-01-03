#include "uniform.h"

//glGetUniformLocation(program, name);
//glUniformMatrix4fv(space->location, 1, false, (const float *)space->matrix);

Uniform uniform_init(unsigned int program, char *name, void *value, Uniform_Type type) {
	Uniform uniform;
	uniform.location = glGetUniformLocation(program, name);
	uniform.type = type;

	switch (uniform.type) {
		case UNIFORM_FLOAT1:
			memcpy(&uniform.values.f1, value, sizeof(float) * 1);
			break;
		case UNIFORM_FLOAT2:
			memcpy(&uniform.values.f2, value, sizeof(float) * 2);
			break;
		case UNIFORM_FLOAT3:
			memcpy(&uniform.values.f3, value, sizeof(float) * 3);
			break;
		case UNIFORM_FLOAT4:
			memcpy(&uniform.values.f4, value, sizeof(float) * 4);
			break;
		case UNIFORM_INT1:
			memcpy(&uniform.values.i1, value, sizeof(int) * 1);
			break;
		case UNIFORM_INT2:
			memcpy(&uniform.values.i2, value, sizeof(int) * 2);
			break;
		case UNIFORM_INT3:
			memcpy(&uniform.values.i3, value, sizeof(int) * 3);
			break;
		case UNIFORM_INT4:
			memcpy(&uniform.values.i4, value, sizeof(int) * 4);
			break;
		case UNIFORM_MAT2:
			memcpy(&uniform.values.m2, value, sizeof(float) * 4);
			break;
		case UNIFORM_MAT3:
			memcpy(&uniform.values.m3, value, sizeof(float) * 9);
			break;
		case UNIFORM_MAT4:
			memcpy(&uniform.values.m3, value, sizeof(float) * 16);
			break;
		default:
			break;

	}

	return uniform;
}


void uniform_send(Uniform *uniform) {
	switch (uniform->type) {
		case UNIFORM_FLOAT1:
			glUniform1f(uniform->location, uniform->values.f1);
			break;
		case UNIFORM_FLOAT2:
			glUniform2fv(uniform->location, 1, uniform->values.f2);
			break;
		case UNIFORM_FLOAT3:
			glUniform3fv(uniform->location, 1, uniform->values.f3);
			break;
		case UNIFORM_FLOAT4:
			glUniform4fv(uniform->location, 1, uniform->values.f4);
			break;
		case UNIFORM_INT1:
			glUniform1i(uniform->location, uniform->values.i1);
			break;
		case UNIFORM_INT2:
			glUniform2iv(uniform->location, 1, uniform->values.i2);
			break;
		case UNIFORM_INT3:
			glUniform3iv(uniform->location, 1, uniform->values.i3);
			break;
		case UNIFORM_INT4:
			glUniform4iv(uniform->location, 1, uniform->values.i4);
			break;
		case UNIFORM_MAT2:
			glUniformMatrix2fv(uniform->location, 1, false, (float *)uniform->values.m2);
			break;
		case UNIFORM_MAT3:
			glUniformMatrix3fv(uniform->location, 1, false, (float *)uniform->values.m3);
			break;
		case UNIFORM_MAT4:
			glUniformMatrix4fv(uniform->location, 1, false, (float *)uniform->values.m4);
			break;
		default:
			break;

	}
}

//int rotloc = glGetUniformLocation(program, "rot");
//glUniform4fv(rotloc, 1, result);
