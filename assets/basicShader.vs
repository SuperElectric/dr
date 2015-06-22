attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
varying vec3 fPosition;
varying vec2 fTexCoord;
varying vec3 fNormal;
uniform mat4 cameraTransformMatrix;
uniform mat3 cameraRotationMatrix; // is equal to the top left 3x3 of transformMatrix
uniform mat4 cameraProjectionMatrix;

void main()
{	
    vec4 pos = cameraTransformMatrix*vec4(vertexPosition,1.0);
    fPosition = pos.xyz;
    fTexCoord = vertexTexCoord;
    fNormal = normalize(cameraRotationMatrix*vertexNormal);
    gl_Position = cameraProjectionMatrix*pos;
}
