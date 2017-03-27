#include <QtWidgets\Qapplication.h>
#include <QtWidgets\qwidget.h>
#include "MyGLWindow.h"

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);
  MyGLWindow myGLWindow;
  myGLWindow.show();
  return application.exec();
}
