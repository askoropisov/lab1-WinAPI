#include <windows.h>

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API - Layer Simulator";

HPEN pen, old_pen;
HBRUSH brush, old_brush;

long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hdc;

    RECT r;
    GetClientRect(hWnd, &r);

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        //TextOut(hdc, 0, 0, L"œ »Ã— ÛÎËÚ!", 12);

        pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 40));
        old_pen = (HPEN)SelectObject(hdc, pen);
        brush = CreateSolidBrush(RGB(0, 0, 40));
        old_brush = (HBRUSH)SelectObject(hdc, brush);

        Rectangle(hdc, 0, 0, 4000, 4000);

        SelectObject(hdc, old_pen);
        SelectObject(hdc, old_brush);
        

        EndPaint(hWnd, &ps);

        DeleteObject(pen);
        DeleteObject(brush);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Can“t register window class!", L"Win32 API Test", NULL);
        return 1;
    }

    HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, NULL, NULL, hInstance, NULL);
    

    if (!hWnd) {
        MessageBox(NULL, L"Can“t create window!", L"Win32 API Test", NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
