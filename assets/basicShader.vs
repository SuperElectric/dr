attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 fPosition;
varying vec2 fTexCoord;
varying vec3 fNormal;
uniform mat4 transformMatrix;
uniform mat3 rotationMatrix; // is equal to the top left 3x3 of transformMatrix
uniform mat4 cameraMatrix;

void main()
{	
    vec4 pos = transformMatrix*vec4(position,1.0);
    fPosition = pos.xyz;
    fTexCoord = texCoord;
    fNormal = normalize(rotationMatrix*normal);
    gl_Position = cameraMatrix*pos;
}
