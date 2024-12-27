#include "myglwidget.h"

void MyGLWidget::phong(int x, int y, TransformedTriangle const & tri){
  auto l = tri.lines;

  // 计算重心
	float alpha = (
      (static_cast<float>(y - l[1].y) * (l[2].x - l[1].x) - static_cast<float>(x - l[1].x) * (l[2].y - l[1].y)
    ) / (
      static_cast<float>(l[0].y - l[1].y) * (l[2].x - l[1].x) - static_cast<float>(l[0].x - l[1].x) * (l[2].y - l[1].y)
    )
  );
	float beta = (
    static_cast<float>(y - l[2].y) * (l[0].x - l[2].x) - static_cast<float>(x - l[2].x) * (l[0].y - l[2].y)
  ) / (
    static_cast<float>(l[1].y - l[2].y) * (l[0].x - l[2].x) - static_cast<float>(l[1].x - l[2].x) * (l[0].y - l[2].y)
  );
	float gamma = 1.0f - alpha - beta;

  // 插值法向量和位置
  vec3 interpolatedNormal = glm::normalize(alpha * l[0].normal + beta * l[1].normal + gamma * l[2].normal);
  vec3 interpolatedPosMV = alpha * vec3(l[0].pos_mv) + beta * vec3(l[1].pos_mv) + gamma * vec3(l[2].pos_mv);

  FragmentAttr nowPixelResult(x, y, 0.0f, 0);
  nowPixelResult.normal = interpolatedNormal;
  nowPixelResult.pos_mv = interpolatedPosMV;

  // 光照计算
  vec3 lightDir = glm::normalize(lightPosition - nowPixelResult.pos_mv);
  vec3 viewDir = glm::normalize(camPosition - nowPixelResult.pos_mv);
  vec3 reflectDir = glm::reflect(-lightDir, nowPixelResult.normal);
  
  // 颜色计算
  vec3 ambient = ambientStrength * lightColor;
  float diff = glm::max(glm::dot(nowPixelResult.normal, lightDir), 0.0f);
  vec3 diffuse = diff * lightColor;
  float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 16);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 color = (ambient + diffuse + specular) * objectColor;
  temp_render_buffer[y * WindowSizeW + x] = color;
}