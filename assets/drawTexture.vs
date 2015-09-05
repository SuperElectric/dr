#version 430

in vec3 vertex_position;
in vec2 vertex_tex_coord;
out vec3 f_position;
out vec2 f_tex_coord;

void main()
{
    f_position = vertex_position;
    f_tex_coord = vertex_tex_coord;
    gl_Position = vec4(vertex_position,1.0);
}
