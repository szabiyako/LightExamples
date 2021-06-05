#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 texturePos;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main()
{
	texturePos = vec3(position.x, position.y * -1, position.z);;

	vec4 worldCoords = u_Model * vec4(position, 1.0);
	gl_Position = vec4(u_ViewProj * worldCoords).xyww;
};

#shader fragment
#version 330 core

in vec3 texturePos;
out vec4 FragColor;

uniform samplerCube u_skybox;

void main()
{
	FragColor = texture(u_skybox, texturePos);
};