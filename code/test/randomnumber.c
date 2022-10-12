#include "syscall.h"

int main()
{
  int result;
  result = RandomNum();
  PrintChar(result);
  PrintString('/n');
  Halt();
}
