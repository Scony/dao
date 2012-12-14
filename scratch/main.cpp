#include <gtk/gtk.h>

static void do_drawing(cairo_t *cr)
{
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 2.5);

  cairo_move_to(cr,0,0);
  cairo_line_to(cr,300,200);

  cairo_stroke(cr);    
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event,
			gpointer user_data)
{
  g_print("%f %f\n",event->x,event->y);
  // if(event->button == 1)
  //   {
  //     glob.coordx[glob.count] = event->x;
  //     glob.coordy[glob.count++] = event->y;
  //   }

  // if(event->button == 3)
  //   {
  //     gtk_widget_queue_draw(widget);
  //   }

  return TRUE;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  g_print("draw!\n");
  do_drawing(cr);
  cairo_destroy(cr);

  return false;
}

int main(int argc, char ** argv)
{
  gtk_init(&argc, &argv);

  GtkWidget * window;
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  // gtk_widget_set_size_request(window,300,200);
  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

  GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_container_add(GTK_CONTAINER(window),vbox);

  GtkWidget * button = gtk_button_new_with_label("Some");
  gtk_box_pack_start(GTK_BOX(vbox),button,false,false,0);

  GtkWidget * darea = gtk_drawing_area_new();
  gtk_widget_set_size_request(darea,300,200);
  g_signal_connect(G_OBJECT(darea),"draw",G_CALLBACK(on_draw_event),NULL);
  gtk_box_pack_start(GTK_BOX(vbox),darea,false,false,0);

  gtk_widget_add_events(darea, GDK_BUTTON_PRESS_MASK); 
  g_signal_connect(darea,"button-press-event",G_CALLBACK(clicked),NULL);

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

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
