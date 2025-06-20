#include <Windows.h>
#include <glut.h>
#include <cmath>

const float DEFAULT_RADIUS = 100.0f;
const float DEFAULT_ANGLE_X = 0.0f;
const float DEFAULT_ANGLE_Y = 0.0f;
float angleX = 0.0f;
float angleY = 0.0f;
float lastX = -100;
float lastY = -100;
bool leftButtonDown = false;
bool rightButtonDown = false;
float radius = 100.0f;
int width = 800;
int height = 600;

float tailAngle = 0.0f;
float tailSpeed = 4.5f;
float headAngle = 0.0f;
float headSpeed = 4.5f;
bool animationRunning = true;
float catMoveX = 0.0f;
bool walking = false;

bool jumping = false;
float jumpTime = 0.0f;

void drawTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, -10, z);
    glColor3f(0.55f, 0.27f, 0.07f);
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0, 5, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.5, 0.5, 10.0, 16, 16);
    glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.0f);
    glPushMatrix();
    glTranslatef(0, 11, 0);
    glutSolidSphere(3.5, 20, 20);
    glPopMatrix();
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void drawFlower(float x, float z) {
    glPushMatrix();
    glTranslatef(x, -19, z);
    glColor3f(1.0f, 0.4f, 0.7f);
    glutSolidSphere(0.5, 10, 10);
    glPopMatrix();
}

void drawCatRobot() {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();

    float jumpHeight = 0.0f;
    if (jumping) {
        jumpHeight = sinf(jumpTime) * 5.0f;
    }
    glTranslatef(catMoveX, jumpHeight, 0);

    glColor3f(1.0f, 0.9f, 0.7f);
    glPushMatrix();
    glutSolidSphere(4.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 7.5, 0);
    glRotatef(headAngle, 0, 1, 0);
    glColor3f(0.4f, 0.2f, 0.1f);
    glutSolidSphere(3.0, 20, 20);

    glPushMatrix();
    glTranslatef(-1.5, 3.5, 0.8);
    glRotatef(-20, 0, 0, 1);
    glScalef(0.5, 1.0, 0.5);
    glutSolidCube(2.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, 3.5, 0.8);
    glRotatef(20, 0, 0, 1);
    glScalef(0.5, 1.0, 0.5);
    glutSolidCube(2.0);
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.8, 2, 2);
    glutSolidSphere(0.6, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.8, 2, 2);
    glutSolidSphere(0.6, 12, 12);
    glPopMatrix();

    glPopMatrix(); 

    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0, -3.0, -3.5);
    glRotatef(tailAngle, 3, 0, 0);
    gluCylinder(quad, 0.9, 0.3, 9.0, 12, 12);
    glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = radius * sinf(angleY) * cosf(angleX);
    float camY = radius * sinf(angleX);
    float camZ = radius * cosf(angleY) * cosf(angleX);
    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(30, 40, -40);
    glutSolidSphere(20, 100, 100);
    glPopMatrix();

    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(50, -20, 50);
    glVertex3f(50, -20, -50);
    glVertex3f(-50, -20, -50);
    glVertex3f(-50, -20, 50);
    glEnd();

    drawTree(-30, -40);
    drawTree(0, -45);
    drawTree(30, -40);
    drawTree(-20, -35);
    drawTree(20, -35);

    for (float x = -45; x <= 45; x += 10) {
        for (float z = -45; z <= 45; z += 10) {
            drawFlower(x + (rand() % 5 - 2), z + (rand() % 5 - 2));
        }
    }

    drawCatRobot();

    glFlush();
}

void updateAnimation(int value) {
    if (animationRunning) {
        tailAngle += tailSpeed;
        if (tailAngle > 30.0f  tailAngle < -30.0f) tailSpeed = -tailSpeed;

        headAngle += headSpeed;
        if (headAngle > 15.0f  headAngle < -15.0f) headSpeed = -headSpeed;
    if (walking) {
            catMoveX += 0.1f;
            if (catMoveX > 40.0f) catMoveX = -40.0f;
        }

        if (jumping) {
            jumpTime += 0.1f;
            if (jumpTime > 3.14f) {
                jumping = false;
                jumpTime = 0.0f;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        leftButtonDown = (state == GLUT_DOWN);
    else if (button == GLUT_RIGHT_BUTTON)
        rightButtonDown = (state == GLUT_DOWN);
    lastX = x;
    lastY = y;
}

void motion(int x, int y) {
    float dx = (x - lastX) * 0.01f;
    float dy = (y - lastY) * 0.01f;
    if (leftButtonDown) {
        angleY += dx;
        angleX += dy;
        if (angleX > 1.5f) angleX = 1.5f;
        if (angleX < -1.5f) angleX = -1.5f;
    }
    if (rightButtonDown) {
        radius += dy * 20.0f;
        if (radius < -200.0f) radius = -200.0f;
        if (radius > 200.0f) radius = 200.0f;
    }
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        angleX = DEFAULT_ANGLE_X;
        angleY = DEFAULT_ANGLE_Y;
        radius = DEFAULT_RADIUS;
        glutPostRedisplay();
    }
    else if (key == 's') {
        animationRunning = !animationRunning;
    }
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        walking = true;
    }
    else if (key == GLUT_KEY_LEFT) {
        walking = false;
    }
    else if (key == GLUT_KEY_UP) {
        if (!jumping) {
            jumping = true;
            jumpTime = 0.0f;
        }
    }
}

int main() {
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Himalayan Cat Robot Scene");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(drawScene);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, updateAnimation, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light0_dir[] = { -1.0f, -1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);

    GLfloat light1_pos[] = { 50.0f, 50.0f, 50.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);


    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);

    glutMainLoop();
}