#include "TetrisGame.h"

TetrisGame tetris;

void Display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    tetris.Display();
}

void NonPrintableKeys(int key, int x, int y) {
    tetris.NonPrintableKeys(key, x, y);
}

void Timer(int m) {
    glutPostRedisplay();
    glutTimerFunc(500, Timer, 0); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize(tetris.GetWidth(), tetris.GetHeight());
    glutInitWindowPosition(400, 180);
    glutCreateWindow("Tetris Game");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, tetris.GetWidth(), 0, tetris.GetHeight(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(Display);

    glutSpecialFunc(NonPrintableKeys);

    glutTimerFunc(500, Timer, 0);

    glutMainLoop();
    return 0;
}