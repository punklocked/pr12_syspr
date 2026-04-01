#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;


struct ClientInfo {

    PROCESS_INFORMATION piClient;
};

int main()
{
    setlocale(LC_ALL, "ru");

    wchar_t printerProcess[255] = L"C:\\Users\\st310-10\\Desktop\\pr12_syspr-main\\x64\\Debug\\Printer.exe";
    wchar_t clientProcess[255] = L"C:\\Users\\st310-10\\Desktop\\pr12_syspr-main\\x64\\Debug\\Client.exe";

    STARTUPINFO si;
    PROCESS_INFORMATION piPrinter;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(NULL, printerProcess, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piPrinter)) {
        cout << "Не удалось запустить принтер, код ошибки: " << GetLastError() << endl;
        return 1;
    }

    char input = '0';

    cout << "Нажмите 1 чтобы создать нового клиента\nНажмите 2 чтобы выйти" << endl;

    ClientInfo* clientInfo = NULL;
    int clientCount = 0;
    int clientCapacity = 0;

    while (input != '2') {

        input = _getch();

        if (input == '1') {

            PROCESS_INFORMATION piClient;
            ZeroMemory(&piClient, sizeof(piClient));

            if (!CreateProcess(NULL, clientProcess, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piClient)) {
                cout << "Не удалось создать клиента, код ошибки: " << GetLastError() << endl;
                break;
            }

            if (clientCount >= clientCapacity) {

                ClientInfo* newClientInfo = new ClientInfo[clientCapacity + 10];

                for (int i = 0; i < clientCount; i++)
                    newClientInfo[i] = clientInfo[i];

                clientInfo = newClientInfo;
                clientCapacity += 10;
            }

            clientInfo[clientCount].piClient = piClient;
            clientCount++;
        }
    }

    for (int i = 0; i < clientCount; i++) {

        if (clientInfo[i].piClient.hProcess) {

            TerminateProcess(clientInfo[i].piClient.hProcess, 0);
            CloseHandle(clientInfo[i].piClient.hProcess);
        }
        
        if (clientInfo[i].piClient.hThread)
            CloseHandle(clientInfo[i].piClient.hThread);
    }

    if (piPrinter.hProcess) {

        TerminateProcess(piPrinter.hProcess, 0);
        CloseHandle(piPrinter.hProcess);
    }
    
    if (piPrinter.hThread)
        CloseHandle(piPrinter.hThread);

}