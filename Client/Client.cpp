#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

void clearBuffer() {
    
    while (_kbhit())
        _getch();
}

int main()
{
    setlocale(LC_ALL, "ru");

    LPCTSTR mutexName = TEXT("PrinterMutex");
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexName);

    DWORD idThread;

    if (hMutex == NULL) {
        cout << "Принтер не запущен, не удалось открыть мьютекс" << endl;
        Sleep(5000);
        return 0;
    }

    cout << "Клиент" << endl;
    cout << "Введите 1 чтобы пользоваться принтером" << endl;
    cout << "Введите 2 чтобы выйти" << endl;
    cout << "Введите 3 чтобы поставить высокий приоритет" << endl;
    cout << "Введите 4 чтобы поставить средний приоритет" << endl;
    cout << "Введите 5 чтобы поставить фоновый приоритет" << endl;

    char input = '0';

    DWORD result;

    while (input != '2') {

        if (_kbhit()) {

            input = _getch();

            switch (input) {

            case '1':

                cout << "Отправлен запрос на использование принтера" << endl;

                result = WaitForSingleObject(hMutex, 1000);

                if (result == WAIT_OBJECT_0) {
                    cout << "Принтер работает..." << endl;

                    ReleaseMutex(hMutex);

                    WaitForSingleObject(hMutex, INFINITE);
                    cout << "Печать завершена" << endl;
                }
                else {
                    cout << "Принтер занят" << endl;
                }

                break;
            case '3':
                SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
                break;
            case '4':
                SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
                break;
            case '5':
                SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
                break;
            }

            clearBuffer();
        }
    }
        
    return 0;
}