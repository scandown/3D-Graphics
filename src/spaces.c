#include "spaces.h"


void space_scale(Space *space, vec3 scale) {
       glm_scale(space->matrix, scale);
}
void space_translate(Space *space, vec3 translation) {
       glm_translate(space->matrix, translation);
}
void space_set_uniform(Space *space) {
       glUniformMatrix4fv(space->location, 1, false, (const float *)space->matrix);
}




void setup_space(Space *space, char *name, int program) {
       glm_mat4_identity(space->matrix);
       space->location = glGetUniformLocation(program, name);

       space->scale = space_scale;
       space->translate = space_translate;
       space->set_uniform = space_set_uniform;
}
