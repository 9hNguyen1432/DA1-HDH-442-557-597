#include "syscall.h"

int main()
{
  int result = RandomNum();
  PrintNum(result);
  PrintString('/n');
  Halt();
  
}
