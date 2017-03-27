#ifndef MYGLWINDOW_H
#define MYGLWINDOW_H
#include <QtOpenGL\qglwidget>

class MyGLWindow : public QGLWidget
{

protected:
  void initializeGL();
  void paintGL();

};

#endif

