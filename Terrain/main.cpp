#include <iostream>
#include <ctime>
#include <GL/freeglut.h>
#include "camera.h"

double screen_x = 800;
double screen_y = 600;
double t = 0.0;
bool loopExit = false;
int timeSinceStart;
int oldTimeSinceStart = 0;
int deltaTime;
Camera* camera;


double GetTime()
{
    static clock_t start_time = clock();
    clock_t current_time = clock();
    double total_time = double(current_time - start_time) / CLOCKS_PER_SEC;
    return total_time;
}

// Displaying happens here
void display(void)
{
    double t = GetTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,1,0); // Y is up!
    gluLookAt(
        camera->Position.x, camera->Position.y, camera->Position.z,
        camera->Position.x + camera->Front.x, camera->Position.y + camera->Front.y, camera->Position.z + camera->Front.z,
        //at[0], at[1], at[2],
        0, 1, 0
    );

    // Draw a square to move around
    int i = 0;
    int j = 0;
    GLfloat light[]{ 0.5, 0.5, 0.5, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, light);
    glBegin(GL_QUADS);
    glVertex3d(500.0 + (i * 1000.0), 0, 500 + (j * 1000.0));
    glVertex3d(500.0 + (i * 1000.0), 0, 500 + ((j + 1.0) * 1000.0));
    glVertex3d(500.0 + ((i + 1.0) * 1000.0), 0, 500 + ((j + 1.0) * 1000.0));
    glVertex3d(500.0 + ((i + 1.0) * 1000.0), 0, 500 + (j * 1000.0));

    glEnd();


    //drawBoard();
    //drawMovingPieces();
    //drawPieces();

    GLfloat light_position[] = { 1,2,-.1f, 0 }; // light comes FROM this vector direction.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light

    glutSwapBuffers();
    glutPostRedisplay();
}

void SetPerspectiveView(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspectRatio = (GLdouble)w / (GLdouble)h;
    gluPerspective(
        /* field of view in degree */ 45.0,
        /* aspect ratio */ aspectRatio,
        /* Z near */ 100, /* Z far */ 30000.0);
    glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
    screen_x = w;
    screen_y = h;

    // Set the pixel resolution of the final picture (Screen coordinates).
    glViewport(0, 0, w, h);

    SetPerspectiveView(w, h);

}


void asciiKeyboardDown(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 27: // escape character means to quit the program
        exit(0);
        break;
    case 32: // space bar
        CamMove.up = true;
        break;
    case 'w':
        CamMove.forward = true;
        break;
    case 's':
        CamMove.back = true;
        break;
    case 'a':
        CamMove.left = true;
        break;
    case 'd':
        CamMove.right = true;
        break;
    case 'c':
        std::cout << camera->Position.x << ", " << camera->Position.y << ", " << camera->Position.z << "\n";
        std::cout << camera->Front.x << ", " << camera->Front.y << ", " << camera->Front.z << "\n";
        std::cout << "P: " << camera->Pitch << ", Y: " << camera->Yaw << "\n";
        break;
    default:
        return; // if we don't care, return without glutPostRedisplay()
    }
    //camera->ProcessKeyboard(deltaTime);
    //glutPostRedisplay();
}

void asciiKeyboardUp(unsigned char c, int x, int y)
{
    switch (c)
    {
    case 32: // space bar
        CamMove.up = false;
        break;
    case 'w':
        CamMove.forward = false;
        break;
    case 's':
        CamMove.back = false;
        break;
    case 'a':
        CamMove.left = false;
        break;
    case 'd':
        CamMove.right = false;
        break;
    default:
        return; // if we don't care, return without glutPostRedisplay()
    }
    //camera->ProcessKeyboard(deltaTime);
    //glutPostRedisplay();
}

#pragma endregion Keyboard

#pragma region Mouse
// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouseButton(int mouse_button, int state, int x, int y)
{
    if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    }
    if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
    }
    if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    {
    }
    if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
    {
    }
    glutPostRedisplay();
}

void mouseMove(int x, int y)
{

}

void mousePassiveMove(int x, int y)
{
    static bool startMouse = true;

    float xoffset = x - (screen_x / 2);
    float yoffset = (screen_y / 2) - y; // reversed since y coordinates go from top to bottom

    // warping the pointer causes a mousePassiveMove to fire again ... even if we didn't actually move.
    // So, let's not warp the pointer if we didn't move to avoid unnecessary processing.
    if (x != screen_x / 2 || y != screen_y / 2)
        glutWarpPointer(screen_x / 2, screen_y / 2);

    //std::cout << "At: " << x << ", " << y << "\n";
    CamMove.mouseXOffset = xoffset;
    CamMove.mouseYOffset = yoffset;
}

void mouseWheel(int wheel, int direction, int x, int y)
{
    //wheel: the wheel number, if the mouse has only a wheel this will be zero.
    //direction : a + / -1 value indicating the wheel movement direction
    //x, y : the window mouse coordinates
}


#pragma endregion Mouse

void InitializeMyStuff()
{
    // Create our camera
    camera = new Camera(glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0), 0.0f, 0.0f);

}

void update(int deltaTime)
{
    camera->Update(deltaTime);

    double t = GetTime();
}

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
        glutCreateWindow("Terrain");
    }

    // callbacks for display
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // callbacks for input

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(asciiKeyboardDown);
    glutKeyboardUpFunc(asciiKeyboardUp);
    //glutSpecialFunc(pressKey);
    //glutSpecialUpFunc(releaseKey);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mousePassiveMove);
    glutMouseWheelFunc(mouseWheel);

    glClearColor(0, 0, 0, 1);
    InitializeMyStuff();
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(screen_x / 2, screen_y / 2);

    while (!loopExit)
    {
        // Get Delta Time for calculations
        timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        glutMainLoopEvent();
        update(deltaTime);
        glutPostRedisplay();
    }

    return 0;
}