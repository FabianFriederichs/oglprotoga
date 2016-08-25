#version 330 core
uniform sampler2D alph;
in vec2 uvs;
layout(location=0) out vec4 color;
void main()
{
if(texture(alph, uvs).a<0.9)
		discard;
//color = vec4(0.5,0.5,0.25,1.0);
}