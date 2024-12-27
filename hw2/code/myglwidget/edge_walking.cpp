#include "myglwidget.h"

int MyGLWidget::edge_walking(TransformedTriangle const & tri) {
	// int firstChangeLine = 0;
	// return firstChangeLine;
	
	std::vector<int> edge_starts(WindowSizeH, -1);
    std::vector<int> edge_ends(WindowSizeH, -1);
    int firstChangeLine = -1;

    // 构建边缘表
    for (int y = 0; y < WindowSizeH; ++y) {
        for (int x = 0; x < WindowSizeW; ++x) {
            int index = y * WindowSizeW + x;
            if (temp_render_buffer[index] != vec3(0.0f, 0.0f, 0.0f)) {
                if (edge_starts[y] == -1) {
                    edge_starts[y] = x; // 记录第一个边缘
                }
                edge_ends[y] = x; // 更新最后一个边缘
            }
        }

        if (firstChangeLine == -1 && edge_starts[y] != -1 && edge_ends[y] != -1 && edge_starts[y] != edge_ends[y]) {
            firstChangeLine = y;
        }
    }

    // 填充三角形内部区域
    for (int y = 0; y < WindowSizeH; ++y) {
        if (edge_starts[y] != -1 && edge_ends[y] != -1) {
            int x_start = edge_starts[y];
            int x_end = edge_ends[y];
            float z_start = temp_z_buffer[y * WindowSizeW + x_start];
            float z_end = temp_z_buffer[y * WindowSizeW + x_end];

            for (int x = x_start + 1; x < x_end; ++x) {
                // 选择光照计算方法
                switch (shading_mode) {
                    case 0: gouraud(x, y, tri); break;
                    case 1: phong(x, y, tri); break;
                    case 2: blinn_phong(x, y, tri); break;
                    default:
                        temp_render_buffer[y * WindowSizeW + x] = vec3(1.0f, 1.0f, 1.0f);
                }

                // 插值计算深度值
                float t = static_cast<float>(x - x_start) / (x_end - x_start);
                temp_z_buffer[y * WindowSizeW + x] = (1 - t) * z_start + t * z_end;
            }
        }
    }
    
    return firstChangeLine != -1 ? firstChangeLine : 0;
}