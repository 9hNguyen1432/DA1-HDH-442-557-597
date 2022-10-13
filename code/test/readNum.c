#include "syscall.h"

int main() {
    int n;
    PrintString("Press Your Number: \0");
    n = ReadNum();
    PrintString("Your Number: \0");
    PrintNum(n);
    PrintString("\n");
    Halt();
}
