#include <iostream>
#include <ctime>
#include <GL/freeglut.h>
#include "camera.h"
#include <glm/glm.hpp>

double screen_x = 800;
double screen_y = 600;
double t = 0.0;
double squareSize = 200.0;
int sizeX = 100;
int sizeZ = 100;
bool loopExit = false;
int timeSinceStart;
int oldTimeSinceStart = 0;
int deltaTime;
int waterLevel = 0;
Camera* camera;
double maxHeight = 2000;
double eye[3] = { 4500, 8000, -4000 }; // pick a nice vantage point.

// As t goes from t0 to t1, set v between v0 and v1 accordingly.
void Interpolate(double t, double t0, double t1, double& v, double v0, double v1)
{
    double ratio = (t - t0) / (t1 - t0);
    if (ratio < 0)
        ratio = 0;
    if (ratio > 1)
        ratio = 1;
    v = v0 + (v1 - v0) * ratio;
}

double GetTime()
{
    static clock_t start_time = clock();
    clock_t current_time = clock();
    double total_time = double(current_time - start_time) / CLOCKS_PER_SEC;
    return total_time;
}

double yScale(double x, double z)
{
    double y = 0;
    double yScale = 200.00;

    double xratio = x / (sizeX * squareSize);
    double zratio = z / (sizeZ * squareSize);

    y += 2.1 * sin(xratio * 2 * 3.14 * 5);
    y += 1.2 * cos(zratio * 9 * 3.14);
    y += 2.7 * sin(zratio * 2 * 3.14 * 3);
    y += cos(zratio * 11 * 3.14) * sin(xratio * 8.2);
    y += sin(zratio * 2 * 3.14) * cos(xratio * 3.14 * 8);


    return y * yScale;
}

void drawWaterLine()
{
    glm::vec3 bl, br, tl, tr;
    bl.x = 0;
    bl.y = waterLevel;
    bl.z = 0;

    tl.x = 0;
    tl.y = waterLevel;
    tl.z = sizeZ * squareSize;

    br.x = sizeX * squareSize;
    br.y = waterLevel;
    br.z = 0;

    tr.x = sizeX * squareSize;
    tr.y = waterLevel;
    tr.z = sizeZ * squareSize;


    //glColor3f(0.2, 0.35, 1.0);
    GLfloat water[]{ 0.2, 0.35, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, water);
    glBegin(GL_QUADS);
    glVertex3d(bl.x, bl.y, bl.z);
    glVertex3d(tl.x, tl.y, tl.z);
    glVertex3d(tr.x, tr.y, tr.z);
    glVertex3d(br.x, br.y, br.z);
    glEnd();
}

void drawSquare(int i, int j)
{
    glm::vec3 bl, br, tl, tr;
    bl.x = i * squareSize;
    bl.z = j * squareSize;

    tl.x = i * squareSize;
    tl.z = (j + 1) * squareSize;

    br.x = (i + 1) * squareSize;
    br.z = j * squareSize;

    tr.x = (i + 1) * squareSize;
    tr.z = (j + 1) * squareSize;

    // Heights
    bl.y = yScale(bl.x, bl.z);
    tl.y = yScale(tl.x, tl.z);
    br.y = yScale(br.x, br.z);
    tr.y = yScale(tr.x, tr.z);

    double avgHeight = (bl.y + tl.y + br.y + tr.y) / 4.0; 
    if (avgHeight > maxHeight)
        maxHeight = avgHeight;
    double hRatio = avgHeight / maxHeight;

    double lowRed = 0.96;
    double highRed = 0.54;
    double lowGreen = 0.87;
    double highGreen = 0.27;
    double lowBlue = 0.7;
    double highBlue = 0.07;

    if (avgHeight < 0)
    {
        //glColor3f(lowRed, lowGreen, lowBlue); // wheat
        GLfloat low[]{ lowRed, lowGreen, lowBlue, 1.0 };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, low);
    }
    else
    {
        double newRed, newGreen, newBlue;
        
        Interpolate(hRatio, 0, 1, newRed, lowRed, highRed);
        Interpolate(hRatio, 0, 1, newGreen, lowGreen, highGreen);
        Interpolate(hRatio, 0, 1, newBlue, lowBlue, highBlue);

        //glColor3f(newRed, newGreen, newBlue);
        GLfloat mat[]{ newRed, newGreen, newBlue, 1.0 };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat);
    }
    glBegin(GL_QUADS);
    glVertex3d(bl.x, bl.y, bl.z);
    glVertex3d(tl.x, tl.y, tl.z);
    glVertex3d(tr.x, tr.y, tr.z);
    glVertex3d(br.x, br.y, br.z);
    glEnd();
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

    // Draw our water line
    drawWaterLine();

    // Draw a square to move around
    for (size_t i = 0; i < sizeX; i++)
    {
        for (size_t j = 0; j < sizeZ; j++)
        {
            drawSquare(i, j);
        }
    }

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

#pragma region Keyboard
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
    case 'f':
        CamMove.flyingMode = !CamMove.flyingMode;
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
    if (CamMove.flyingMode)
    {
        CamMove.mouseYOffset = yoffset;
    }
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
    // set material's specular properties
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // set light properties
    GLfloat light_position[] = { (float)eye[0], (float)eye[1], (float)eye[2],1 };
    GLfloat white_light[] = { 1,1,1,1 };
    GLfloat low_light[] = { .3f,.3f,.3f,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
    glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

    glEnable(GL_DEPTH_TEST); // turn on depth buffering
    glEnable(GL_LIGHTING);	// enable general lighting
    glEnable(GL_LIGHT0);	// enable the first light.

    // Create our camera
    camera = new Camera(glm::vec3(0, 1000, 0), glm::vec3(0, 1, 0), 0.0f, 0.0f);

}

void update(int deltaTime)
{
    camera->Update(deltaTime);


    // If in flying mode, snap to x units above the ground at this position
    if (!CamMove.flyingMode)
    {
        double newYPosition = yScale(camera->Position.x, camera->Position.z);
        if (newYPosition < waterLevel)
        {
            newYPosition = waterLevel;
        }
        camera->Position.y = newYPosition + 100;

        double frontYPosition = yScale(camera->Position.x + camera->Front.x, camera->Position.z + camera->Front.z);
        if (frontYPosition < waterLevel)
        {
            frontYPosition = waterLevel;
        }

        // Testing
        if (newYPosition == frontYPosition)
        {
            camera->Pitch = 0;
            camera->updateCameraVectors();
        }
        else
        {
            double newPitch = atan((frontYPosition - newYPosition) / 1.0) * 180 / 3.14;
            
            camera->Pitch = newPitch;
            camera->updateCameraVectors();
        }

    }

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