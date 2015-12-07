#version 430

in vec3 f_position;
in vec2 f_tex_coord;
uniform int resolution_x;
uniform int resolution_y;
out vec4 colour;

uniform sampler2D image;
uniform sampler2D position;
uniform sampler2D derivatives;

void main(){
    float pixel_size_x = 1.0 / resolution_x;
    float pixel_size_y = 1.0 / resolution_y;
    vec4 derivative = texture2D(derivatives, f_tex_coord);
    derivative = derivative + texture2D(derivatives, f_tex_coord + vec2(pixel_size_x, 0.0));
    derivative = derivative + texture2D(derivatives, f_tex_coord  + vec2(pixel_size_x, pixel_size_y));
    derivative = derivative + texture2D(derivatives, f_tex_coord  + vec2(0.0, pixel_size_y));
    derivative = derivative + texture2D(derivatives, f_tex_coord + vec2(-pixel_size_x, pixel_size_y));
    derivative = derivative + texture2D(derivatives, f_tex_coord  + vec2(-pixel_size_x, 0.0));
    derivative = derivative + texture2D(derivatives, f_tex_coord  + vec2(-pixel_size_x, -pixel_size_y));
    derivative = derivative + texture2D(derivatives, f_tex_coord + vec2(0.0, -pixel_size_y));
    derivative = derivative + texture2D(derivatives, f_tex_coord  + vec2(pixel_size_x, -pixel_size_y));
    derivative = derivative / 9.0;


    float magnitude = derivative.x + derivative.y + derivative.z;
    if (magnitude > 0){
        colour = vec4(magnitude, 0.0,0.0,1.0);
    }
    else{
        colour = vec4(0.0,0.0,-magnitude,1.0);
    }

    colour = vec4(0.5,0.5,0.5,1.0) + 10.0*derivative;
    //colour = texture2D(image, f_tex_coord);
}
