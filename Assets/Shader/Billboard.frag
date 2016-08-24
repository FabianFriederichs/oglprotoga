#version 330 core
in vec2 TexCoords;
layout(location = 0) out vec4 color;
uniform sampler2D billboard;
uniform vec3 norm;
void main()

{
vec4 texColor = texture(billboard, TexCoords);
	if(texColor.a < 0.9)
        discard;
    color =texColor;
}