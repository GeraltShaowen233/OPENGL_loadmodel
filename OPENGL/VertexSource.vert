#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 7) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//out vec4 vertexColor;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   Normal = mat3(model)*aNormal;
   FragPos = (model*vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;
   //vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);
   TexCoord = aTexCoord;
}