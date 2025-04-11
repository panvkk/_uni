// lesson9-2.cpp : Defines the entry point for the application.
//
#define _USE_MATH_DEFINES // Для использования M_PI
#include <cmath>
#include <vector>
#include "framework.h"
#include "lesson9-2.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LESSON92, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LESSON92));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LESSON92));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохраняем дескриптор экземпляра в глобальной переменной

    // Получаем размеры экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Создаем окно без рамки в полноэкранном режиме
    HWND hWnd = CreateWindowW(
        szWindowClass,   // имя класса окна
        szTitle,         // заголовок окна
        WS_POPUP,        // стиль окна: WS_POPUP означает окно без рамок
        0,               // позиция X
        0,               // позиция Y
        screenWidth,     // ширина окна = ширине экрана
        screenHeight,    // высота окна = высоте экрана
        nullptr,         // родительское окно отсутствует
        nullptr,         // меню отсутствует
        hInstance,       // дескриптор приложения
        nullptr          // дополнительные параметры отсутствуют
    );

    if (!hWnd)
    {
        return FALSE;
    }

    // Отображаем окно
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Получаем размеры клиентской области
        RECT rect;
        GetClientRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        // Заливаем фон чёрным
        FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Создаём белое перо для рисования (оси и графика)
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        // Задаём прозрачный фон для текста и белый цвет текста
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));

        // Рисуем график функции sin(x)
        // Отображаем интервал x ∈ [–π, π] и y ∈ [–1, 1]
        std::vector<POINT> sinPoints;
        for (int x = 0; x < width; x++) {
            // Преобразуем пиксель по горизонтали в математическое значение x:
            // x = -π + (2π * x / (width - 1))
            double xVal = -M_PI + (2.0 * M_PI * x) / (width - 1);
            double yVal = sin(xVal);
            // Преобразуем y из диапазона [-1,1] в координату пикселя:
            // y при 1 должен давать верх окна (0), а при -1 – низ (height)
            int yPixel = (int)(height / 2 - yVal * (height / 2));
            POINT pt = { x, yPixel };
            sinPoints.push_back(pt);
        }
        if (!sinPoints.empty()) {
            Polyline(hdc, sinPoints.data(), static_cast<int>(sinPoints.size()));
        }

        // Рисуем оси координат:
        // Горизонтальная ось (ось X) проходит по центру окна (y = height/2)
        MoveToEx(hdc, 0, height / 2, NULL);
        LineTo(hdc, width, height / 2);
        // Вертикальная ось (ось Y) проходит по центру (x = width/2)
        MoveToEx(hdc, width / 2, 0, NULL);
        LineTo(hdc, width / 2, height);

        // Рисуем стрелки на концах осей:
        int arrowSize = 10;
        // Стрелка для правого конца оси X:
        POINT xArrowRight[3] = {
            { width, height / 2 },
            { width - arrowSize, height / 2 - arrowSize / 2 },
            { width - arrowSize, height / 2 + arrowSize / 2 }
        };
        Polygon(hdc, xArrowRight, 3);
        
        // Стрелка для верхнего конца оси Y:
        POINT yArrowTop[3] = {
            { width / 2, 0 },
            { width / 2 - arrowSize / 2, arrowSize },
            { width / 2 + arrowSize / 2, arrowSize }
        };
        Polygon(hdc, yArrowTop, 3);

        // Вывод меток координат:
        // Используем функцию TextOutW (т.к. проект Unicode)
        // Для оси X: метки «–π», «0», «π»
        TextOutW(hdc, 5, height / 2 + 5, L"-π", 2);
        TextOutW(hdc, 5, height / 2 + 5, L"-π/2", 2);
        TextOutW(hdc, width / 2 - 8, height / 2 + 5, L"0", 1);
        TextOutW(hdc, width - 30, height / 2 + 5, L"π", 1);
        // Для оси Y: метки «1» (вверху) и «–1» (внизу)
        TextOutW(hdc, width / 2 + 5, 5, L"1", 1);
        TextOutW(hdc, width / 2 + 5, height - 20, L"-1", 2);

        // Возвращаем старое перо и очищаем ресурсы
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN: // Закрыть окно по нажатию ESC
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
