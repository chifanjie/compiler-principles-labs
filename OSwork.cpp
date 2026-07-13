#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
    char driveLetter = 'U';
    string drivePath = "\\\\.\\" +  string(1, driveLetter) + ":";

    //判断U盘是否存在
    if (GetDriveTypeA(drivePath.c_str()) == DRIVE_REMOVABLE) {
         cout << "U盘存在" <<  endl;

        //显示U盘的剩余容量
        ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
        if (GetDiskFreeSpaceExA(drivePath.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
             cout << "剩余容量: " << freeBytesAvailable.QuadPart << " 字节" <<  endl;

            //复制文件到U盘
             string sourcePath = "C:\\test.txt"; 
             string destinationPath = drivePath;
            if (CopyFileA((sourcePath + "\\test.txt").c_str(), (destinationPath + "\\test.txt").c_str(), FALSE)) {
                 cout << "文件复制成功" <<  endl;
            } else {
                 cerr << "文件复制失败" <<  endl;
            }

            //删除U盘上的文件
             string fileToDelete = drivePath + "\\test.txt";
            if (DeleteFileA(fileToDelete.c_str())) {
                 cout << "文件删除成功" <<  endl;
            } else {
                 cerr << "文件删除失败" <<  endl;
            }

            // (5) 禁止/开启U盘的使用
            DWORD error;
            HKEY hKey;
            LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\USBSTOR", 0, KEY_SET_VALUE, &hKey);
            if (result == ERROR_SUCCESS) {
                DWORD value = 4;  // 4禁止U盘，3启用U盘
                result = RegSetValueEx(hKey, "Start", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
                if (result == ERROR_SUCCESS) {
                     cout << "U盘使用已禁止/开启" <<  endl;
                } else {
                    error = GetLastError();
                     cerr << "无法更改注册表项：" << error <<  endl;
                }
                RegCloseKey(hKey);
            } else {
                error = GetLastError();
                 cerr << "无法打开注册表键：" << error <<  endl;
            }
        } else {
             cerr << "无法获取U盘信息" <<  endl;
        }
    } else {
         cerr << "U盘不存在" <<  endl;
    }

    return 0;
}
/*首先可以通过OS的API实现访问系统资源和硬件设备，就比如当前程序对U盘的一些操作，也可以对一些进程进行管理，
允许应用程序的创建与终止等，也可以对文件与IO进行操作，通过这些API允许应用程序在不同的操作系统上运行
能够创建功能丰富、高效和可靠的应用程序，充分利用底层硬件和操作系统的功能
*/
