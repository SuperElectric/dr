#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec3 fPosition;
varying vec2 fTexCoord;
varying vec3 fNormal;

void main()
{
    gl_Position = vec4(position, 1.0);
    fPosition = position;
    fTexCoord = texCoord;
    fNormal = normalize(normal);
}
