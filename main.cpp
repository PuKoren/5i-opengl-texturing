#include <GL\glew.h>
#include <GL\freeglut.h>

#include "config.h"
#include "TP1.h"

TP1* tp1;

void MouseMotionTP1(int x, int y){
   tp1->MouseMove(x, y);
}

void IdleTP1(){
   tp1->Update();
}

void DrawTP1(){
   tp1->Draw();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    glutCreateWindow("TP1 - 2D - OpenGL");
    glewInit();

    tp1 = new TP1();
    if(tp1->Init()){
        
        glutPassiveMotionFunc(MouseMotionTP1);
        glutIdleFunc(IdleTP1);
        glutDisplayFunc(DrawTP1);
    }

    glutMainLoop();

    delete tp1;
    glutExit();
    return EXIT_SUCCESS;
}