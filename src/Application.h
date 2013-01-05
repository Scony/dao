#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtk/gtk.h>

#include "Configuration.h"
#include "GraphicalBoard.h"

class Application
{
  GtkWidget * window;
  GraphicalBoard * gBoard;
  GtkWidget* configuration_text;

  GtkTextBuffer* configuration_buffer;

  void initUI();
  void commitClicked();
 public:
  Application();
  ~Application();
  void run();
  static void onClick(GtkWidget *widget, gpointer user_data);
  static void onConfigurationChange(GObject* object,
				    gpointer user_data);
};
#endif
