#version 430

in vec3 f_position;
in vec2 f_tex_coord;
in vec3 f_normal;

uniform bool is_first_pass;
uniform int resolution_x;
uniform int resolution_y;
uniform vec3 spherical_harmonic_coefficients[9];

uniform sampler2D surface_colour_texture;
uniform sampler2D second_pass_texture;
uniform sampler2D SH_AO_map[9];

layout (location=0) out vec3 output_image;
layout (location=1) out vec3 output_derivative_0;


void render_image(){
    output_image = texture2D(surface_colour_texture,  f_tex_coord);
}

void render_derivatives(){
    vec3 output_derivative_0 = vec3(1.0,0.0,0.0);
}


void main(){
    if (is_first_pass){
        render_image();
    }
    else{
        render_derivatives();
    }   
}
