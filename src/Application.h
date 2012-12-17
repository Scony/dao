#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtk/gtk.h>

#include "Configuration.h"
#include "GraphicalBoard.h"

class Application
{
  GtkWidget * window;
  GraphicalBoard * gBoard;
  Configuration* configuration;
 public:
  Application(int argc, char ** argv);
  ~Application();
  void run();
};

#endif
