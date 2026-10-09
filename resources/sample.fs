#version 460 core

uniform float mycolor;
uniform sampler2D myTexture;

in vec3 color;
in vec2 TexCoord;

out vec4 FragColor;

void main(){
    FragColor = texture(myTexture,TexCoord)*vec4(color.xy, mycolor, 1.0f);
}