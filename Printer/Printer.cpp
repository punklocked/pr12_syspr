#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

BOOL printerActive = TRUE;
int timer = 0;
long long tick = GetTickCount64();

void PrintFun() {

	tick = GetTickCount64();
	cout << "Идет печать..." << endl;
	int time = (rand() % 11 + 5);
	timer = 0;

	for (int i = 0; i < time; i++) {

		if (i > 10) {

			timer = 0;
			cout << "Ошибка печати" << endl;
			Sleep(1000);
			cout << "Готов к печати" << endl;
			return;
		}

		timer += 1;
		cout << "Время в работе: " << timer << endl;
		Sleep(1000);
	}

	timer = 0;
	cout << "Печать прошла успешно" << endl;
	Sleep(1000);
	cout << "Готов к печати" << endl;
}

int main()
{
	setlocale(LC_ALL, "ru");

	srand(time(NULL));

	LPCTSTR mutexName = TEXT("PrinterMutex");
	HANDLE hMutex = CreateMutex(NULL, FALSE, mutexName);
	
	if (hMutex == NULL) {
		cout << "Не удалось создать мьютекс, код ошибки: " << GetLastError() << endl;
		Sleep(3000);
		return 1;
	}

	while (true) {

		DWORD result = WaitForSingleObject(hMutex, 100);

		if (result == WAIT_TIMEOUT) {

			WaitForSingleObject(hMutex, INFINITE);
			PrintFun();
			ReleaseMutex(hMutex);
		}
		else
			ReleaseMutex(hMutex);

		if ((GetTickCount64() - tick) >= 600000) {

			printerActive = FALSE;
			cout << "Принтер выключен, нажмите 1 чтобы включить" << endl;
			CloseHandle(hMutex);
		}

		if (printerActive == FALSE) {

			char input = _getch();

			if (input == '1') {

				hMutex = CreateMutex(NULL, FALSE, mutexName);

				if (hMutex == NULL) {
					cout << "Не удалось включить принтер" << endl;
					break;
				}

				cout << "Принтер готов к работе" << endl;
				printerActive = TRUE;
				tick = GetTickCount64();
			}
			else
				break;
		}

		Sleep(50);
	}
	CloseHandle(hMutex);
	CloseHandle(GetCurrentProcess());
	TerminateProcess(GetCurrentProcess(), 0);
}