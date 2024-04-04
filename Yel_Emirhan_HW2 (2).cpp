/*********
   CTIS164 - The Bird Hunt
----------
STUDENT : Emirhan Yel 22103407
SECTION : 01
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define MAXTARGET 5
#define RADIUSTARGET 30

#define SPEED1 0 
#define SPEED2  1
#define SPEED3  2


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


#define MAXAR 30

typedef struct {
    double x, y;
}position_t;

typedef struct {

    int r, g, b;
}colors_t;

typedef struct {

    position_t pos;

}gun_t;

typedef struct {
    position_t pos;

    bool active;
}arrow_fire_t;

typedef struct {

    float x, y;
}pnt_t;

typedef struct //target structure
{
    pnt_t pos_center;
    colors_t color;
    float speed;
    float radius;

}target_t;

target_t target[MAXTARGET];



gun_t gun = { 0,350 };
bool spacebar = false;
arrow_fire_t fire = { {0,350},false };
bool activeTimer = true;
float counter = 20;


int speed_arrow_state = SPEED1;
double speedarrow;


int hit = 0;
double CloudX1 = 360, CloudY1 = 200;
double CloudX2 = 280, CloudY2 = 100;


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}




void drawgun()
{

    glColor3f(204 / 255.0, 102.0 / 255, 0);
    glRectf(gun.pos.x - 10, gun.pos.y, gun.pos.x + 10, gun.pos.y - 70);


    glColor3f(204 / 255.0, 102.0 / 255, 0);
    glRectf(gun.pos.x - 60, gun.pos.y - 40, gun.pos.x + 60, gun.pos.y - 60);


    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(gun.pos.x - 10, gun.pos.y);
    glVertex2f(gun.pos.x - 60, gun.pos.y - 40);
    glEnd();


    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(gun.pos.x + 10, gun.pos.y);
    glVertex2f(gun.pos.x + 60, gun.pos.y - 40);
    glEnd();


    glBegin(GL_TRIANGLES);
    glVertex2f(gun.pos.x - 15, gun.pos.y - 70);
    glVertex2f(gun.pos.x + 15, gun.pos.y - 70);
    glVertex2f(gun.pos.x, gun.pos.y - 90);
    glEnd();

}


void fire_arrow(arrow_fire_t fre)
{
    if (fre.active) {

        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(fre.pos.x, fre.pos.y);
        glVertex2f(fre.pos.x, fre.pos.y - 5);
        glEnd();


        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(fre.pos.x - 5, fre.pos.y - 5);
        glVertex2f(fre.pos.x, fre.pos.y - 5);
        glEnd();


        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(fre.pos.x + 5, fre.pos.y - 5);
        glVertex2f(fre.pos.x, fre.pos.y - 5);
        glEnd();


        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(fre.pos.x - 8, fre.pos.y - 8);
        glVertex2f(fre.pos.x, fre.pos.y - 5);
        glEnd();

        glColor3ub(255, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(fre.pos.x + 8, fre.pos.y - 8);
        glVertex2f(fre.pos.x, fre.pos.y - 5);
        glEnd();



        glColor3ub(255, 0, 0);
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2f(fre.pos.x, fre.pos.y - 5);
        glVertex2f(fre.pos.x, fre.pos.y - 5 - 30);
        glEnd();


        glBegin(GL_TRIANGLES);
        glVertex2f(fre.pos.x + 6, fre.pos.y - 5 - 20);
        glVertex2f(fre.pos.x - 6, fre.pos.y - 5 - 20);
        glVertex2f(fre.pos.x, fre.pos.y - 5 - 30 - 7);
        glEnd();


    }

}

void clouddraw(double x, double y)
{
    glColor3f(1, 1, 1);
    circle(x, y, 20);
    circle(x - 3, y - 35, 20);
    circle(x + 15, y - 16, 20);
    circle(x - 15, y - 16, 20);
}

void backgrnd()
{

    //grass
    glColor3ub(0, 204, 0);
    glRectf(-500, -350, 500, -240);


    glColor3ub(204, 102, 0);
    glRectf(-70, -240, 70, -120);


    glBegin(GL_TRIANGLES);
    glColor3ub(102, 51, 0);
    glVertex2f(-90, -120);
    glVertex2f(90, -120);
    glVertex2f(0, -40);
    glEnd();

    glRectf(-15, -240, 15, -190);

    glColor3ub(204, 102, 0);
    circle(5, -205, 5);

    glColor3ub(192, 192, 192);
    glRectf(-25, -175, -55, -145);
    glRectf(25, -175, 55, -145);


    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-40, -145);
    glVertex2f(-40, -175);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-55, -160);
    glVertex2f(-25, -160);
    glEnd();


    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(40, -145);
    glVertex2f(40, -175);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(55, -160);
    glVertex2f(25, -160);
    glEnd();



    glColor3ub(102, 51, 0);
    glRectf(-330, -240, -270, -120);

    glColor3f(0, 102.0 / 256, 0);
    circle(-300, -110, 35);
    circle(-300, -55, 35);
    circle(-300 + 35, -93, 35);
    circle(-300 - 35, -93, 35);

    glColor3ub(102, 51, 0);
    glRectf(330, -240, 270, -120);

    glColor3f(0, 102.0 / 256, 0);
    circle(300, -110, 35);
    circle(300, -55, 35);
    circle(300 - 35, -93, 35);
    circle(300 + 35, -93, 35);



    glColor3ub(20, 20, 20);
    glRectf(-500, 230, -320, 140);
    glColor3ub(0, 110.0, 0);
    glRectf(-500, 230, -330, 150);
    glColor3ub(0, 0, 235.0);


    if (counter > 0 && counter < 20) {
        glColor3ub(198.0, 26.0, 0);
        vprint(-490, 115, GLUT_BITMAP_9_BY_15, "Press F1 to stop/continue");
        vprint(-490, 100, GLUT_BITMAP_9_BY_15, "Press SPACE to fire the arrow");


        glColor3ub(0, 0, 235.0);
        vprint(-485, 200, GLUT_BITMAP_9_BY_15, "Time");
        vprint(-485, 175, GLUT_BITMAP_9_BY_15, "Remaining: %4.2f", counter);

        glColor3ub(198.0, 26.0, 0);
        if (speed_arrow_state == SPEED1)
        {
            vprint(-490, 85, GLUT_BITMAP_9_BY_15, "Press <DOWN> button to incrase speed to SPEED 2");
            vprint(-490, 68, GLUT_BITMAP_9_BY_15, "TARGET HIT: %d", hit);
        }
        if (speed_arrow_state == SPEED2)
        {

            vprint(-490, 85, GLUT_BITMAP_9_BY_15, "Press <UP> button to decrease speed to SPEED 1");
            vprint(-490, 68, GLUT_BITMAP_9_BY_15, "Press <DOWN> button to incrase speed to SPEED 3");
            vprint(-490, 51, GLUT_BITMAP_9_BY_15, "TARGET HIT: %d", hit);
        }

        if (speed_arrow_state == SPEED3) {
            vprint(-490, 85, GLUT_BITMAP_9_BY_15, "Press <UP> button to decrease speed to SPEED 2");
            vprint(-490, 68, GLUT_BITMAP_9_BY_15, "TARGET HIT: %d", hit);
        }

    }

    else
    {
        glColor3ub(198.0, 26.0, 0);
        vprint(-490, 115, GLUT_BITMAP_9_BY_15, "Press F5 to Re-start");

        if (counter == 0)
            vprint(-490, 98, GLUT_BITMAP_9_BY_15, "TARGET HIT: %d", hit);

        glColor3ub(0, 0, 235.0);
        vprint(-485, 200, GLUT_BITMAP_9_BY_15, "Time");
        vprint(-485, 175, GLUT_BITMAP_9_BY_15, "Remaining: %4.2f", counter);

    }




}

void targetdraw(target_t tar)
{
    glColor3ub(tar.color.r, tar.color.g, tar.color.b);

    circle(tar.pos_center.x, tar.pos_center.y, tar.radius);

    glBegin(GL_TRIANGLES);
    glVertex2f(tar.pos_center.x + 15, tar.pos_center.y + 20);
    glVertex2f(tar.pos_center.x + 20, tar.pos_center.y + 15);
    glVertex2f(tar.pos_center.x + 30, tar.pos_center.y + 45);
    glEnd();


    glBegin(GL_TRIANGLES);
    glVertex2f(tar.pos_center.x - 15, tar.pos_center.y + 20);
    glVertex2f(tar.pos_center.x - 20, tar.pos_center.y + 15);
    glVertex2f(tar.pos_center.x - 30, tar.pos_center.y + 45);
    glEnd();

}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //


    glClearColor(51.0 / 256, 150.0 / 256, 255.0 / 256, .0 / 256);
    glClear(GL_COLOR_BUFFER_BIT);


    backgrnd();
    clouddraw(CloudX1, CloudY1);
    clouddraw(CloudX2, CloudY2);
    drawgun();
    fire_arrow(fire);

    if (activeTimer) {
        CloudX1 -= 0.3;
        CloudX2 -= 0.3;
    }

    for (int i = 0; i < MAXTARGET; i++)
        targetdraw(target[i]);

    glutSwapBuffers();
}

void targetreset(int r)
{
    float k = r;
    float a = 450;

    target[r].color.r = rand() % 256;
    target[r].color.g = rand() % 256;
    target[r].color.b = rand() % 256;

    float yPos = rand() % 5 ? 30 : -30;
    target[r].pos_center = { a + k * 24, yPos * r };

    target[r].radius = RADIUSTARGET;

    target[r].speed = 5;

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    switch (key)
    {
    case ' ': spacebar = true;
        break;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);


    if (key == ' ')
        spacebar = false;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: {

        up = true;
        if (speed_arrow_state == SPEED2)
            speed_arrow_state = SPEED1;
        if (speed_arrow_state == SPEED3)
            speed_arrow_state = SPEED2;
    }break;
    case GLUT_KEY_DOWN: {

        down = true;
        switch (speed_arrow_state)
        {
        case SPEED1:
            speed_arrow_state = SPEED2;
            break;

        case SPEED2:
            speed_arrow_state = SPEED3;
            break;
        }

    }break;
    case GLUT_KEY_LEFT: {
        left = true;
    }break;
    case GLUT_KEY_RIGHT: {
        right = true;
    }break;

    case GLUT_KEY_F1:
    {
        if (counter <= 20 && counter >= 0)
        {
            activeTimer = !activeTimer;

        }

    } break;


    case GLUT_KEY_F5:
    {
        if (counter <= 0 && activeTimer == FALSE)
        {
            counter = 20;
            hit = 0;
            speed_arrow_state = SPEED1;
            CloudX1 = 360;
            CloudX2 = 280;
            fire.pos.y = -351;
            for (int s = 0; s < MAXTARGET; s++)
                targetreset(s);
            activeTimer = TRUE;
        }

    }break;

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}




//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: {
        up = false;
    }break;
    case GLUT_KEY_DOWN: {
        down = false;
    }break;
    case GLUT_KEY_LEFT: {
        left = false;
    }break;
    case GLUT_KEY_RIGHT: {
        right = false;
    }break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}


bool CollisionTEST(target_t trg, arrow_fire_t arr) {
    float DX = trg.pos_center.x - arr.pos.x;
    float DY = trg.pos_center.y - arr.pos.y;
    float D = sqrt(DX * DX + DY * DY);
    return D <= trg.radius;
}



#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.


    if (activeTimer)
    {
        counter -= 0.0165;

        if (counter <= 0)
        {
            counter = 0;
            activeTimer = FALSE;

        }


        if (right)
            gun.pos.x += 4;
        if (left)
            gun.pos.x -= 4;


        if (gun.pos.x + 60 >= winWidth / 2)
            gun.pos.x -= 4;
        if (gun.pos.x - 60 <= -winWidth / 2)
            gun.pos.x += 4;
    }

    if (fire.active == false && spacebar && activeTimer)
    {
        fire.pos = gun.pos;
        fire.active = true;
    }

    if (activeTimer) {
        switch (speed_arrow_state)
        {
        case SPEED1: {
            speedarrow = 5.0;
        } break;

        case SPEED2: {
            speedarrow = 10.0;
        }   break;

        case SPEED3: {
            speedarrow = 14.0;
        }break;

        }

    }

    if (fire.active && activeTimer) {


        fire.pos.y -= speedarrow;

        if (fire.pos.y < -350)
        {
            fire.active = false;
        }

        for (int k = 0; k < MAXTARGET; k++)
            if (CollisionTEST(target[k], fire))
            {
                hit++;
                fire.active = false;
                targetreset(k);

            }

    }

    if (activeTimer) {

        for (int f = 0; f < MAXTARGET; f++)
            target[f].pos_center.x -= target[f].speed;

        for (int n = 0; n < MAXTARGET; n++)
            if (target[n].pos_center.x < -530) {
                targetreset(n);
            }

    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < MAXTARGET; i++)
        targetreset(i);
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("The Bird Hunt-Emirhan Yel 22103407");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    srand(time(NULL));
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}