#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(50);

    phi = 0.0f;                         // horizontal angle
    psy = 0.0f;                         // vertical angle
    length = 5;                         // distance of camera from center
    ox = 0; oy = 0; oz = 1;             // center coordinates
    cx = cos(psy) * sin(phi) * length;  // camera coordinates
    cy = cos(psy) * cos(phi) * length;  // camera coordinates
    cz = sin(psy) * length;             // camera coordinates
}

void GLWidget::initializeGL(){
    setFormat(QGLFormat(QGL::DoubleBuffer));
    glDepthFunc(GL_LEQUAL);
}

void GLWidget::paintGL(){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/height, 0.01, 100000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h){
    width = w; height = h;
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Space:
        if (!keyButtonPressed)
            keyButtonPressed = 1;
        else
            keyButtonPressed = 0;
            break;
    case Qt::Key_Left:
        keyButtonPressed = 2;
        break;
    case Qt::Key_Right:
        keyButtonPressed = 3;
        break;
    case Qt::Key_Escape:
        keyButtonPressed = 1;
        break;
    case Qt::Key_F: // fullscreen for example
        keyButtonPressed = 1;
        break;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - mouseX;
    int dy = event->y() - mouseY;

    if(mouseButtonPressed == 1) {
        phi += dx * M_PI / 180.0f / 4.0f; // horizontal angle
        psy += dy * M_PI / 180.0f / 4.0f; // vertical angle

        psy = qMin(psy, M_PI/2.0);
        psy = qMax(psy, -M_PI/2.0);

        cx = cos(psy) * sin(phi) * length;
        cy = cos(psy) * cos(phi) * length;
        cz = sin(psy) * length;
    }

    if(mouseButtonPressed == 2) {
        ox -= cos(phi)*dx/20;
        oy += sin(phi)*dx/20;
        oz -= sin(psy)*dy/5;
    }

    mouseX = event->x();
    mouseY = event->y();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)  mouseButtonPressed=1;
    if(event->button()==Qt::RightButton) mouseButtonPressed=2;

    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)  mouseButtonPressed=0;
    if(event->button()==Qt::RightButton) mouseButtonPressed=0;

    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // TODO: rename numDegrees
    int numDegrees = event->delta() / 8;
    length += numDegrees / 15;
    length = qMax(length, 0.005);

    cx = cos(psy) * sin(phi) * length;
    cy = cos(psy) * cos(phi) * length;
    cz = sin(psy) * length;

    updateGL();
}
