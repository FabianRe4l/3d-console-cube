#include "utils3d.h"
#include <math.h>
#include <stdio.h>

#define FOV ((3) * (3.1415))

void set_char(int x, int y, char *buf, char c, int buf_width, int buf_height)
{
    if (x < 0 || x > buf_width || y < 0 || y > buf_height)
    {
        sprintf(buf, "%s", "Error1");
    }
    else
    {
        buf[y * buf_width + x] = c;
    }
}

void set_vertex(vertex3f* v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

vertex3f rotate_vertex(vertex3f point, vertex3f origin, vertex3f angle)
{
    // Set origin at 0 0 0
    point.x = point.x - origin.x;
    point.y = point.y - origin.y;
    point.z = point.z - origin.z;

    //rotate x y z
    float y = point.y * cos(angle.x) - point.z * sin(angle.x);
    point.z = point.y * sin(angle.x) + point.z * cos(angle.x);
    point.y = y;

    float x = point.x * cos(angle.y) + point.z * sin(angle.y);
    point.z = point.z * cos(angle.y) - point.x * sin(angle.y);
    point.x = x;

    x = point.x * cos(angle.z) - point.y * sin(angle.z);
    point.y = point.x * sin(angle.z) + point.y * cos(angle.z);
    point.x = x;
    // Set origin back
    point.x = point.x + origin.x;
    point.y = point.y + origin.y;
    point.z = point.z + origin.z;

    return point;
}

vertex2f project_vertex(vertex3f v3, double width, double height)
{
    double d = sqrt(v3.x * v3.x + v3.y * v3.y + v3.z * v3.z);
    vertex2f v2 = {FOV * (d / (double)v3.z) * (double)v3.x + 0.5 * (double)width, FOV * (d / (double)v3.z) * (double)v3.y + 0.5 * (double)height};
    return v2;
}

void draw_line(char *buf, int width, int height, char c, vertex2f v1, vertex2f v2)
{
    int deltaY = v2.y - v1.y;
    int deltaX = v2.x - v1.x;

    double m = (double)deltaY / (double)deltaX;
    if (m >= 1 || m <= -1)
    {
        for (int i = 0; i <= abs(deltaY); i++)
        {
            set_char(round(v1.x + (deltaX > 0 ? 1 : -1) * round((1 / (m > 0 ? m : -m)) * i)), round(v1.y + (deltaY < 0 ? -1 : 1) * i), buf, c, width, height);
        }
    }
    else if (m < 1 || m > -1)
    {
        for (int i = 0; i <= abs(deltaX); i++)
        {
            set_char(round(v1.x + (deltaX < 0 ? -1 : 1) * i), round(v1.y + (deltaY > 0 ? 1 : -1) * round((m > 0 ? m : -m) * i)), buf, c, width, height);
        }
    }
}