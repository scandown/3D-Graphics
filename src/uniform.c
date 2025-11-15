#include "uniform.h"

Uniform uniform_init(State *state, char *name, Uniform_Type type) {
	Uniform uniform;
	uniform.location = glGetUniformLocation(state->program, name);
	uniform.type = type;

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
		default:
			break;

	}
}

//int rotloc = glGetUniformLocation(program, "rot");
//glUniform4fv(rotloc, 1, result);
