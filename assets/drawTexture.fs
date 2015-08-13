#version 150

in float fdA;
out vec4 color;

uniform sampler2D sampler0;

void main(){    
    color = vec4(fdA,0.0,0.0,1.0);
    color = vec4(dot(gl_PointCoord,gl_PointCoord),0.0,0.0,1.0)*1000.0;
    color = texture2D(sampler0, gl_PointCoord);    
}
