

// CS3241 Assignment 1: Doodle
#include <cmath>
#include <iostream>
#include <array>

#ifdef _WIN32
#include <Windows.h>
#include "GL/glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#endif

using namespace std;

GLfloat PI = 3.14;
float alpha = 0.0, k = 1;
float tx = 0.0, ty = 0.0;
float WHEEL_RADIUS = 0.8f;
float HUB_RADIUS = 0.2f;
float LOWER_BODY_LENGTH = 13.75f;
float LOWER_BODY_HEIGHT = 1.f;
float MIDDLE_BODY_LENGTH = 13.f;
float MIDDLE_BODY_HEIGHT = 1.5f;
float UPPER_BODY_LENGTH = 8.f;
float UPPER_BODY_HEIGHT = 1.5f;
std::array<float, 3> BODY_WHITE = {0.949f, 0.945f, 0.929f};
std::array<float, 3> STRIPES_BLUE = {0.031f, 0.054f, 0.521f}; 

void drawLowerBody() {
    glColor3fv(BODY_WHITE.data());
    
    glPushMatrix();
    
    glLoadIdentity();
    glTranslatef(-6.5f, -7.5f, 0.f);

    glBegin(GL_TRIANGLES);

    glVertex2f(0.f, 0.f);
    glVertex2f(LOWER_BODY_LENGTH, 0.f);
    glVertex2f(0.f, -LOWER_BODY_HEIGHT);

    glVertex2f(0.f, -LOWER_BODY_HEIGHT);
    glVertex2f(LOWER_BODY_LENGTH, -LOWER_BODY_HEIGHT);
    glVertex2f(LOWER_BODY_LENGTH, 0.f);

    glEnd();

    glPopMatrix();
}

void drawMiddleBody() {
    glColor3fv(BODY_WHITE.data());

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(-6.1f, -6.5f, 0.f);

    glBegin(GL_TRIANGLES);

    glVertex2f(0.f, 0.f);
    glVertex2f(MIDDLE_BODY_LENGTH, 0.f);
    glVertex2f(0.f, -MIDDLE_BODY_HEIGHT);

    glVertex2f(0.f, -MIDDLE_BODY_HEIGHT);
    glVertex2f(MIDDLE_BODY_LENGTH, -MIDDLE_BODY_HEIGHT);
    glVertex2f(MIDDLE_BODY_LENGTH, 0.f);

    glEnd();

    glPopMatrix();
}

void drawUpperBody() {
    glColor3fv(BODY_WHITE.data());

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-3.5f, -6.5f, 0.f);

    glBegin(GL_POLYGON);

    glVertex2f(0.f, 0.f);
    glVertex2f(9.f, 0.f);
    glVertex2f(7.5f, UPPER_BODY_HEIGHT);
    glVertex2f(3.f, UPPER_BODY_HEIGHT);

    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    glScalef(.9f, .9f, .9f);
    glTranslatef(.5f, 0.f, 0.f);

    glBegin(GL_POLYGON);

    glVertex2f(0.f, 0.f);
    glVertex2f(9.f, 0.f);
    glVertex2f(7.5f, UPPER_BODY_HEIGHT);
    glVertex2f(3.f, UPPER_BODY_HEIGHT);

    glEnd();

    glPopMatrix();
}

void drawLights() {

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(-6.1f, -6.7f, 0.f);

    glBegin(GL_TRIANGLES);

    glColor3f(1.f, 1.f, 1.f);

    glVertex2f(0.f, 0.f);
    glVertex2f(0.f, -.5f);
    glVertex2f(.5f, 0.f);

    glColor3f(1.f, 0.f, 0.f);

    glVertex2f(MIDDLE_BODY_LENGTH, 0.f);
    glVertex2f(MIDDLE_BODY_LENGTH, -0.75f);
    glVertex2f(MIDDLE_BODY_LENGTH - 0.75f, 0.f);

    glEnd();

    glPopMatrix();
}

void drawSpoiler() {
    glColor3f(0.f, 0.f, 0.f);

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(6.f, -6.7f, 0.f);
    glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);  

    glBegin(GL_POLYGON);
    glVertex2f(0.f, 0.f);
    glVertex2f(.5f, 0.f);
    glVertex2f(.5f, 1.f);
    glVertex2f(0.f, 1.f);
    glEnd();

    glTranslatef(.0f, .5f, 0.f);
    glScalef(1.5f, 1.5f, 1.5f);
    glRotatef(20.0f, 0.0f, 0.0f, 1.0f);  

    glBegin(GL_POLYGON);
    glVertex2f(0.f, 0.f);
    glVertex2f(.5f, 0.f);
    glVertex2f(.5f, .5f);
    glVertex2f(0.f, .5f);
    glEnd();

    glPopMatrix();
}

void drawStripes() {
    glColor3fv(STRIPES_BLUE.data());

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-5.5f, -9.5f, 0.f);
    glScalef(0.6f, 0.6f, 0.6f);

    glBegin(GL_TRIANGLES);

    glVertex2f(0.f, 0.f);
    glVertex2f(0.f, 5.f);
    glVertex2f(0.f, 2.f);

    glVertex2f(0.f, 5.f);
    glVertex2f(7.f, 5.f);
    glVertex2f(2.f, 2.f);

    glVertex2f(0.f, 5.f);
    glVertex2f(2.f, 2.f);
    glVertex2f(0.f, 2.f);

    glVertex2f(5.f, 1.65f);
    glVertex2f(11.f, 1.65f);
    glVertex2f(17.f, 5.f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex2f(14.f, 1.65f);
    glVertex2f(20.f, 1.65f);
    glVertex2f(18.f, 5.f);

    glVertex2f(20.f, 1.65f);
    glVertex2f(18.f, 5.f);
    glVertex2f(20.7f, 5.f);

    glEnd();

    glPopMatrix();
}

void drawTyre() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    for (float t = 0; t <= 2 * PI + 0.05; t += 0.05f) {
        glVertex2f(cos(t), sin(t));
    }
    glEnd();
}

void drawRim() {
    glColor3f(0.827f, 0.858f, 0.909f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    for (float i = 0; i <= 2 * PI + 0.05; i += 0.05f) {
        glVertex2f(WHEEL_RADIUS * cos(i), WHEEL_RADIUS * sin(i));
    }
    glEnd();
}

void drawSpokes() {
    glColor3f(0.827f, 0.858f, 0.909f);
    glLineWidth(3.0f);

    glBegin(GL_POLYGON);
    for (float i = 0; i <= 2 * PI + 0.05; i += 0.05f) {
        glVertex2f(HUB_RADIUS * cos(i), HUB_RADIUS * sin(i));
    }
    glEnd();

    glBegin(GL_LINES);
    for (float i = 0; i <= 2 * PI; i += 2 * PI / 10) {
        float x = cos(i);
        float y = sin(i);
        float offset = 20.f;
        float ox = cos(i + offset);
        float oy = sin(i + offset);

        glVertex2f(0, 0);
        glVertex2f(WHEEL_RADIUS * x, WHEEL_RADIUS * y);
        
        glVertex2f(0, 0);
        glVertex2f(WHEEL_RADIUS * ox, WHEEL_RADIUS * oy);
    }
    glEnd();
}

void drawWheel(float x, float y) {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, 0.f);
    drawTyre();
    drawRim();
    drawSpokes();
    glPopMatrix();
}

void display(void) {
    glClearColor(0.462f, 0.784f, 0.960f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glLoadIdentity();
    glScalef(k, k, k);
    glTranslatef(tx, ty, 0);
    glRotatef(alpha, 0, 0, 1);

    drawSpoiler();
    drawLowerBody();
    drawMiddleBody();
    drawUpperBody();
    drawStripes();
    drawLights();
    drawWheel(-5.f, -8.f);
    drawWheel(5.f, -8.f);

    glPopMatrix();
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
}

void keyboard(unsigned char key, int x, int y) {
    //keys to control scaling - k
    //keys to control rotation - alpha
    //keys to control translation - tx, ty
    switch (key) {

        case 'a':
            alpha += 10;
            glutPostRedisplay();
            break;

        case 'd':
            alpha -= 10;
            glutPostRedisplay();
            break;

        case 'q':
            k += 0.1;
            glutPostRedisplay();
            break;

        case 'e':
            if (k > 0.1)
                k -= 0.1;
            glutPostRedisplay();
            break;

        case 'z':
            tx -= 0.1;
            glutPostRedisplay();
            break;

        case 'c':
            tx += 0.1;
            glutPostRedisplay();
            break;

        case 's':
            ty -= 0.1;
            glutPostRedisplay();
            break;

        case 'w':
            ty += 0.1;
            glutPostRedisplay();
            break;

        default:
            break;
    }
}

int main(int argc, char** argv) {
    cout << "CS3241 Lab 1\n\n";
    cout << "+++++CONTROL BUTTONS+++++++\n\n";
    cout << "Scale Up/Down: Q/E\n";
    cout << "Rotate Clockwise/Counter-clockwise: A/D\n";
    cout << "Move Up/Down: W/S\n";
    cout << "Move Left/Right: Z/C\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
