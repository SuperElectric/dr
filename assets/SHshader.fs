#version 430

in vec3 f_position;
in vec2 f_tex_coord;
in vec3 f_normal;

uniform bool is_first_pass;
uniform int resolution_x;
uniform int resolution_y;
uniform vec3 spherical_harmonic_coefficients[9];

uniform sampler2D surface_colour_texture;
uniform sampler2D first_pass_colour;
uniform sampler2D first_pass_depth;
uniform sampler2D SH_AO_map;

layout (location=0) out vec3 output_image;
layout (location=1) out float output_depth;
layout (location=2) out vec3 output_derivative;


void render_image(){
    output_image = vec3(texture2D(surface_colour_texture,  f_tex_coord));
    output_depth = 0.5;//f_position.z;
//    output_derivative = 0.01*spherical_harmonic_coefficients[0];
}

void render_derivatives(){
    //output_image = vec3(0.01*spherical_harmonic_coefficients[0].x, 0.0,0.0);
    output_derivative = 0.01*spherical_harmonic_coefficients[0];
}


void main(){
    if (is_first_pass){
        render_image();
    }
    else{
        render_derivatives();
    }   
}
