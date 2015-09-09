#version 430

in vec3 f_position;
in vec3 f_global_position;
in vec2 f_tex_coord;
in vec3 f_normal;

uniform bool is_first_pass;
uniform int resolution_x;
uniform int resolution_y;
uniform vec3 spherical_harmonic_coefficients[9];
uniform mat4 camera_transform_matrix;
uniform mat4 camera_projection_matrix;

uniform sampler2D surface_colour_texture;
uniform sampler2D first_pass_colour;
uniform sampler2D first_pass_position;
uniform sampler2D first_pass_is_not_sky;
uniform sampler2D SH_AO_map;

layout (location=0) out vec3 output_image;
layout (location=1) out vec3 output_position;
layout (location=2) out float output_is_not_sky;
layout (location=3) out vec3 output_derivatives;


vec2 motion_vector(float is_not_sky, vec4 position, mat4 D){  // D = derivative direction
    if (is_not_sky < 0.5){
        return vec2(0.0,0.0);
    }
    else{
        mat4 M = camera_projection_matrix*camera_transform_matrix;
        return ((D*position).xy - (D*position).w * (M*position).xy / (M*position).w)
               / (M*position).w;
    }
}


void render_image(){
    output_image = vec3(texture2D(surface_colour_texture,  f_tex_coord));
    output_position = f_global_position;
    output_is_not_sky = 1.0;
}

void render_derivatives(){
    float pixel_size_x = 1.0 / resolution_x;
    float pixel_size_y = 1.0 / resolution_y;
    vec4 colour_center = texture2D(first_pass_colour, f_tex_coord);
    vec4 colour_left = texture2D(first_pass_colour, f_tex_coord - vec2(pixel_size_x, 0.0));
    vec4 colour_right = texture2D(first_pass_colour, f_tex_coord + vec2(pixel_size_x, 0.0));
    vec4 colour_up = texture2D(first_pass_colour, f_tex_coord + vec2(0.0, pixel_size_y));
    vec4 colour_down = texture2D(first_pass_colour, f_tex_coord - vec2(0.0, pixel_size_y));
    
    vec4 position_center = texture2D(first_pass_position, f_tex_coord);
    vec4 position_left = texture2D(first_pass_position, f_tex_coord - vec2(pixel_size_x, 0.0));
    vec4 position_right = texture2D(first_pass_position, f_tex_coord + vec2(pixel_size_x, 0.0));
    vec4 position_up = texture2D(first_pass_position, f_tex_coord + vec2(0.0, pixel_size_y));
    vec4 position_down = texture2D(first_pass_position, f_tex_coord - vec2(0.0, pixel_size_y));

    float is_not_sky_center = texture2D(first_pass_is_not_sky, f_tex_coord).x;
    float is_not_sky_left = texture2D(first_pass_is_not_sky, f_tex_coord - vec2(pixel_size_x, 0.0)).x;
    float is_not_sky_right = texture2D(first_pass_is_not_sky, f_tex_coord + vec2(pixel_size_x, 0.0)).x;
    float is_not_sky_up = texture2D(first_pass_is_not_sky, f_tex_coord + vec2(0.0, pixel_size_y)).x;
    float is_not_sky_down = texture2D(first_pass_is_not_sky, f_tex_coord - vec2(0.0, pixel_size_y)).x;
    
    mat4 matrix_direction = mat4(0.0);
    matrix_direction[3][0] = 1.0;
    matrix_direction = camera_projection_matrix*camera_transform_matrix*matrix_direction; // moving the object in the x direction, in world space

    vec2 motion_vector_center = motion_vector(is_not_sky_center, position_center, matrix_direction);
    vec2 motion_vector_left = motion_vector(is_not_sky_left, position_left, matrix_direction);
    vec2 motion_vector_right = motion_vector(is_not_sky_right, position_right, matrix_direction);
    vec2 motion_vector_up = motion_vector(is_not_sky_up, position_up, matrix_direction);
    vec2 motion_vector_down = motion_vector(is_not_sky_down, position_down, matrix_direction);
    
    float depth;
    if (is_not_sky_center < 0.5){
        depth = 100.0;
    }
    else{
        depth = -(camera_transform_matrix*position_center).z;
    }
    depth = 0.1*depth;
    output_derivatives = vec3(colour_right - colour_left);
    output_derivatives = vec3(motion_vector_center, 0.0);
    output_derivatives = vec3(depth,0.0,0.0);
}


void main(){
    if (is_first_pass){
        render_image();
    }
    else{
        render_derivatives();
    }   
}
