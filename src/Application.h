#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtk/gtk.h>

#include "GraphicalBoard.h"

class Application
{
  GtkWidget * window;
  GraphicalBoard * gBoard;
 public:
  Application(int argc, char ** argv);
  ~Application();
  void run();
};

#endif
