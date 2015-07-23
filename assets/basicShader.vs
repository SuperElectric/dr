#version 430

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
out vec3 fPosition;
out vec2 fTexCoord;
out vec3 fNormal;
uniform mat4 cameraTransformMatrix;
uniform mat4 cameraProjectionMatrix;

void main()
{	
    vec4 pos = cameraTransformMatrix*vec4(vertexPosition,1.0);
    mat3 cameraRotationMatrix = mat3(cameraTransformMatrix);
    fPosition = pos.xyz;
    fTexCoord = vertexTexCoord;
    fNormal = normalize(cameraRotationMatrix*vertexNormal);
    gl_Position = cameraProjectionMatrix*pos;
}
