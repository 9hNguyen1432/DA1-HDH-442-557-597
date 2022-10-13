#include "syscall.h"

#define MAX_LENGHT_ARRAY 255

int main()
{
    int lenght;
    int arr[MAX_LENGHT_ARRAY + 1];
    int i,j,temp,sortOrder;

    do
    {
        PrintString("Nhap do dai cua mang (0 < length <= 255): ");
        lenght = ReadNum();

        if(lenght < 0 || lenght > MAX_LENGHT_ARRAY)
        {
            PrintString("Do dai phai thoa man 0 < length <= 255\n");
            PrintString("Moi nhap lai!\n");
        }

    } while (lenght < 0 || lenght >MAX_LENGHT_ARRAY);

    for(i=0;i<lenght;i++)
    {
        PrintString("arr[");
        PrintNum(i);
        PrintString("] = ");
        arr[i] = ReadNum();
    }

    do {
        PrintString("Nhap cach sap xep (1: Tang dan, 2: Giam dan): ");
        sortOrder = ReadNum();
        if (sortOrder != 1 && sortOrder != 2)
        {
            PrintString("Ban da nhap sai roi. Moi nhap lai!\n");
        }
    } while (sortOrder != 1 && sortOrder != 2);

    for(i = lenght; i > 0; i--)
    {
        for ( j = 0; j < i; j++)
        {
            if(sortOrder == 1)
            {
                if(arr[j]>arr[j+1])
                {
                    temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
            else
            {
                if(arr[j] < arr[j+1])
                {
                    temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
        
    }
    

    PrintString("Mang sau khi sap xep: ");
    for(i = 0; i < lenght;i++)
    {
        PrintNum(arr[i]);
        PrintChar(' ');
    }

    return 0;
}