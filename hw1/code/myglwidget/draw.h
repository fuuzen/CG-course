// #include <QtGui>
// #include <QOpenGLWidget>
// #include <QOpenGLFunctions>
#include "QtGui/QtGui"
#include "QtOpenGLWidgets/QOpenGLWidget"
#include "QtGui/QOpenGLFunctions"

#define CHAR_THICKNESS 10
#define CHAR_HEIGHT 90
#define CHAR_WIDTH 70


void draw2dTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float z);

// (x1,y1)左下角; (x2,y2)右上角
void draw2dRectangle(float x1, float y1, float x2, float y2, float z);

// xOy 平面上的一条线为基础, 宽为 CHAR_THICKNESS 的边框
void drawSideRectangle(float x1, float y1, float x2, float y2, float z);

void draw_L_2d(float dx, float dy, float dz);
void draw_S_2d(float dx, float dy, float dz);
void draw_Y_2d(float dx, float dy, float dz);

void draw_L_3d(float dx, float dy, float dz);
void draw_S_3d(float dx, float dy, float dz);
void draw_Y_3d(float dx, float dy, float dz);

// Random color generator
void randomColor();