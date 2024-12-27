#include "myglwidget.h"

void MyGLWidget::gouraud(int x, int y, TransformedTriangle const & tri) {
  auto l = tri.lines;
	float alpha = (
    (
      static_cast<float>(y - l[1].y) * (l[2].x - l[1].x) - static_cast<float>(x - l[1].x) * (l[2].y - l[1].y)
    ) / (
      static_cast<float>(l[0].y - l[1].y) * (l[2].x - l[1].x) - static_cast<float>(l[0].x - l[1].x) * (l[2].y - l[1].y)
    )
  );
	float beta  = (
    static_cast<float>(y - l[2].y) * (l[0].x - l[2].x) - static_cast<float>(x - l[2].x) * (l[0].y - l[2].y)
  ) / (
    static_cast<float>(l[1].y - l[2].y) * (l[0].x - l[2].x) - static_cast<float>(l[1].x - l[2].x) * (l[0].y - l[2].y)
  );
	float gamma = 1 - alpha - beta;
	vec3 color = alpha * l[0].color + beta * l[1].color + gamma * l[2].color;
	temp_render_buffer[y * WindowSizeW + x] = color;
}