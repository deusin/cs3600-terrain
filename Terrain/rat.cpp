#include "rat.h"
#include <vector>
#include <cstdlib>
#include "glut.h"
#include "graphics.h"

Rat::Rat(double x, double y, double degrees) : x(x), y(y), degrees(degrees)
{
	radius = 0.25;
	speed = 0.04;
	turnspeed = 1.5;
}

void Rat::Draw()
{
	if (current_view == rat_view)
	{
		return; // Don't draw ourselves in rat view
	}
	glColor3d(0, 0, 0);

	glPushMatrix();
	glTranslated(x, y, 0);
	glRotated(degrees, 0, 0, 1);
	glScaled(0.5, 0.5, 1.0);
	//DrawTriangle(0.5, 0, -0.3, 0.2, -0.3, -0.2);
	glBegin(GL_TRIANGLES);
	glVertex2d(0.5, 0);
	glVertex2d(-0.3, 0.2);
	glVertex2d(-0.3, -0.2);
	glEnd();
	// Whiskers
	glBegin(GL_LINES);
	glVertex2d(0.2, 0);
	glVertex2d(0.25, 0.3);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(0.25, 0);
	glVertex2d(0.35, 0.3);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(0.2, 0);
	glVertex2d(0.25, -0.3);
	glEnd();
	glBegin(GL_LINES);
	glVertex2d(0.25, 0);
	glVertex2d(0.35, -0.3);
	glEnd();
	// Tail
	glBegin(GL_LINES);
	for (int i = 8; i < 16; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double x = -0.3 + radius * cos(theta);
		double y = -0.25 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();

	glPopMatrix();
}

void Rat::SpinLeft()
{
	degrees += turnspeed;
}

void Rat::SpinRight()
{
	degrees -= turnspeed;
}

void Rat::ScurryForward()
{

}