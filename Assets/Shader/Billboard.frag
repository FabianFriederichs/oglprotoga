#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D billboard;
void main()
{
vec4 texColor = texture(billboard, TexCoords);
	if(texColor.a < 0.9)
        discard;
    color =texColor;
}