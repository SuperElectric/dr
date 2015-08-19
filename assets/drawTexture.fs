#version 450

out vec4 color;

uniform sampler2D sampler0;

void main(){
    vec2 tex_coord = vec2(gl_PointCoord.x, -gl_PointCoord.y);
    color = texture2D(sampler0, tex_coord);
}
