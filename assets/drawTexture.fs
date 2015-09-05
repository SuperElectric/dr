#version 430

in vec3 f_position;
in vec2 f_tex_coord;
uniform int resolution_x;
uniform int resolution_y;
out vec4 color;

uniform sampler2D image;
uniform sampler2D depth;
uniform sampler2D derivatives;

void main(){
    color = texture2D(image, f_tex_coord);
}
