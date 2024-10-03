#version 330 core

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_normal0;
uniform sampler2D texture_specular0;
uniform sampler2D texture_height0;

in vec3 norm_vec;
in vec2 frag_text_coord;
out vec4 outCol;


vec3 lightning(vec3 norm_vec) {
    vec3 res_color = vec3(0.1);

    vec3 sun_direction = normalize(vec3(0) - vec3(-1,1,0));
    res_color += 0.5 * max(0.0, dot(-sun_direction, norm_vec)) * vec3(1);
    return res_color;
}

void main() {
    // outCol = texture(texture_height0, frag_text_coord);
    outCol = vec4(lightning(norm_vec), 1.0);
}