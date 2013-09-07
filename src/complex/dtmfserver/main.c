#include "engine.h"
#include "finish.h"

/**
 * я главная функция и нечего меня по мелочам дергать
 */
int main(int argc, char** argv)
{

Prepare(argc, argv);

do
  {
  MainCycle();
  } while(1);

Finalize();
return(0);
}
