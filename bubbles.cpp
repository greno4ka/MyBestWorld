#include "bubbles.h"

Bubbles::Bubbles(int numOfBubbles, double maxH)
{
    numberOfBubbles = numOfBubbles;
    maxHeight = maxH;
    *bubbles = new double*[numberOfBubbles];

    double radian = M_PI / 180.f; // for clearness and optimize

    for(int i = 0; i < numberOfBubbles; i++)
    {
        bubbles[i] = new double[4];

        int alpha = rand() % 360;
        float dist = 2.0f*rand() / RAND_MAX;
        bubbles[i][0] = dist * cos(alpha * radian);		// С…
        bubbles[i][1] = -4.8+10.0f*rand() / RAND_MAX;   // y
        bubbles[i][2] = dist * sin(alpha * radian);     // z
        bubbles[i][3] = 1.0f + 2.0f * rand() / RAND_MAX;// r
    }
}

void Bubbles::moveBubbles()
{
    glDisable(GL_COLOR_MATERIAL);

    GLfloat bsp_a[4] = { 0.3, 0.7, 0.1, 1.0 };
    GLfloat bsp_d[4] = { 0.3, 0.7, 0.1, 1.0 };
    GLfloat bsp_s[4] = { 0.3, 0.3, 0.1, 1.0 };
    GLfloat bsp_r = 128;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bsp_a);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bsp_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bsp_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &bsp_r);

    glPushMatrix();
    glEnable(GL_NORMALIZE);
    glScalef(0.2,0.2,0.2);

    for (int i = 0; i < numberOfBubbles; i++)
    {
        glTranslatef(bub[i][0], bub[i][1]+movieVar, bub[i][2]);
        glutSolidSphere(0.1,10,10);
        // TODO: - make smth with glut
        //       - maybe add different sizes?
    }
    glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);

    // postprocess of the movieVar
    movieVar += 0.1f;
    for (int i = 0; i < numberOfBubbles; i++)
        if (bubbles[i][1] + movieVar > maxHeight) bubbles[i][1] -=  9;
}
