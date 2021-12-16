#ifndef UTILS3D_H
#define UTILS3D_H

typedef struct _vertex3f {
    float x, y, z;
} vertex3f;

typedef struct _vertex2f {
    float x, y;
} vertex2f;

void set_vertex(vertex3f* v, float x, float y, float z);
vertex3f rotate_vertex(vertex3f point, vertex3f origin, vertex3f angle);
vertex2f project_vertex(vertex3f v3, double width, double height);
void draw_line(char *buf, int width, int height, char c, vertex2f v1, vertex2f v2);

#endif