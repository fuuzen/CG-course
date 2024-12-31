#version 450 core

in vec3 FragPos;      // 世界空间片段位置
in vec3 Normal;       // 世界空间法向量
out vec4 FragColor;   // 输出颜色

uniform vec3 lightPos;    // 光源位置
uniform vec3 camPos;      // 相机位置
uniform vec3 lightColor;  // 光源颜色
uniform vec3 objectColor; // 物体颜色
uniform vec3 ambient;     // 环境光

void main()
{
  // 漫反射
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  // 镜面反射
  float specularStrength = 0.5;
  vec3 viewDir = normalize(camPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  // 最终颜色
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}