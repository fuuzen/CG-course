#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    :QOpenGLWidget(parent),
    scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width(), height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    if (scene_id == 0) {
        scene_0();
    }
    else if (scene_id == 1) {
        scene_1();
    }
    else {
        scene_2();
    }
}

void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_0) {
        scene_id = 0;
        update();
    }
    else if (e->key() == Qt::Key_1) {
        scene_id = 1;
        update();
    }
    else if (e->key() == Qt::Key_2) {
        scene_id = 2;
        update();
    }
    else if (e->key() == Qt::Key_W) {
        rotationX += 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_S) {
        rotationX -= 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_A) {
        rotationY += 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_D) {
        rotationY -= 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_Q) {
        rotationZ += 5.0f;
        update();
    }
    else if (e->key() == Qt::Key_E) {
        rotationZ -= 5.0f;
        update();
    }
}

void MyGLWidget::scene_0()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(50.0f, 50.0f, 0.0f);
    
    //draw a diagonal "I"
    glPushMatrix();
    glColor3f(0.839f, 0.153f, 0.157f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-2.5f, -22.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 45.0f);

    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 45.0f);
    glVertex2f(5.0f, 45.0f);

    glEnd();
    glPopMatrix();    
}


void MyGLWidget::scene_1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);

    // your implementation here, maybe you should write several glBegin
    // glPushMatrix();
    // your implementation
    draw_L_2d(-100, 0, 0);
    draw_S_2d(0, 0, 0);
    draw_Y_2d(100, 0, 0);
}


void MyGLWidget::scene_2() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width() / (GLfloat)height(), 1.0f, 2000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 0.0f, 500.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f
    );

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);

    draw_L_3d(-100, 0, 0);
    draw_S_3d(0, 0, 0);
    draw_Y_3d(100, 0, 0);

    glEnd();
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
}