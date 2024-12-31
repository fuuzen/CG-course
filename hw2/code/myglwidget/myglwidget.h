#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#endif

#include <chrono>
#include "../utils.h"
#include "QtGui/QtGui"
#include "QtOpenGLWidgets/QOpenGLWidget"
#include "QtGui/QOpenGLFunctions"
#include "QtOpenGL/QOpenGLFunctions_4_5_Core"  // newest version by the end of 2024

#define MAX_Z_BUFFER 99999999.0f
#define MIN_FLOAT 1e-10f

using namespace glm;

struct Handles {
  GLuint program;
  GLuint VBO;
  GLuint VAO;
  GLsizei verticesCount;
};

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
  Q_OBJECT

public:
  MyGLWidget(QWidget *parent = nullptr);
  ~MyGLWidget();

protected:  // Implement QOpenGLWidget
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void keyPressEvent(QKeyEvent* e) override;

  void draw();

private:
  // switch
  int scene = 0;          // load up to 5 objs
  int degree = 0;
  int camDistance = 70; 

  Model objModel;
  Handles objHandles;

  Handles initShader(const char *vertexPath, const char *fragmentPath);

  void loadBuffer(Model objModel, Handles & handles);

  void drawObj(const Handles & handlers);

  // timing
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
};

#endif // MYGLWIDGET_H
