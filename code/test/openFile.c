#include "syscall.h"

int main() {
    char fileName[] = "abc";
    int id;
    if ((id = Open(fileName, 0)) != -1) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" opened successfully!\n");
        PrintString("Id: ");
        PrintNum(id);
        PrintString("\n");

        Close(id);
        PrintString("Close successfully!\n");
    } else
    {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" opened failed!\n");

    }
}