#include <vector>
#include <cstdlib>
#include "glut.h"
#include "graphics.h"
#include "maze.h"

Cell::Cell()
{
    left = top = right = bottom = true;
    visited = false;
}
void Cell::DrawWall(int x1, int y1, int x2, int y2)
{
    double green = (double)(y1) / 1.5 / (double)(HEIGHT);
    double blue = (double)(x1) / 1.5 / (double)(WIDTH);

    glColor3d(0.4, green, blue);



    glBegin(GL_QUADS);
    glVertex3i(x1, y1, 0);
    glVertex3i(x2, y2, 0);
    glVertex3i(x2, y2, 1);
    glVertex3i(x1, y1, 1);
    glEnd();
}
void Cell::Draw(int x, int y)
{
    // draw walls as GL_LINES
    if (current_view == top_view)
    {
        if (left)
            DrawLine(x, y, x, y + 1);
        if (top)
            DrawLine(x, y + 1, x + 1, y + 1);
        if (right)
            DrawLine(x + 1, y + 1, x + 1, y);
        if (bottom)
            DrawLine(x + 1, y, x, y);
    }
    else
    {
        if (left)
        {
            DrawWall(x, y, x, y + 1);
        }
        if (top)
        {
            DrawWall(x, y + 1, x + 1, y + 1);
        }
        if (right)
        {
            // Only draw the rightmost wall, the others are handled by the lefts already
            if (x == WIDTH - 1)
            {
                DrawWall(x + 1, y + 1, x + 1, y);
            }
        }
        if (bottom)
        {
            // Only draw the bottom wall, the others are handled by the tops already
            if (y == 0)
            {
                DrawWall(x + 1, y, x, y);
            }
        }
    }

}



Maze::Maze()
{

}
void Maze::RemoveWalls()
{
    RemoveWallsR(0, 0);

    cells[0][0].bottom = false;
    cells[WIDTH - 1][HEIGHT - 1].top = false;
}

void Maze::RemoveWallsR(int i, int j)
{
    cells[i][j].visited = true;

    //while (true)
    //{
        // record legal possibilitites between LEFT, UP, RIGHT, DOWN
        enum MOVES { LEFT, UP, RIGHT, DOWN };
        std::vector<MOVES> moves;

        // check for a legal LEFT move
        if (i - 1 >= 0 && !cells[i - 1][j].visited)
        {
            moves.push_back(LEFT);
        }
        if (i + 1 < WIDTH && !cells[i + 1][j].visited)
        {
            moves.push_back(RIGHT);
        }
        if (j - 1 >= 0 && !cells[i][j - 1].visited)
        {
            moves.push_back(DOWN);
        }
        if (j + 1 < HEIGHT && !cells[i][j + 1].visited)
        {
            moves.push_back(UP);
        }

        // check other 3 directions

        if (moves.size() == 0)
        {
            return;
        }

        // pick which direction randomly
        int r = rand() % moves.size();

        if (moves[r] == LEFT)
        {
            cells[i][j].left = false;
            cells[i - 1][j].right = false;
            RemoveWallsR(i - 1, j);
        }
        if (moves[r] == RIGHT)
        {
            cells[i][j].right = false;
            cells[i + 1][j].left = false;
            RemoveWallsR(i + 1, j);
        }
        if (moves[r] == UP)
        {
            cells[i][j].top = false;
            cells[i][j + 1].bottom = false;
            RemoveWallsR(i, j + 1);
        }
        if (moves[r] == DOWN)
        {
            cells[i][j].bottom = false;
            cells[i][j - 1].top = false;
            RemoveWallsR(i, j - 1);
        }
        // Likewise for other 3 directions

//    }

}

void Maze::Draw()
{
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
            cells[i][j].Draw(i, j);
}

bool Maze::IsSafe(double x, double y, double r)
{
    // TEST QUESTION

    // What cell am I in?
    int i = (int)x;
    int j = (int)y;

    // Where am I in the cell?
    double xOffset = x - i;
    double yOffset = y - j;

    // There's a wall there and I'm colliding
    if (cells[i][j].right && xOffset + r > 1.0)
        return false;
    if (cells[i][j].left && xOffset - r < 0.0)
        return false;
    if (cells[i][j].top && yOffset + r > 1.0)
        return false;
    if (cells[i][j].bottom && yOffset - r < 0.0)
        return false;

    // We know that there is always an edge on every corner based on the way we drew the maze
    if (xOffset + r > 1.0 && yOffset + r > 1.0)
        return false;
    if (xOffset + r > 1.0 && yOffset - r < 0.0)
        return false;
    if (xOffset - r < 0.0 && yOffset + r > 1.0)
        return false;
    if (xOffset - r < 0.0 && yOffset - r < 0.0)
        return false;


    // No collisions
    return true;


}