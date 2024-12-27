#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>
#include <vector>

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
}

void MyGLWidget::resizeBuffer(int newW, int newH) {
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
	WindowSizeW = newW;
	WindowSizeH = newH;
	render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_z_buffer = new float[WindowSizeH*WindowSizeW];
	z_buffer = new float[WindowSizeH*WindowSizeW];
}

void MyGLWidget::initializeGL()
{
	WindowSizeW = width();
	WindowSizeH = height();
	glViewport(0, 0, WindowSizeW, WindowSizeH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	// 对定义的数组初始化
	render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH*WindowSizeW];
	temp_z_buffer = new float[WindowSizeH*WindowSizeW];
	z_buffer = new float[WindowSizeH*WindowSizeW];
	for (int i = 0; i < WindowSizeH*WindowSizeW; i++) {
		render_buffer[i] = vec3(0, 0, 0);
		temp_render_buffer[i] = vec3(0, 0, 0);
		temp_z_buffer[i] = MAX_Z_BUFFER;			
		z_buffer[i] = MAX_Z_BUFFER;
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	
	switch (e->key()) {
		case Qt::Key_0: scene_id = 0; update(); break;
		case Qt::Key_1: scene_id = 1; update(); break;
		case Qt::Key_6: obj = (obj + 1) % 5; update(); break;
		case Qt::Key_7: line_mode = (line_mode + 1) % 2; update(); break;
		case Qt::Key_8: shading_mode = (shading_mode + 1) % 4; update(); break;
		case Qt::Key_9: degree += 5; update(); break;
	}
}

void MyGLWidget::paintGL()
{
	start_time = std::chrono::high_resolution_clock::now();

	switch (scene_id) {
		case 0:scene_0(); break;
		case 1:scene_1(); break;
	}
	
	end_time = std::chrono::high_resolution_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
  std::cout << ms.count() / 1000000.0 << " ms" << std::endl;
}
void MyGLWidget::clearBuffer(vec3* now_buffer) {
	for (int i = 0; i < WindowSizeH*WindowSizeW; i++) {
		now_buffer[i] = vec3(0,0,0);
	}
}

void MyGLWidget::clearBuffer(int* now_buffer) {
	memset(now_buffer, 0, WindowSizeW * WindowSizeH * sizeof(int));
}


void MyGLWidget::clearZBuffer(float* now_buffer) {
	std::fill(now_buffer,now_buffer+WindowSizeW * WindowSizeH, MAX_Z_BUFFER);
}


// 窗口大小变动后，需要重新生成render_buffer等数组
void MyGLWidget::resizeGL(int w, int h)
{
	resizeBuffer(w, h);
	offset = vec2(WindowSizeW / 2, WindowSizeH / 2);
	clearBuffer(render_buffer);
}

void MyGLWidget::scene_0()
{
	// 选择要加载的model
	objModel.loadModel("./objs/singleTriangle.obj");

	// 自主设置变换矩阵
	camPosition = vec3(
		100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y,
		100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x,
		10+ objModel.centralPoint.z
	);
	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);

	// 单一点光源，可以改为数组实现多光源
	lightPosition = objModel.centralPoint + vec3(0,100,100);
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer,WindowSizeH,WindowSizeW);
}


void MyGLWidget::scene_1()
{
	// 选择要加载的model
	switch(obj){
	  case 0: objModel.loadModel("./objs/teapot_600.obj"); break;
	  case 1: objModel.loadModel("./objs/teapot_8000.obj"); break;
	  case 2: objModel.loadModel("./objs/rock.obj"); break;
	  case 3: objModel.loadModel("./objs/cube.obj"); break;
	  default : objModel.loadModel("./objs/singleTriangle.obj");
	}
	
	// 自主设置变换矩阵
	camPosition = vec3(
		100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.x,
		100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.y,
		10 + objModel.centralPoint.z
	);
	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(25.0f), 1.0f, 0.1f, 2000.0f);

	// 单一点光源，可以改为数组实现多光源
	lightPosition = objModel.centralPoint + vec3(0,100,100);
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer, WindowSizeH, WindowSizeW);
}

void MyGLWidget::drawTriangle(Triangle triangle) {
	// 三维顶点映射到二维平面
	vec3* vertices = triangle.triangleVertices;
	vec3* normals = triangle.triangleNormals;
	TransformedTriangle tri;
	clearBuffer(this->temp_render_buffer);
	clearZBuffer(this->temp_z_buffer);
	mat4 viewMatrix = glm::lookAt(camPosition, camLookAt, camUp);

	for (int i = 0; i < 3; ++i) {
		vec4 ver_mv = viewMatrix * vec4(vertices[i], 1.0f);
		float nowz = glm::length(camPosition - vec3(ver_mv));
		vec4 ver_proj = projMatrix * ver_mv;
		tri.lines[i].x = ver_proj.x + offset.x;
		tri.lines[i].y = ver_proj.y + offset.y;
		tri.lines[i].z = nowz;
		tri.lines[i].pos_mv = ver_mv;  
		mat3 normalMatrix = mat3(viewMatrix);
		vec3 normal_mv = normalMatrix * normals[i];
		tri.lines[i].normal = normal_mv;
	}

	// 将当前三角形渲染在temp_buffer中
		
	// HomeWork: 1、绘制三角形三边
	if(line_mode == 0) {
		dda(tri.lines[0], tri.lines[1]);
		dda(tri.lines[1], tri.lines[2]);
		dda(tri.lines[2], tri.lines[0]);
	} else if(line_mode == 1) {
		bresenham(tri.lines[0], tri.lines[1]);
		bresenham(tri.lines[1], tri.lines[2]);
		bresenham(tri.lines[2], tri.lines[0]);
		// bresenhamThicker(tri.lines[0], tri.lines[1], 10);
		// bresenhamThicker(tri.lines[1], tri.lines[2], 10);
		// bresenhamThicker(tri.lines[2], tri.lines[0], 10);
	}

	// HomeWork: 2: 用edge-walking填充三角形内部到temp_buffer中
	int firstChangeLine = edge_walking(tri);

	// 合并temp_buffer 到 render_buffer, 深度测试
	// 从firstChangeLine开始遍历，可以稍快
	for(int h = firstChangeLine; h < WindowSizeH ; h++){
		auto render_row = &render_buffer[h * WindowSizeW];
		auto temp_render_row = &temp_render_buffer[h * WindowSizeW];
		auto z_buffer_row = &z_buffer[h*WindowSizeW];
		auto temp_z_buffer_row = &temp_z_buffer[h*WindowSizeW];
		for (int i = 0 ; i < WindowSizeW ; i++){
			if (z_buffer_row[i] < temp_z_buffer_row[i])
				continue;
			else
			{
				z_buffer_row[i] = temp_z_buffer_row[i];
				render_row[i] = temp_render_row[i];
			}
		}
	}
}

