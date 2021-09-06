#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <TCHAR.H>

using namespace std;

const wchar_t windowClass[] = _T("win32app");
const wchar_t windowTitle[] = _T("Win32API - Layer Simulator");


HMENU hMenubar = nullptr;
HMENU hMenu = nullptr;
OPENFILENAME ofn;
ifstream infile;

enum color {
    red,
    blue,
};

class Poly {
public:
    int firts_angle_x;
    int firts_angle_y;
    int second_angle_x;
    int second_angle_y;
    color:red;
};

class Metal {
public:
    int firts_angle_x;
    int firts_angle_y;
    int second_angle_x;
    int second_angle_y;
    color:blue;
};

vector<Poly> vec_poly;
vector<Metal> vec_met;

bool read_file(ifstream file) {

    string token;
    while (true) {
        file >> token;
        if (token == "RECT") continue;
    }

    return true;
}

bool save_file(ifstream& file) {

    return true;
}

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hdc;

    RECT r;

    GetClientRect(hWnd, &r);

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        hMenubar = CreateMenu();
        hMenu = CreateMenu();

        //AppendMenu(hMenu, MF_STRING, 4131, L"&Quit");
        AppendMenu(hMenu, MF_STRING, 100, _T("&Open file"));
        AppendMenu(hMenu, MF_STRING, 2, _T("&Save As"));
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, _T("&File"));


        SetMenu(hWnd, hMenubar);

        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 100: {

            wchar_t szFileName[MAX_PATH] = L"";
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Text Files(*.txt)\0 * .txt\0All Files(*.*)\0 * .*\0");
            ofn.lpstrFile = (LPWSTR)szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = _T("txt");


            if (GetOpenFileName(&ofn)) {
                MessageBox(hWnd, ofn.lpstrFile, _T("File name"), MB_OK);
               /* string token;
                while (true) {
                    ofn.lpstrFile >> token;
                    if (token == "RECT") continue;
                }*/
            }


            /*Metal met1;
            met1.firts_angle_x = 50;
            met1.firts_angle_y = 50;
            met1.second_angle_x = 250;
            met1.second_angle_y = 300;
            vec_met.push_back(met1);*/

            break;
        }
        case 2:
            save_file(infile);
            break;
        default:
            break;
        }
        break;
    case WM_PAINT:

        HPEN pen, old_pen;
        HBRUSH brush, old_brush;

        //set black client rect
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &r, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));


        //TextOut(hdc, 0, 0, L"œ »Ã— ÛÎËÚ!", 12);


        //draw metall in file
        pen = CreatePen(PS_SOLID, 1, RGB(80, 0, 250));
        old_pen = (HPEN)SelectObject(hdc, pen);
        brush = CreateSolidBrush(RGB(80, 0, 250));
        old_brush = (HBRUSH)SelectObject(hdc, brush);

        for (int i = 0; i < vec_met.size(); i++) {
            Rectangle(hdc, vec_met[i].firts_angle_x, vec_met[i].firts_angle_y, vec_met[i].second_angle_x, vec_met[i].second_angle_y);
        }

        SelectObject(hdc, old_pen);
        SelectObject(hdc, old_brush);

        //draw poly in file
        pen = CreatePen(PS_SOLID, 1, RGB(250, 80, 0));
        old_pen = (HPEN)SelectObject(hdc, pen);
        brush = CreateSolidBrush(RGB(250, 80, 0));
        old_brush = (HBRUSH)SelectObject(hdc, brush);

        for (int i = 0; i < vec_poly.size(); i++) {
            Rectangle(hdc, vec_poly[i].firts_angle_x, vec_poly[i].firts_angle_y, vec_poly[i].second_angle_x, vec_poly[i].second_angle_y);
        }

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
        MessageBox(NULL, _T("Can“t register window class!"), _T("Win32 API Test"), NULL);
        return 1;
    }

    HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800, NULL, NULL, hInstance, NULL);


    if (!hWnd) {
        MessageBox(NULL, _T("Can“t create window!"), _T("Win32 API Test"), NULL);
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
