#include <GL/freeglut.h>
#include <glm/vec3.hpp>
//#include <GLFW/glfw3.h>

double screen_x = 800;
double screen_y = 600;
double t = 0.0;
bool loopExit = false;
int timeSinceStart;
int oldTimeSinceStart = 0;
int deltaTime;

// Main update loop for events happens here
void update(void)
{

    // After updating the simulation/game, tell Glut we're ready for a redraw
    glutPostRedisplay();
}

// Displaying happens here
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();

    glutSwapBuffers();
    //glutPostRedisplay();
}

void reshape(int w, int h)
{

}


#pragma region Keyboard

void processNormalKeys(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 27: // escape character means to quit the program
        loopExit = true;
        break;
    default:
        return; // if we don't care, return without glutPostRedisplay()
    }

    // With the freeglut loop, do I need this?
    //glutPostRedisplay();
}

#pragma endregion Keyboard

#pragma region Mouse

void mouseButton(int mouse_button, int state, int x, int y)
{
    if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        loopExit = true;
    }
}

void mouseMove(int x, int y)
{

}

void mousePassiveMove(int x, int y)
{

}

void mouseWheel(int wheel, int direction, int x, int y)
{
    //wheel: the wheel number, if the mouse has only a wheel this will be zero.
    //direction : a + / -1 value indicating the wheel movement direction
    //x, y : the window mouse coordinates
}

#pragma endregion Mouse


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screen_x, screen_y);
    glutInitWindowPosition(50, 50);

    int fullscreen = 0;
    if (fullscreen)
    {
        glutGameModeString("800x600:32");
        glutEnterGameMode();
    }
    else
    {
        glutCreateWindow("Starter Sample");
    }

    // callbacks for display
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // callbacks for input

    //glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(processNormalKeys);
    //glutSpecialFunc(pressKey);
    //glutSpecialUpFunc(releaseKey);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mousePassiveMove);
    glutMouseWheelFunc(mouseWheel);

    glColor3d(0, 0, 0); // forground color
    glClearColor(1, 1, 1, 0); // background color

    //glutMainLoop();


    while (!loopExit)
    {
        // Get Delta Time for calculations
        timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        glutMainLoopEvent();
        update();
    }


    return 0;
}