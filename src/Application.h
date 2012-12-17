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
  GtkWidget* configuration_text;

  GtkTextBuffer* configuration_buffer;

  void commitClicked();
 public:
  Application(int argc, char ** argv);
  ~Application();
  void run();
  static void onClick(GtkWidget *widget, gpointer user_data);
};

#endif
