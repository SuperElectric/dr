#version 430

varying vec3 fPosition;
varying vec2 fTexCoord;
varying vec3 fNormal;
uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 materialDiffuseColour;
uniform vec3 materialSpecularColour;
uniform float materialSpecularHardness;

layout (location=0) out vec4 output_image;
layout (location=1) out vec4 output_d_light_r;

vec3 blinnPhongDir(vec3 lightDir, vec3 lightColour, float Ka, float Kd, float Ks,
                   float shininess, vec3 diffColour, vec3 specColour)
{
  vec3 s = normalize(lightDir);
  vec3 v = normalize(-fPosition);
  vec3 n = normalize(fNormal);
  vec3 h = normalize(v+s);
  float diffuse = Kd * max(0.0, dot(n, s));
  float spec =  Ks * pow(max(dot(n,h), dot(n,h)), shininess);
  return Ka*diffColour + lightColour*(diffuse*diffColour + spec*specColour);
}

void main()
{
    vec3 colour = blinnPhongDir(lightPosition,lightColour,0.3, 1.0, 1.0,
                                materialSpecularHardness, materialDiffuseColour,
                                materialSpecularColour);
    //gl_FragColor = vec4(colour,1.0);
    output_image = vec4(colour,1.0);
    output_d_light_r =
    vec4(blinnPhongDir(lightPosition, vec3(lightColour.x,0.0,0.0), 0.0, 1.0,
                       1.0, materialSpecularHardness, materialDiffuseColour,
                       materialSpecularColour), 1.0);
    
}
