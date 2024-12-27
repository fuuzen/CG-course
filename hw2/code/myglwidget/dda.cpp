#include "myglwidget.h"

void MyGLWidget::dda(FragmentAttr& start, FragmentAttr& end) {
	float x_start = start.x, y_start = start.y;
    float x_end = end.x, y_end = end.y;

    float dx = x_end - x_start;
    float dy = y_end - y_start;

    int step_count = static_cast<int>(std::max(abs(dx), abs(dy)));

    float x_step = dx / step_count;
    float y_step = dy / step_count;

    float z_start = start.z, z_end = end.z;
    vec3 color_start = start.color, colorEnd = end.color;

    float z_step = (z_end - z_start) / step_count;
    vec3 color_step = (colorEnd - color_start) / static_cast<float>(step_count);

    float x = x_start;
    float y = y_start;
    float z = z_start;
    vec3 color = color_start;

    for (int step = 0; step <= step_count; step++) {
			int pixel_x = static_cast<int>(round(x));
			int pixel_y = static_cast<int>(round(y));

			if (pixel_x >= 0 && pixel_x < WindowSizeW && pixel_y >= 0 && pixel_y < WindowSizeH) {
				int index = pixel_y * WindowSizeW + pixel_x;
				if (temp_z_buffer[index] > z) {
					temp_z_buffer[index] = z;
					temp_render_buffer[index] = color;
				}
			}

			x += x_step;
			y += y_step;
			z += z_step;
			color += color_step;
    }
}