#shader vertex
#version 330 core

out vec2 fragCoord;
void main(void)
{
	vec4 vertices[4] = vec4[4]
	(
		vec4(-1.0, -1.0, 0.0, 1.0),
		vec4(1.0, -1.0, 0.0, 1.0),
		vec4(-1.0, 1.0, 0.0, 1.0),
		vec4(1.0, 1.0, 0.0, 1.0)
	);

	vec2 texCoord[4] = vec2[4]
	(
		vec2(0.0, 0.0),
		vec2(1.0, 0.0),
		vec2(0.0, 1.0),
		vec2(1.0, 1.0)
	);

	fragCoord = texCoord[gl_VertexID];
	gl_Position = vertices[gl_VertexID];
}

#shader fragment
#version 330 core

in vec2 fragCoord;
out vec4 color;

uniform sampler2D Texture;

void main() {
	color = texture(Texture, fragCoord);
}