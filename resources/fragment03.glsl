#version 330 core
out vec4 FragColor;
  
in vec3 vertColor;
in vec2 vertTexCoord;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, vertTexCoord);
}
