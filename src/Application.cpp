#include "Application.h"

Application::Application(int argc, char ** argv)
{
  gtk_init(&argc, &argv);
}

Application::~Application()
{
}

void Application::run()
{
  gtk_main();
}
