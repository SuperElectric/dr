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
//uniform mat4 camera_transform_matrix_derivative;
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


float depth(float is_not_sky, vec4 position){
    if (is_not_sky > 0.5){
        return -(camera_transform_matrix*position).z;
    }
    else{
        return 1000.0;
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

    float depth_center = depth(is_not_sky_center, position_center);
    float depth_left = depth(is_not_sky_left, position_left);
    float depth_right = depth(is_not_sky_right, position_right);
    float depth_up = depth(is_not_sky_up, position_up);
    float depth_down = depth(is_not_sky_down, position_down);

    mat4 matrix_direction = mat4(0.0);
    matrix_direction[3][0] = 1.0;
    matrix_direction = camera_projection_matrix*camera_transform_matrix*matrix_direction; // derivative wrt object x position in world space

    vec2 motion_vector_center = motion_vector(is_not_sky_center, position_center, matrix_direction);
    vec2 motion_vector_left = motion_vector(is_not_sky_left, position_left, matrix_direction);
    vec2 motion_vector_right = motion_vector(is_not_sky_right, position_right, matrix_direction);
    vec2 motion_vector_up = motion_vector(is_not_sky_up, position_up, matrix_direction);
    vec2 motion_vector_down = motion_vector(is_not_sky_down, position_down, matrix_direction);
        
    vec4 derivative = vec4(0.0,0.0,0.0,1.0);
    if (depth_right < depth_center){
        derivative = derivative + 0.5*(colour_center - colour_right)*motion_vector_right.x;
    }
    else{
        derivative = derivative + 0.5*(colour_center - colour_right)*motion_vector_center.x;
    }
    if (depth_left < depth_center){
        derivative = derivative + 0.5*(colour_left - colour_center)*motion_vector_left.x;
    }
    else{
        derivative = derivative + 0.5*(colour_left - colour_center)*motion_vector_center.x;
    }
    if (depth_up < depth_center){
        derivative = derivative + 0.5*(colour_center - colour_up)*motion_vector_up.y;
    }
    else{
        derivative = derivative + 0.5*(colour_center - colour_up)*motion_vector_center.y;
    }
    if (depth_down < depth_center){
        derivative = derivative + 0.5*(colour_down - colour_center)*motion_vector_down.y;
    }
    else{
        derivative = derivative + 0.5*(colour_down - colour_center)*motion_vector_center.y;
    }

    output_derivatives = derivative.xyz;
//    output_derivatives = colour_center.xyz;
}


void main(){
    if (is_first_pass){
        render_image();
    }
    else{
        render_derivatives();
    }   
}
