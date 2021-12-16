#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "utils3d.h"

#define PI 3.1415

const double FRAME_LOCK = 60.0;
double frameRate = 60.0;
bool isRunning = true;
clock_t deltaTime = 0;
unsigned int frames = 0;

char *screen;
char *buf;
HANDLE hConsole;
DWORD dwBytesWritten = 0;
COORD c = {0, 0};
int nScreenWidth = 70;
int nScreenHeight = 50;

void tick();
void init();

double clockToMilliseconds(clock_t ticks)
{
    return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

int main()
{
    init();
    while (isRunning)
    {
        clock_t beginFrame = clock();
        tick();
        while ((clock() - beginFrame) < (1000.0 / FRAME_LOCK))
            ; // Lock to 60fps
        clock_t endFrame = clock();

        deltaTime += endFrame - beginFrame;
        frames++;

        if (clockToMilliseconds(deltaTime) > 1000.0)
        {
            frameRate = (double)frames * 0.5 + frameRate * 0.5;
            frames = 0;
            deltaTime -= 1000.0;
        }
    }

    return 0;
}

void set(int x, int y, char *s, char c)
{
    s[y * nScreenWidth + x] = c;
}

void init()
{
    screen = (char *)malloc(2 * nScreenWidth * nScreenHeight * sizeof(char) + 1 * sizeof(char));
    buf = (char *)malloc(nScreenWidth * nScreenHeight * sizeof(char) + 1 * sizeof(char));
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    for (int i = 0; i < 2 * nScreenWidth * nScreenHeight; i++)
        screen[i] = ' ';
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        buf[i] = ' ';
}

float val = 0.0;
vertex3f rot = {0, 0, 0};
vertex3f ori = {0, 0, 2};

vertex3f v1 = {-1, -1, 1};
vertex3f v2 = { 1, -1, 1};
vertex3f v3 = {-1,  1, 1};
vertex3f v4 = { 1,  1, 1};
vertex3f v5 = {-1, -1, 3};
vertex3f v6 = { 1, -1, 3};
vertex3f v7 = {-1,  1, 3};
vertex3f v8 = { 1,  1, 3};

vertex2f d1, d2, d3, d4, d5, d6, d7, d8;

void tick()
{
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        buf[i] = ' ';

    val += 0.05f;

    if (GetAsyncKeyState((unsigned short)'R') & 0x8000)
    {
    }

    set_vertex(&v1,-1, -1, 1);
    set_vertex(&v2, 1, -1, 1);
    set_vertex(&v3,-1,  1, 1);
    set_vertex(&v4, 1,  1, 1);
    set_vertex(&v5,-1, -1, 3);
    set_vertex(&v6, 1, -1, 3);
    set_vertex(&v7,-1,  1, 3);
    set_vertex(&v8, 1,  1, 3);

    rot.x += PI / 180.0;
    rot.y += PI / 180.0;
    rot.z += PI / 180.0;

    v1 = rotate_vertex(v1, ori, rot);
    v2 = rotate_vertex(v2, ori, rot);
    v3 = rotate_vertex(v3, ori, rot);
    v4 = rotate_vertex(v4, ori, rot);

    v5 = rotate_vertex(v5, ori, rot);
    v6 = rotate_vertex(v6, ori, rot);
    v7 = rotate_vertex(v7, ori, rot);
    v8 = rotate_vertex(v8, ori, rot);
    
    d1 = project_vertex(v1, nScreenWidth, nScreenHeight);
    d2 = project_vertex(v2, nScreenWidth, nScreenHeight);
    d3 = project_vertex(v3, nScreenWidth, nScreenHeight);
    d4 = project_vertex(v4, nScreenWidth, nScreenHeight);

    d5 = project_vertex(v5, nScreenWidth, nScreenHeight);
    d6 = project_vertex(v6, nScreenWidth, nScreenHeight);
    d7 = project_vertex(v7, nScreenWidth, nScreenHeight);
    d8 = project_vertex(v8, nScreenWidth, nScreenHeight);

    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d1, d2);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d2, d4);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d4, d3);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d3, d1);

    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d5, d6);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d6, d8);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d8, d7);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d7, d5);

    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d1, d5);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d2, d6);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d3, d7);
    draw_line(buf, nScreenWidth, nScreenHeight, '\xb2', d4, d8);

    sprintf(buf + nScreenHeight * nScreenWidth - nScreenWidth, "FPS = %.1f", frameRate);
    for (int i = 0; i < nScreenHeight * nScreenWidth; i++)
    {
        screen[2 * i] = buf[nScreenHeight * nScreenWidth - ((i / nScreenWidth + 1) * nScreenWidth) + i % nScreenWidth];
        screen[2 * i + 1] = ' ';
    }
    screen[2 * nScreenWidth * nScreenHeight] = '\0';
    WriteConsoleOutputCharacter(hConsole, screen, 2 * nScreenWidth * nScreenHeight, c, &dwBytesWritten);
}