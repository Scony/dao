#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtk/gtk.h>

class Application
{
  //params
 public:
  Application(int argc, char ** argv);
  ~Application();
  void run();
};

#endif
