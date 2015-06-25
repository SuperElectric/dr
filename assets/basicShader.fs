varying vec3 fPosition;
varying vec2 fTexCoord;
varying vec3 fNormal;
uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 materialDiffuseColour;
uniform vec3 materialSpecularColour;
uniform float materialSpecularHardness;

vec3 blinnPhongDir(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks,
                   float shininess, vec3 diffColour, vec3 specColour)
{
  vec3 s = normalize(lightDir);
  vec3 v = normalize(-fPosition);
  vec3 n = normalize(fNormal);
  vec3 h = normalize(v+s);
  float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
  float spec =  Ks * pow(max(dot(n,h), dot(n,h)), shininess);
  return diffuse*diffColour + spec*specColour;
}

void main()
{
    vec3 colour = blinnPhongDir(lightPosition,lightColour[0],0.0, 1.0, 1.0,
                                materialSpecularHardness, materialDiffuseColour,
                                materialSpecularColour);
    gl_FragColor = vec4(colour,1.0);
}
