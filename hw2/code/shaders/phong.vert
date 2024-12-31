#version 450 core

layout(location = 0) in vec3 aPos;     // 模型空间顶点位置
layout(location = 1) in vec3 aNormal;  // 模型空间法向量

out vec3 FragPos;  // 世界空间片段位置
out vec3 Normal;   // 世界空间法向量

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  FragPos = vec3(model * vec4(aPos, 1.0));
  Normal = mat3(transpose(inverse(model))) * aNormal;
  gl_Position = projection * view * vec4(FragPos, 1.0);
}