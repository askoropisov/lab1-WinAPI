#include <windows.h>

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API - Ïðèìåð 2";

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

      TextOut(hdc, 0, 0, L"ÏÊÈÌÑ ðóëèò!", 12);

      Rectangle(hdc, 50, 50, 250, 150);

      EndPaint(hWnd, &ps);
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
    MessageBox(NULL, L"CanÒt register window class!", L"Win32 API Test", NULL);
    return 1;
  }

  HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

  if (!hWnd) {
    MessageBox(NULL, L"CanÒt create window!", L"Win32 API Test", NULL);
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
