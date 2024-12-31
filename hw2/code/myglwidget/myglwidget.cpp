#include "myglwidget.h"
#include "GL/glew.h"
#include "QtGui/QOpenGLExtraFunctions"
#include "../utils.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

MyGLWidget::~MyGLWidget()
{
  // TODO : perhaps wrap in a class Shader ?
  glDeleteProgram(objHandles.program);
}

void MyGLWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glViewport(0, 0, width(), height());
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  objHandles = initShader("shaders/phong.vert", "shaders/phong.frag");
	objModel.loadModel("./objs/singleTriangle.obj");
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
    case Qt::Key_Q:
      scene = (scene + 1) % 5;
      switch(scene){
        case 0 : objModel.loadModel("./objs/singleTriangle.obj");
        case 1: objModel.loadModel("./objs/teapot_600.obj"); break;
        case 2: objModel.loadModel("./objs/teapot_8000.obj"); break;
        case 3: objModel.loadModel("./objs/rock.obj"); break;
        case 4: objModel.loadModel("./objs/cube.obj"); break;
        default : objModel.loadModel("./objs/singleTriangle.obj");
      }
      break;
    case Qt::Key_E: degree += 1; break;
    case Qt::Key_W: camDistance = camDistance == 0 ? 0 : camDistance - 10; break;
    case Qt::Key_S: camDistance = camDistance == 300 ? 0 : camDistance + 10; break;
  }
  update();
}

void MyGLWidget::paintGL()
{
  start_time = std::chrono::high_resolution_clock::now();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  loadBuffer(objModel, objHandles);
  drawObj(objHandles);
  
  end_time = std::chrono::high_resolution_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
  std::cout << ms.count() / 1000000.0 << " ms" << std::endl;
}

// 窗口大小变动后，调用 glViewport 再更新即可
void MyGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  update();
}
