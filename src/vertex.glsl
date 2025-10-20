#version 330 core

struct quat{
    float w;
    float i;
    float j;
    float k;
};

quat quat_normalize(quat q) {
    float q_mag = sqrt(q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k);

    return quat(q.w / q_mag,
               q.i / q_mag,
               q.j / q_mag,
               q.k / q_mag);
}

quat vec_to_quat(vec3 v1) {
    return quat(0.0, v1.x, v1.y, v1.z);
}

quat quat_mul(quat q1, quat q2) {
    vec3 q1_i = vec3(q1.i, q1.j, q1.k);
    vec3 q2_i = vec3(q2.i, q2.j, q2.k);

    float scalar = q1.w * q2.w - dot(q1_i, q2_i);
    vec3 imaginary = (q2_i * q1.w) + (q1_i * q2.w) + cross(q1_i, q2_i);

    return quat(scalar, imaginary.x, imaginary.y, imaginary.z);
}

vec3 quat_rotate_vector(float theta, vec3 axis, vec3 v) {
    float ca = cos(theta/2.0);
    float sa = sin(theta/2.0);

    axis = normalize(axis);

    quat q = quat(ca, sa * axis.x, sa * axis.y, sa * axis.z);
    quat q_inv = quat(ca, -sa * axis.x, -sa * axis.y, -sa * axis.z);

    quat quat_point = vec_to_quat(v);
    quat rotated = quat_mul(quat_mul(q, quat_point), q_inv);

    return vec3(rotated.i, rotated.j, rotated.k);
}

layout (location = 0) in vec3 aPos;
out vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    // Extract rotation angle from quaternion (simplified approach)
    float angle = radians(29);
    vec3 axis = normalize(vec3(1, 0, 0));
    
    // Rotate the position using quaternion rotation
    vec3 rotated_pos = quat_rotate_vector(angle, axis, aPos);

    mat4 coordinates = projection * view * model;
    gl_Position = coordinates * vec4(rotated_pos, 1.0);
    pos = aPos;
}
