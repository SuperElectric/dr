#version 430

in vec3 vertex_position;
in vec2 vertex_tex_coord;
in vec3 vertex_normal;
out vec3 f_position;
out vec3 f_global_position;
out vec2 f_tex_coord;
out vec3 f_normal;
uniform bool is_first_pass;
uniform mat4 camera_transform_matrix;
uniform mat4 camera_projection_matrix;

void main()
{
    vec4 position = camera_transform_matrix*vec4(vertex_position,1.0);
    mat3 camera_rotation_matrix = mat3(camera_transform_matrix);
    f_position = position.xyz;
    f_global_position = vertex_position;
    f_normal = normalize(camera_rotation_matrix*vertex_normal);

    if (is_first_pass){
        f_tex_coord = vec2(vertex_tex_coord.x, 1.0 - vertex_tex_coord.y);
        vec4 pos = camera_projection_matrix*position;
        gl_Position = pos;
    }
    else{
        f_tex_coord = vertex_tex_coord;
        gl_Position = vec4(vertex_position,1.0);
    }
}
