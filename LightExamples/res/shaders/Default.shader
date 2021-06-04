#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

//Base
out vec3 Normal;
out vec3 Pos;
out vec2 UV;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform mat4 u_normalMatrix;
uniform mat4 u_normalModelMatrix;

void main()
{
	Normal = normalize((u_normalMatrix * (u_normalModelMatrix * vec4(normal, 1.f))).xyz);

	Pos = (u_viewMatrix * u_modelMatrix * vec4(position, 1.0)).xyz;
	UV = uv;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(position, 1.0);
};

#shader fragment
#version 330 core

uniform vec3 u_color;

in vec3 Normal;
in vec3 Pos;
in vec2 UV;

out vec4 out_Color; //was gl_Color

void main()
{
	vec4 color = vec4(u_color, 1.f);
	vec3 lightDir = normalize(Pos - vec3(0.0, 0.0, 0.0));
	float diffuse = dot(Normal, -lightDir);
	if (diffuse < 0)
		diffuse = abs(diffuse / 2);
	vec3 viewDir = normalize(Pos);
	vec3 reflectedLightDir = reflect(lightDir, Normal);
	float specular = max(dot(reflectedLightDir, -viewDir), 0.0);

	float specularAmount = 0.05;
	float diffuseAmount = 0.75;
	float ambientAmount = 1.0 - diffuseAmount;

	//float specularAmount = 0.0;
	//float diffuseAmount = 0.5;
	//float ambientAmount = 0.5;

	gl_FragColor = vec4((color * ambientAmount + color * diffuse * diffuseAmount + specular * specularAmount).xyz, 1.f);

	//Texture coords test
	//vec4 resultColor = vec4((color * ambientAmount + color * diffuse * diffuseAmount + specular * specularAmount).xyz, 1.f);
	//gl_FragColor = vec4(resultColor.x, resultColor.y * UV.x, resultColor.z * UV.y, resultColor.w);
};