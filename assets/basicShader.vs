#version 430

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
out vec3 fPosition;
out vec2 fTexCoord;
out vec3 fNormal;
out vec3 lightPositionWorldSpace;
uniform mat4 cameraTransformMatrix;
uniform mat4 cameraProjectionMatrix;
uniform vec3 lightPosition;

void main()
{	
    vec4 pos = cameraTransformMatrix*vec4(vertexPosition,1.0);
    mat3 cameraRotationMatrix = mat3(cameraTransformMatrix);
    fPosition = pos.xyz;
    fTexCoord = vertexTexCoord;
    fNormal = normalize(cameraRotationMatrix*vertexNormal);
    gl_Position = cameraProjectionMatrix*pos;
    lightPositionWorldSpace = inverse(cameraRotationMatrix)*lightPosition;
}
