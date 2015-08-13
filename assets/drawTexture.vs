#version 150

in float dummyAttribute;
out float fdA;

void main(){
    //gl_PointSize = 100.0;
    gl_Position = vec4(0.0,0.0,0.0,1.0);
    fdA = dummyAttribute;
}
