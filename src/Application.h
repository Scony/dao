#ifndef APPLICATION_H
#define APPLICATION_H

#include <gtk/gtk.h>
#include <glibmm/refptr.h>
#include <gtkmm/window.h>

#include "Configuration.h"
#include "GraphicalBoard.h"

class Application : public Gtk::Window
{
  GraphicalBoard * m_gBoard;
  Glib::RefPtr<Gtk::TextBuffer> m_configuration_buffer;

  void initUI();
  void onCommitClicked();
  void onConfigurationChanged();
 public:
  Application();
  ~Application();
};
#endif
