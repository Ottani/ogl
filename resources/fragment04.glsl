#version 330 core
out vec4 FragColor;
  
in vec3 vertTexCoord;

uniform sampler2DArray textureSampler;

void main()
{
	FragColor = texture(textureSampler, vertTexCoord);
}
