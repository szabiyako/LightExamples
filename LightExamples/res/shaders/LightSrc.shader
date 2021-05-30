#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texturepos;
layout(location = 2) in vec3 normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;
out vec3 col;

void main()
{
	vec4 worldCoords = u_Model * vec4(position.xyz, 1.0);
	gl_Position = u_ViewProj * worldCoords;

	if (normal.y > 0.5f)
		col = vec3(1.0, 1.0, 1.0);
	else if (normal.y < -0.5f)
		col = vec3(0.9, 0.9, 0.9);
	else
	{
		if (normal.x > 0.5f || normal.x < -0.5f)
			col = vec3(0.97, 0.97, 0.97);
		else
			col = vec3(0.93, 0.93, 0.93);
	}
};

#shader fragment
#version 330 core

in vec3 col;
out vec4 out_Color; // was gl_Color;

void main()
{
	out_Color = vec4(col, 1.0f);
};