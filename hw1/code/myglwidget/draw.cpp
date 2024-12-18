#include "draw.h"


void randomColor() {
    static float r = rand() / (float)RAND_MAX;
    static float g = rand() / (float)RAND_MAX;
    static float b = rand() / (float)RAND_MAX;
    float step = 0.10f;
    r += (rand() / (float)RAND_MAX - 0.5f) * step;
    g += (rand() / (float)RAND_MAX - 0.5f) * step;
    b += (rand() / (float)RAND_MAX - 0.5f) * step;
    r = fmin(fmax(r, 0.0f), 1.0f);
    g = fmin(fmax(g, 0.0f), 1.0f);
    b = fmin(fmax(b, 0.0f), 1.0f);
    glColor3f(r, g, b);
    return;
}

void draw2dTriangle(
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float z
) {
    glPushMatrix();
    glBegin(GL_TRIANGLES);
        glVertex3f(x1, y1, z);
        glVertex3f(x2, y2, z);
        glVertex3f(x3, y3, z);
    glEnd();
    glPopMatrix();
}

void draw2dRectangle(float x1, float y1, float x2, float y2, float z) {
    glPushMatrix();
    glBegin(GL_QUADS);
        glVertex3f(x1, y1, z);
        glVertex3f(x2, y1, z);
        glVertex3f(x2, y2, z);
        glVertex3f(x1, y2, z);
    glEnd();
    glPopMatrix();
}

void drawSideRectangle(float x1, float y1, float x2, float y2, float z){
    glPushMatrix();
    glBegin(GL_QUADS);
        glVertex3f(x1, y1, z + CHAR_THICKNESS);
        glVertex3f(x1, y1, z - CHAR_THICKNESS);
        glVertex3f(x2, y2, z - CHAR_THICKNESS);
        glVertex3f(x2, y2, z + CHAR_THICKNESS);
    glEnd();
    glPopMatrix();
}


void draw_L_2d(float dx, float dy, float dz){
    draw2dRectangle(dx, dy, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, dz);
    draw2dRectangle(dx, dy, dx + CHAR_WIDTH, dy + CHAR_THICKNESS, dz);
}

void draw_S_2d(float dx, float dy, float dz){
    draw2dRectangle(dx, dy, dx + CHAR_WIDTH, dy + CHAR_THICKNESS, dz);
    draw2dRectangle(dx, dy + (CHAR_HEIGHT - CHAR_THICKNESS) * 0.5, dx + CHAR_WIDTH, dy + (CHAR_HEIGHT + CHAR_THICKNESS) * 0.5, dz);
    draw2dRectangle(dx, dy + CHAR_HEIGHT - CHAR_THICKNESS, dx + CHAR_WIDTH, dy + CHAR_HEIGHT, dz);
    draw2dRectangle(dx, dy + CHAR_HEIGHT * 0.5, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, dz);
    draw2dRectangle(dx, dy, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT * 0.25, dz);
    draw2dRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy, dx + CHAR_WIDTH, dy + CHAR_HEIGHT * 0.5, dz);
    draw2dRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT * 0.75, dx + CHAR_WIDTH, dy + CHAR_HEIGHT, dz);
}

void draw_Y_2d(float dx, float dy, float dz){
    draw2dTriangle(
        dx, dy + CHAR_HEIGHT, 
        dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, 
        dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5,
        dz
    );
    draw2dTriangle(
        dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, 
        dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5,
        dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5,
        dz
    );
    draw2dTriangle(
        dx + CHAR_WIDTH, dy + CHAR_HEIGHT, 
        dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT, 
        dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5,
        dz
    );
    draw2dTriangle(
        dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT, 
        dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5,
        dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5,
        dz
    );
    draw2dRectangle(dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy, dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dz);
}


void draw_L_3d(float dx, float dy, float dz){
    randomColor();
    draw_L_2d(dx, dy, dz + CHAR_THICKNESS * 0.5);
    randomColor();
    draw_L_2d(dx, dy, dz - CHAR_THICKNESS * 0.5);
    randomColor();
    drawSideRectangle(dx, dy, dx, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx, dy + CHAR_HEIGHT, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, dx + CHAR_THICKNESS, dy + CHAR_THICKNESS, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + CHAR_THICKNESS, dx + CHAR_WIDTH, dy + CHAR_THICKNESS, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy + CHAR_THICKNESS, dx + CHAR_WIDTH, dy, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy, dx, dy, dz);
}


void draw_S_3d(float dx, float dy, float dz){
    randomColor();
    draw_S_2d(dx, dy, dz + CHAR_THICKNESS * 0.5);
    randomColor();
    draw_S_2d(dx, dy, dz - CHAR_THICKNESS * 0.5);
    randomColor();
    drawSideRectangle(dx, dy, dx, dy + CHAR_HEIGHT * 0.25, dz);
    randomColor();
    drawSideRectangle(dx, dy + CHAR_HEIGHT * 0.25, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT * 0.25, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + CHAR_HEIGHT * 0.25, dx + CHAR_THICKNESS, dy + CHAR_THICKNESS, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + CHAR_THICKNESS, dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_THICKNESS, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_THICKNESS, dx + CHAR_WIDTH - CHAR_THICKNESS, dy + (CHAR_HEIGHT - CHAR_THICKNESS) * 0.5, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy + (CHAR_HEIGHT - CHAR_THICKNESS) * 0.5, dx, dy + (CHAR_HEIGHT - CHAR_THICKNESS) * 0.5, dz);
    randomColor();
    drawSideRectangle(dx, dy + (CHAR_HEIGHT - CHAR_THICKNESS) * 0.5, dx, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx, dy + CHAR_HEIGHT, dx + CHAR_WIDTH, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy + CHAR_HEIGHT, dx + CHAR_WIDTH, dy + CHAR_HEIGHT * 0.75, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy + CHAR_HEIGHT * 0.75, dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT * 0.75, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT * 0.75, dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT - CHAR_THICKNESS, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT - CHAR_THICKNESS, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT - CHAR_THICKNESS, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + CHAR_HEIGHT - CHAR_THICKNESS, dx + CHAR_THICKNESS, dy + (CHAR_HEIGHT + CHAR_THICKNESS) * 0.5, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + (CHAR_HEIGHT + CHAR_THICKNESS) * 0.5, dx + CHAR_WIDTH, dy + (CHAR_HEIGHT + CHAR_THICKNESS) * 0.5, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy + (CHAR_HEIGHT + CHAR_THICKNESS) * 0.5, dx + CHAR_WIDTH, dy, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy, dx, dy, dz);
}

void draw_Y_3d(float dx, float dy, float dz){
    randomColor();
    draw_Y_2d(dx, dy, dz + CHAR_THICKNESS * 0.5);
    randomColor();
    draw_Y_2d(dx, dy, dz - CHAR_THICKNESS * 0.5);
    randomColor();
    drawSideRectangle(dx, dy + CHAR_HEIGHT, dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_THICKNESS, dy + CHAR_HEIGHT, dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dz);
    randomColor();
    drawSideRectangle(dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH - CHAR_THICKNESS, dy + CHAR_HEIGHT, dx + CHAR_WIDTH, dy + CHAR_HEIGHT, dz);
    randomColor();
    drawSideRectangle(dx + CHAR_WIDTH, dy + CHAR_HEIGHT, dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dz);
    randomColor();
    drawSideRectangle(dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy, dz);
    randomColor();
    drawSideRectangle(dx + (CHAR_WIDTH + CHAR_THICKNESS) * 0.5, dy, dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy, dz);
    randomColor();
    drawSideRectangle(dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy, dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dz);
    randomColor();
    drawSideRectangle(dx + (CHAR_WIDTH - CHAR_THICKNESS) * 0.5, dy + CHAR_HEIGHT * 0.5, dx, dy + CHAR_HEIGHT, dz);
}
