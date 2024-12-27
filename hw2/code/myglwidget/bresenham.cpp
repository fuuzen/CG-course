#include "myglwidget.h"

void MyGLWidget::bresenham(FragmentAttr& start, FragmentAttr& end) {
	// 根据起点、终点，计算当前边在画布上的像素
	//（可以只考虑都在画布中。加分思考：在画布外怎么处理）
	uint64_t start_pos_flag = 0, end_pos_flag = 0;
	if(start.x < 0) start_pos_flag |= 1;
	if(start.y >= WindowSizeH) start_pos_flag |= 2;
	if(start.x >= WindowSizeW) start_pos_flag |= 4;
	if(start.y < 0) start_pos_flag |= 8;
	if(end.x < 0) end_pos_flag |= 1;
	if(end.y >= WindowSizeH) end_pos_flag |= 2;
	if(end.x >= WindowSizeW) end_pos_flag |= 4;
	if(end.y < 0) end_pos_flag |= 8;
	FragmentAttr a, b;
	if((start_pos_flag & end_pos_flag) != 0) return;
	else if ((start_pos_flag | end_pos_flag) == 1 + 4) {
		a = getLinearInterpolationByX(start, end, 0);
		b = getLinearInterpolationByX(start, end, WindowSizeW);
	} else if ((start_pos_flag | end_pos_flag) == 2 + 8) {
		a = getLinearInterpolationByY(start, end, 0);
		b = getLinearInterpolationByY(start, end, WindowSizeH);
	} else if ((start_pos_flag | end_pos_flag) == 1 + 2) {
		a = getLinearInterpolationByX(start, end, 0);
		b = getLinearInterpolationByY(start, end, WindowSizeH);
		if(a.y > WindowSizeH || b.x < 0) return;
	} else if ((start_pos_flag | end_pos_flag) == 2 + 4) {
		a = getLinearInterpolationByX(start, end, WindowSizeW);
		b = getLinearInterpolationByY(start, end, WindowSizeH);
		if(a.y > WindowSizeH || b.x > WindowSizeW) return;
	} else if ((start_pos_flag | end_pos_flag) == 4 + 8) {
		a = getLinearInterpolationByX(start, end, WindowSizeW);
		b = getLinearInterpolationByY(start, end, 0);
		if(a.y < 0 || b.x > WindowSizeW) return;
	} else if ((start_pos_flag | end_pos_flag) == 8 + 1) {
		a = getLinearInterpolationByX(start, end, 0);
		b = getLinearInterpolationByY(start, end, 0);
		if(a.y < 0 || b.x < 0) return;
	} else {
		auto f = [&](FragmentAttr a, uint64_t flag){
			if(flag & 1) return getLinearInterpolationByX(start, end, 0);
			if(flag & 2) return getLinearInterpolationByY(start, end, WindowSizeH);
			if(flag & 4) return getLinearInterpolationByX(start, end, WindowSizeW);
			if(flag & 8) return getLinearInterpolationByY(start, end, 0);
			else return a;
		};
		a = f(start, start_pos_flag);
		b = f(end, end_pos_flag);
	}
	int x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int x_step = x1 < x2 ? 1 : -1;
	int y_step = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;

	float dz = end.z - start.z;
	// z 在 x,y 轴方向上的分量的步长
	float z_step_x = dz / dx;
	float z_step_y = dz / dy;
	float z = static_cast<float>(start.z);
	
	while (x1 != x2 || y1 != y2) {
		temp_render_buffer[y1 * WindowSizeW + x1] = vec3(0.0, 1.0, 0.0);
		temp_z_buffer[y1 * WindowSizeW + x1] = z;
		int err_ = err;
		if (err_ > -dx) { err -= dy; x1 += x_step; }
		if (err_ < dy) { err += dx; y1 += y_step; }
		z += dx > dy ? z_step_x : z_step_y;
	}
}


// 画更粗的线，但是效果还不是很好
void MyGLWidget::bresenhamThicker(FragmentAttr a, FragmentAttr b, int thickness) {
	bresenham(a, b);
	int dy = a.y - b.y;
	int dx = a.x - b.x;
	int x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y;
	/* 
	四个象限加粗方向不同 
	*/  
	int dir1[4][2][2]={
		{
			{-1,-1},
			{1,1}
		},
		{
			{-1,1},
			{1,-1}
		},
		{
			{1,1},
			{-1,-1}
		},
		{
			{-1,1},
			{1,-1}
		}
	};   
	if(dx>0 && dy<0) {  
		for(int i=1; i<=(thickness+1)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[0][0][0]+(thickness+1)/2;
			a_.y=y1+i*dir1[0][0][1]+(thickness+1)/2;
			b_.x=x2+i*dir1[0][0][0]+(thickness+1)/2;
			b_.y=y2+i*dir1[0][0][1]+(thickness+1)/2;
			bresenham(a_, b_ );
		}  
		for(int i=1; i<=(thickness)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[0][1][0]+(thickness)/2;
			a_.y=y1+i*dir1[0][1][1]+(thickness)/2;
			b_.x=x2+i*dir1[0][1][0]+(thickness)/2;
			b_.y=y2+i*dir1[0][1][1]+(thickness)/2;
			bresenham(a_, b_ );
		}  
	} else if(dx<0 && dy<0){  
		for(int i=1; i<=(thickness+1)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[1][0][0]+(thickness+1)/2;
			a_.y=y1+i*dir1[1][0][1]+(thickness+1)/2;
			b_.x=x2+i*dir1[1][0][0]+(thickness+1)/2;
			b_.y=y2+i*dir1[1][0][1]+(thickness+1)/2;
			bresenham(a_, b_ );
		}  
		for(int i=1; i<=(thickness)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[1][1][0]+(thickness)/2;
			a_.y=y1+i*dir1[1][1][1]+(thickness)/2;
			b_.x=x2+i*dir1[1][1][0]+(thickness)/2;
			b_.y=y2+i*dir1[1][1][1]+(thickness)/2;
			bresenham(a_, b_ );
		}  
	}  
	else if(dx<0&&dy>0){  
		for(int i=1; i<=(thickness+1)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[2][0][0]+(thickness+1)/2;
			a_.y=y1+i*dir1[2][0][1]+(thickness+1)/2;
			b_.x=x2+i*dir1[2][0][0]+(thickness+1)/2;
			b_.y=y2+i*dir1[2][0][1]+(thickness+1)/2;
			bresenham(a_, b_ );
		}  
		for(int i=1; i<=(thickness)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[2][1][0]+(thickness)/2;
			a_.y=y1+i*dir1[2][1][1]+(thickness)/2;
			b_.x=x2+i*dir1[2][1][0]+(thickness)/2;
			b_.y=y2+i*dir1[2][1][1]+(thickness)/2;
			bresenham(a_, b_ );
		}  
	}  
	else if(dx>0&&dy>0){  
		for(int i=1; i<=(thickness+1)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[3][0][0];
			a_.y=y1+i*dir1[3][0][1];
			b_.x=x2+i*dir1[3][0][0];
			b_.y=y2+i*dir1[3][0][1];
			bresenham(a_, b_ );
		}  
		for(int i=1; i<=(thickness)/2; i++) {
			FragmentAttr a_ = a, b_ = b;
			a_.x=x1+i*dir1[3][1][0];
			a_.y=y1+i*dir1[3][1][1];
			b_.x=x2+i*dir1[3][1][0];
			b_.y=y2+i*dir1[3][1][1];
			bresenham(a_, b_ );
		}  
	}  
}