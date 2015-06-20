varying vec3 fPosition;
varying vec2 fTexCoord;
varying vec3 fNormal;
uniform vec3 lightPosition;

vec2 blinnPhongDir(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks, float shininess)
{
  vec3 s = normalize(lightDir);
  vec3 v = normalize(-fPosition);
  vec3 n = normalize(fNormal);
  vec3 h = normalize(v+s);
  float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
  float spec =  Ks * pow(max(dot(n,h), dot(n,h)), shininess);
  return vec2(diffuse, spec);
}

void main()
{
    vec2 diffspec = blinnPhongDir(lightPosition,1.0,0.0,1.0,1.0,10.0);
    vec3 diffcolor= vec3 (1.0,0.0,0.0);
    vec3 speccolor = vec3(1.0,1.0,1.0);
    gl_FragColor = vec4(diffcolor*diffspec.x + speccolor*diffspec.y, 1.0);
    //gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}
