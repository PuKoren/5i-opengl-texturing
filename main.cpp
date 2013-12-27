#include <GL\glew.h>
#include <GL\freeglut.h>

#include "config.h"
#include "TP1.h"

TP1* tp1;

float ratio_x_tp1 = WINDOW_WIDTH/WINDOW_WIDTH;
float ratio_y_tp1 = WINDOW_HEIGHT/WINDOW_HEIGHT;

void MouseMotionTP1(int x, int y){
   tp1->MouseMove((int)(x*ratio_x_tp1), (int)(y*ratio_y_tp1));
}

void IdleTP1(){
   tp1->Update();
}

void DrawTP1(){
   tp1->Draw();
}

void ReshapeTP1(int width, int height){
    glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
    ratio_x_tp1 = WINDOW_WIDTH/(float)width;
    ratio_y_tp1 = WINDOW_HEIGHT/(float)height;
    glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed  
    glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)  
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
    glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly  
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
        glutReshapeFunc(ReshapeTP1);
    }

    glutMainLoop();

    delete tp1;
    glutExit();
    return EXIT_SUCCESS;
}