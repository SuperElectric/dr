#version 430

in vec3 fPosition;
in vec2 fTexCoord;
in vec3 fNormal;

uniform bool firstPass;
uniform vec3 sphericalHarmonicCoefficients[9];

uniform sampler2D surfaceColourTexture;
uniform sampler2D shAOmap[9];

layout (location=0) out vec3 output_image;
layout (location=1) out vec3 output_derivatives[2];


