#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QTimer>


#include <GL/glu.h>
#include <iostream>

#include <QWheelEvent>
#include <QKeyEvent>

#include <QVector>


class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    /// COMMON VARIABLES
    int mouseX, mouseY;         // for mouse tracking
    int mouseButtonPressed = 0; // 0 - mouse up, 1 - left button down, 2 - right button down
    int keyButtonPressed;       // on keyboard
    QTimer *timer;

    /// GLMainWindow parameters
    int width, height; // GLMainWindow sizes
    double
    phi,        // horizontal angle
    psy,        // vertical angle
    length,     // distance of camera from center
    ox, oy, oz, // center coordinates
    cx, cy, cz; // camera coordinates

 public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // GLWIDGET_H
