#include "syscall.h"

int main() {
    PrintString("Thanh vien nhom:\n");
    PrintString("1: 20120442 Nguyen Huu Chinh\n");
    PrintString("2: 20120557 Tran Duy Quang\n");
    PrintString("3: 20120597 Nguyen Van Toai\n\n\n");

    PrintString("Chuong trinh Sort:\n");
    PrintString("Nhap lenh: \"./nachos -x bubblesort\" de chay chuong trinh");
    PrintString("Bat dau chuong trinh sort:\n");
    PrintString("B1: Nhap so phan tu cua mang (0 < length <= 255)\n");
    PrintString("B2: Nhap cac phan tu cua mang\n");
    PrintString("B3: Nhap cach sap xep (1: Tang dan, 2: Giam dan)\n");
    PrintString("Chuong trinh se in ra mang duoc sap xep theo thu tu da chon\n\n\n");

    PrintString("Chuong trinh ascii:\n");
    PrintString("Nhap lenh: \"./nachos -x ascii\" de chay chuong trinh");
    PrintString("Chuong trinh se in ra bang ASCII (In cac ky tu co the doc duoc)\n");

   Halt();
}