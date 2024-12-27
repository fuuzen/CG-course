#include "myglwidget.h"

void MyGLWidget::blinn_phong(int x, int y, TransformedTriangle const & tri) {
	auto l = tri.lines;
	float alpha = (
    (
      static_cast<float>(y - l[1].y) * (l[2].x - l[1].x) - static_cast<float>(x - l[1].x) * (l[2].y - l[1].y)
    ) / (
      static_cast<float>(l[0].y - l[1].y) * (l[2].x - l[1].x) - static_cast<float>(l[0].x - l[1].x) * (l[2].y - l[1].y)
    )
  );
	float beta = (
    static_cast<float>(y - l[2].y) * (l[0].x - l[2].x) - static_cast<float>(x - l[2].x) * (l[0].y - l[2].y)
  ) / (
    static_cast<float>(l[1].y - l[2].y) * (l[0].x - l[2].x) - static_cast<float>(l[1].x - l[2].x) * (l[0].y - l[2].y)
  );
	float gamma = 1 - alpha - beta;

	// 插值法计算法线和位置
  vec3 normalInterpolated = alpha * l[0].normal + beta * l[1].normal + gamma * l[2].normal;
  vec3 positionMVInterpolated = alpha * vec3(l[0].pos_mv) + beta * vec3(l[1].pos_mv) + gamma * vec3(l[2].pos_mv);

  FragmentAttr currentPixelResult(x, y, 0.0f, 0);
  currentPixelResult.normal = normalize(normalInterpolated);
  currentPixelResult.pos_mv = positionMVInterpolated;

  vec3 normalizedNormal = normalize(currentPixelResult.normal);

  // 光照计算
  vec3 lightDirection = normalize(lightPosition - currentPixelResult.pos_mv);
  vec3 viewDirection = normalize(camPosition - currentPixelResult.pos_mv);
  vec3 halfVector = normalize(lightDirection + viewDirection);

  vec3 ambientLight = ambientStrength * lightColor;
  float diffuseFactor = max(dot(normalizedNormal, lightDirection), 0.0f);
  vec3 diffuseLight = diffuseFactor * lightColor;

  float specularFactor = pow(max(dot(normalizedNormal, halfVector), 0.0f), 16);
  vec3 specularLight = specularStrength * specularFactor * lightColor;

  vec3 color = (ambientLight + diffuseLight + specularLight) * objectColor;

	temp_render_buffer[y * WindowSizeW + x] = color;
}
