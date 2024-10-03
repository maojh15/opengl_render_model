#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormvec;
layout (location = 2) in vec2 text_coord;

uniform mat4 perp_trans;
uniform mat4 camera_trans;

out vec2 frag_text_coord;
out vec3 norm_vec;

void main() {
    gl_Position = perp_trans * camera_trans * vec4(aPos * 0.7 - vec3(0,3,0), 1.0);
    frag_text_coord = text_coord;
    norm_vec = aNormvec;
}