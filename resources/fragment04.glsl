#version 330 core
out vec4 FragColor;
  
in vec3 vertColor;
in vec2 vertTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1, vertTexCoord), texture(texture2, vertTexCoord), 0.2);
}
