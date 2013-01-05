#include <iostream>

#include "Application.h"
#include "Configuration.h"
#include "DaoException.h"
#include "defines.h"

using namespace std;

int main(int argc, char ** argv)
{
  gtk_init(&argc, &argv);
  try
    {
      Configuration::getInstance().readFile(CONFIG_FILE);
    }
  catch(DaoException e)
    {
      cout << "Error reading configuration " << e.what() << endl;
    }

  Application application;
  application.run();
  return 0;
}
