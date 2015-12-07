#version 430

in vec3 fPosition;
in vec2 fTexCoord;
in vec3 fNormal;
in vec3 lightPositionWorldSpace;
uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 materialDiffuseColour;
uniform vec3 materialSpecularColour;
uniform float materialSpecularHardness;

uniform sampler2D sampler_0;
uniform sampler2D sampler_1;
uniform sampler2D sampler_2;
uniform sampler2D sampler_3;

layout (location=0) out vec4 output_image;
layout (location=1) out vec4 output_d_light_r;
layout (location=2) out vec4 output_d_light_direction_x;
layout (location=3) out vec4 visualisation; // will convert to greyscale and then display red for positive and blue for negative
//layout (location=3) out vec4 output_d_light_direction_y;
//layout (location=4) out vec4 output_d_light_direction_z;

vec3 blinnPhongDir(vec3 lightDir, vec3 lightColour, float Ka, float Kd, float Ks,
                   float shininess, vec3 diffColour, vec3 specColour)
{
  vec3 s = normalize(lightDir);
  vec3 v = normalize(-fPosition);
  vec3 n = normalize(fNormal);
  vec3 h = normalize(v+s);
  float diffuse = Kd * max(0.0, dot(n, s));
  float spec =  Ks * pow(max(0.0, dot(n,h)), shininess);
  return Ka*diffColour + lightColour*(diffuse*diffColour + spec*specColour);
}

mat3 blinnPhongDirDerivativeWrtLightDir (vec3 lightDir, vec3 lightColour, float Ka,
    float Kd, float Ks, float shininess, vec3 diffColour, vec3 specColour)
{
    
    vec3 l = lightDir;
    vec3 l_unit = normalize(lightDir);
    float l_magnitude = length(l);
    vec3 v = normalize(-fPosition);
    vec3 n = normalize(fNormal);
    vec3 h = v+l_unit;
    vec3 h_unit = normalize(h);
    float h_magnitude = length(h);
    
    vec3 diffuseDerivative = vec3(0.0,0.0,0.0);
    vec3 specularDerivative = vec3(0.0,0.0,0.0);
    if (dot(n,l_unit) > 0.0){
        diffuseDerivative = (n - l_unit*dot(n,l_unit))/l_magnitude;
    }
    if (dot(n,h) > 0.0){
        specularDerivative = (diffuseDerivative
                              - h_unit*dot(n,h_unit)
                                      *(vec3(1.0,1.0,1.0) - l_unit*l_unit)
                                      / l_magnitude
                             ) / h_magnitude;
    specularDerivative = specularDerivative * shininess
                            * pow(max(0.0, dot(n,h_unit)), shininess - 1.0);
    }

    vec3 finalDiffColour = Kd*lightColour*diffColour;
    vec3 finalSpecColour = Ks*lightColour*specColour;
    
    return mat3(finalDiffColour*diffuseDerivative.x + finalSpecColour*specularDerivative.x,
                finalDiffColour*diffuseDerivative.y + finalSpecColour*specularDerivative.y,
                finalDiffColour*diffuseDerivative.z + finalSpecColour*specularDerivative.z);
}

vec3 calculateDiffColour(){
    vec2 hld = normalize(lightPositionWorldSpace.zx);
    vec3 colourA, colourB;
    float coeffA, coeffB;
    if (hld.x > 0 && hld.y > 0){
        colourA = vec3(texture2D(sampler_0, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        colourB = vec3(texture2D(sampler_1, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        coeffA = abs(hld.x)/(abs(hld.x)+abs(hld.y));
        coeffB = abs(hld.y)/(abs(hld.x)+abs(hld.y));
    }
    else if (hld.x > 0 && hld.y < 0){
        colourA = vec3(texture2D(sampler_0, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        colourB = vec3(texture2D(sampler_3, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        coeffA = abs(hld.x)/(abs(hld.x)+abs(hld.y));
        coeffB = abs(hld.y)/(abs(hld.x)+abs(hld.y));
    }
    else if (hld.x < 0 && hld.y > 0){
        colourA = vec3(texture2D(sampler_2, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        colourB = vec3(texture2D(sampler_1, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        coeffA = abs(hld.x)/(abs(hld.x)+abs(hld.y));
        coeffB = abs(hld.y)/(abs(hld.x)+abs(hld.y));
    }
    else if (hld.x < 0 && hld.y < 0){
        colourA = vec3(texture2D(sampler_2, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        colourB = vec3(texture2D(sampler_3, vec2(fTexCoord.x,1.0-fTexCoord.y)));
        coeffA = abs(hld.x)/(abs(hld.x)+abs(hld.y));
        coeffB = abs(hld.y)/(abs(hld.x)+abs(hld.y));
    }
    return coeffA*colourA + coeffB*colourB;
}

void main()
{
    vec3 diffColour = calculateDiffColour();
    vec3 colour;
    //if (fPosition.y > 0.6){
    colour = blinnPhongDir(lightPosition,lightColour,1.0, 0.0, 0.5,
                                materialSpecularHardness, diffColour,
                                materialSpecularColour);
    //}
    //else {
    //diffColour = vec3(0.5,0.5,0.5);
    //colour = blinnPhongDir(lightPosition,lightColour,0.5, 0.5, 0.5,
    //                            materialSpecularHardness, diffColour,
    //                            materialSpecularColour);
    //}
    //gl_FragColor = vec4(colour,1.0);



    output_image = vec4(colour,1.0);
    output_d_light_r = 
    vec4(blinnPhongDir(lightPosition, vec3(1.0,0.0,0.0), 0.0, 1.0, 0.5,
                       materialSpecularHardness, diffColour,
                       materialSpecularColour), 1.0);
    mat3 derivativeWrtLightDir = blinnPhongDirDerivativeWrtLightDir(
                                lightPosition,lightColour,1.0, 0.0, 0.5,
                                materialSpecularHardness, diffColour,
                                materialSpecularColour);
    output_d_light_direction_x = vec4(derivativeWrtLightDir*vec3(1.0,0.0,0.0),1.0);
    
    // temporary
    vec3 v = vec3(output_d_light_direction_x);
    float rgbAverage = 2.0*(v[0] + v[1] + v[2])/3.0;
    visualisation = vec4(0.0,0.0,0.0,1.0);
    if (rgbAverage > 0.0){
        visualisation[0] = rgbAverage;
    }
    else{
        visualisation[2] = -rgbAverage;
    }
}


