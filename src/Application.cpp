#include <string.h>

#include "defines.h"
#include "Application.h"
#include "Configuration.h"

Application::Application(int argc, char ** argv)
{
  gchar* configuration_text;
  gsize configuration_text_len;

  gtk_init(&argc, &argv);

  /*Set up configuration GObject*/
  this->configuration = configuration_new();
  configuration_read_from_file(this->configuration,
			       CONFIG_FILE);
  g_signal_connect(this->configuration, "changed",
		   G_CALLBACK(onConfigurationChange),
		   (gpointer)this);

  this->initUI();
  
  configuration_text = configuration_get_data(this->configuration, 
			 &configuration_text_len, NULL);
  gtk_text_buffer_set_text(this->configuration_buffer,
			   configuration_text, 
			   configuration_text_len);
}

Application::~Application()
{
  g_object_unref( G_OBJECT(this->configuration) );
  delete this->gBoard;
}

void Application::initUI()
{
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

  /*GtkTextView configuration_text*/
  this->configuration_text = gtk_text_view_new();
  gtk_widget_set_size_request(this->configuration_text,300,50);
  this->configuration_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(this->configuration_text));
  gtk_text_buffer_set_text(this->configuration_buffer, "[Game]\nfirst_move=Player2\n", -1);
  gtk_box_pack_start(GTK_BOX(vbox),this->configuration_text,false,false,0);

  button = gtk_button_new_with_label("Ci");
  gtk_box_pack_start(GTK_BOX(vbox),button,false,false,0);
  g_print("%uld\n", (unsigned long)this);
  g_signal_connect(button,"clicked",G_CALLBACK(onClick),(gpointer)this);

  GtkWidget * statbar = gtk_statusbar_new();
  guint cx = gtk_statusbar_get_context_id(GTK_STATUSBAR(statbar),"Wtf ?");
  gtk_statusbar_push(GTK_STATUSBAR(statbar),cx,"values ...");
  gtk_box_pack_start(GTK_BOX(vbox),statbar,false,false,0);
}  

void Application::run()
{
  gtk_widget_show_all(this->window);
  gtk_main();
}

void Application::commitClicked()
{
  gsize length;
  gchar* data;

  GtkTextIter begin;
  GtkTextIter end;

  gtk_text_buffer_get_iter_at_offset(this->configuration_buffer,
  				     &begin,
  				     0);
  gtk_text_buffer_get_iter_at_offset(this->configuration_buffer,
  				     &end,
  				     -1);
  data = gtk_text_buffer_get_text(this->configuration_buffer,
  				  &begin, &end, FALSE);
  length = strlen(data)+1;

  configuration_read_from_data(this->configuration, data, length);
}

void Application::onClick(GtkWidget *widget, gpointer user_data)
{
  Application* application = (Application*)user_data;
  application->commitClicked();
}

void Application::onConfigurationChange(GObject* object,
					gpointer user_data)
{
  Application* self;
  gsize length;
  gchar* config_data;

  self  = (Application*)user_data;
  config_data = configuration_get_data(self->configuration,
				       &length, NULL);

  //get rid of additional newline
  config_data[length--] = 0;
  
  gtk_text_buffer_set_text(self->configuration_buffer,
			   config_data, length);
}
