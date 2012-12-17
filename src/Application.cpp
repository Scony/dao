#include "Application.h"
#include "Configuration.h"

Application::Application(int argc, char ** argv)
{
  gtk_init(&argc, &argv);

  /*Test GObject Configuration*/
  this->configuration = configuration_new();
  g_object_unref( G_OBJECT(this->configuration) );
  /*End GObject test*/

  this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW(this->window), "Dao");
  gtk_window_set_position (GTK_WINDOW(this->window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(this->window), FALSE);
  g_signal_connect(this->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  this->gBoard = new GraphicalBoard();

  GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_container_add(GTK_CONTAINER(window),vbox);

  GtkWidget * button = gtk_button_new_with_label("Some");
  gtk_box_pack_start(GTK_BOX(vbox),button,false,false,0);

  gtk_box_pack_start(GTK_BOX(vbox),gBoard->getDrawingArea(),false,false,0);

  //gtk_widget_add_events(dArea, GDK_BUTTON_PRESS_MASK); 
  //g_signal_connect(dArea,"button-press-event",G_CALLBACK(clicked),NULL);

  GtkWidget * view = gtk_text_view_new();
  gtk_widget_set_size_request(view,300,50);
  GtkTextBuffer * buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  gtk_text_buffer_set_text(buff, "#!self\nSTART=1\nMODE=HvsH\netc", -1);
  gtk_box_pack_start(GTK_BOX(vbox),view,false,false,0);

  button = gtk_button_new_with_label("Ci");
  gtk_box_pack_start(GTK_BOX(vbox),button,false,false,0);

  GtkWidget * statbar = gtk_statusbar_new();
  guint cx = gtk_statusbar_get_context_id(GTK_STATUSBAR(statbar),"Wtf ?");
  gtk_statusbar_push(GTK_STATUSBAR(statbar),cx,"values ...");
  gtk_box_pack_start(GTK_BOX(vbox),statbar,false,false,0);
}

Application::~Application()
{
  delete this->gBoard;
}

void Application::run()
{
  gtk_widget_show_all(this->window);
  gtk_main();
}
