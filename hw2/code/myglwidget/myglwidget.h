#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#endif
#include "QtGui/QtGui"
#include "QtOpenGLWidgets/QOpenGLWidget"
#include "QtGui/QOpenGLFunctions"
#include "../utils.h"
#include <chrono>

#define MAX_Z_BUFFER 99999999.0f
#define MIN_FLOAT 1e-10f

using namespace glm;

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void keyPressEvent(QKeyEvent* e) override;

private:
    void scene_0();
    void scene_1();

    void drawTriangle(Triangle triangle);
    // 实现三角形的光栅化算法
    void dda(FragmentAttr& start, FragmentAttr& end);
    void bresenham(FragmentAttr& start, FragmentAttr& end);
    void bresenhamThicker(FragmentAttr a, FragmentAttr b, int thickness);
    int edge_walking(TransformedTriangle const & tri);

    // 实现光照、着色
    void gouraud(int x, int y, TransformedTriangle const & tri);
    void phong(int x, int y, TransformedTriangle const & tri);
    void blinn_phong(int x, int y, TransformedTriangle const & tri);

    void clearBuffer(vec3* now_render_buffer);
    void clearBuffer(int* now_buffer);
    void clearZBuffer(float* now_buffer);
    void resizeBuffer(int newW, int newH);

    // switch
    int obj = 0;           // load up to 5 objs
    int line_mode = 0;     // 0:dda, 1:bresneham, 2:none
    int shading_mode = 0;  // 0:gouraud, 1:phong, 2:blinn_phong, 3:pure_color

    int WindowSizeH = 0;
    int WindowSizeW = 0;
    int scene_id = 0;
    int degree = 0;

    // buffers
    vec3* render_buffer;
    vec3* temp_render_buffer;
    float* temp_z_buffer;
    float* z_buffer;
    vec2 offset;

    Model objModel;

    vec3 camPosition;
    vec3 camLookAt;
    vec3 camUp;
    mat4 projMatrix;
    vec3 lightPosition;

    float ambientStrength = 0.1f;
    float specularStrength = 2.0f;
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(0.6f, 0.6f, 0.3f);

    // timing
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
};

#endif // MYGLWIDGET_H
