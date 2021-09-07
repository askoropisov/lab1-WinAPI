#include <windows.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <TCHAR.H>
#include "atlstr.h"

using namespace std;

const wchar_t windowClass[] = _T("win32app");
const wchar_t windowTitle[] = _T("Win32API - Layer Simulator");

const int lyambda = 10;

OPENFILENAME ofn;
fstream file;

class Poly {
public:
    int firts_angle_x;
    int firts_angle_y;
    int second_angle_x;
    int second_angle_y;
};

class Metal {
public:
    int firts_angle_x;
    int firts_angle_y;
    int second_angle_x;
    int second_angle_y;
};

vector<Poly*> vec_poly;
vector<Metal*> vec_met;

vector<Poly*> user_rects_poly;
vector<Metal*> user_rects_met;

bool read_file(fstream &file) {
    int temp_mas_koords[4];

    string token;
    while (true) {
        if (file.eof()) break;
        else {
        file >> token;
        file >> temp_mas_koords[0];
        file >> temp_mas_koords[1];
        file >> temp_mas_koords[2];
        file >> temp_mas_koords[3];
        file >> token;
        if (token == "METAL") {
            Metal* p_met = new  Metal;
            vec_met.push_back(p_met);
            p_met->firts_angle_x=temp_mas_koords[0]*lyambda;
            p_met->firts_angle_y=temp_mas_koords[1]*lyambda;
            p_met->second_angle_x=(temp_mas_koords[2]+ temp_mas_koords[0]) * lyambda;
            p_met->second_angle_y=(temp_mas_koords[3]+ temp_mas_koords[1]) * lyambda;
        }
        if (token == "POLY") {
            Poly* p_poly = new  Poly;
            vec_poly.push_back(p_poly);
            p_poly->firts_angle_x = temp_mas_koords[0] * lyambda;
            p_poly->firts_angle_y = temp_mas_koords[1] * lyambda;
            p_poly->second_angle_x = (temp_mas_koords[2] + temp_mas_koords[0]) * lyambda;
            p_poly->second_angle_y = (temp_mas_koords[3] + temp_mas_koords[1]) * lyambda;
        }
        }
    }

    return true;
}

bool save_file(fstream &file) {

            for (auto element : user_rects_met) {
                file<<endl<<"RECT ";
                file<<element->firts_angle_x / lyambda;
                file<<" ";
                file<<element->firts_angle_y / lyambda;
                file << " ";
                file<<element->second_angle_x / lyambda;
                file << " ";
                file<<element->second_angle_y / lyambda;
                file << " "; 
                file << "METAL";
                //delete element;
            }
            for (auto element : user_rects_poly) {
                file << endl << "RECT ";
                file << element->firts_angle_x / lyambda;
                file << " ";
                file << element->firts_angle_y / lyambda;
                file << " ";
                file << element->second_angle_x / lyambda;
                file << " ";
                file << element->second_angle_y / lyambda;
                file << " ";
                file << "POLY";
                //delete element;
            }

    return true;
}

bool draw_metall = true, draw_poly = true;
WORD cursor_x_f = 0, cursor_y_f = 0, cursor_x_s = 0, cursor_y_s = 0;                        //short

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    HDC hdc;
    RECT r;

    GetClientRect(hWnd, &r);

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        cursor_x_f = LOWORD(lParam);
        cursor_y_f = HIWORD(lParam);
        WORD temt_x, temt_y;
        while(!WM_LBUTTONUP) InvalidateRect(hWnd, 0, true);
        
        break;
    case WM_LBUTTONUP:
        cursor_x_s = LOWORD(lParam);
        cursor_y_s = HIWORD(lParam);

        if (cursor_x_f!=cursor_x_s && cursor_y_f!= cursor_y_s){
            Metal* p_met = new Metal;
            user_rects_met.push_back(p_met);
            p_met->firts_angle_x=cursor_x_f;
            p_met->firts_angle_y=cursor_y_f;
            p_met->second_angle_x=cursor_x_s;
            p_met->second_angle_y=cursor_y_s;
            InvalidateRect(hWnd, 0, true);
        }

        break;
    case WM_RBUTTONDOWN:
        cursor_x_f = LOWORD(lParam);
        cursor_y_f = HIWORD(lParam);
        break;
    case WM_RBUTTONUP:
        cursor_x_s = LOWORD(lParam);
        cursor_y_s = HIWORD(lParam);

        if (cursor_x_f != cursor_x_s && cursor_y_f != cursor_y_s) {
            Poly* p_poly = new Poly;
            user_rects_poly.push_back(p_poly);
            p_poly->firts_angle_x = cursor_x_f;
            p_poly->firts_angle_y = cursor_y_f;
            p_poly->second_angle_x = cursor_x_s;
            p_poly->second_angle_y = cursor_y_s;
            InvalidateRect(hWnd, 0, true);   
        }

        break;
    case WM_CREATE: {
        HMENU hMenubar = CreateMenu();
        HMENU hMenu = CreateMenu();
        HMENU hMenu1 = CreateMenu();

        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu, _T("&File"));
        AppendMenu(hMenu, MF_STRING, 101, _T("&Open file"));
        AppendMenu(hMenu, MF_STRING, 102, _T("&Save As"));
        AppendMenu(hMenu, MF_SEPARATOR, 0, _T(""));
        AppendMenu(hMenu, MF_STRING, 103, _T("&Exit"));

       
        AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu1, _T("&Draw"));
        AppendMenu(hMenu1, MF_STRING, 201, _T("&Visible/invisible metall"));
        AppendMenu(hMenu1, MF_STRING, 202, _T("&Visible/invisible metall"));
        AppendMenu(hMenu1, MF_SEPARATOR, 0, _T(""));
        AppendMenu(hMenu1, MF_STRING, 203, _T("&Clear"));

        SetMenu(hWnd, hMenubar);
        break;
        }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 101: {                                                                                //Open file
            wchar_t szFileName[MAX_PATH] = _T("");
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Text Files(*.txt)\0 * .txt\0All Files(*.*)\0 * .*\0");
            ofn.lpstrFile = (LPWSTR)szFileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = _T("txt");

            if (GetOpenFileName(&ofn)) {
                string name_file;
                name_file = CW2A(ofn.lpstrFile);                                                    //LPWSTR to string for open file

                file.open(name_file, ios_base::in | ios_base::app);

                

                if (!read_file(file)) {
                    user_rects_met.clear();
                    user_rects_poly.clear();
                    InvalidateRect(hWnd, 0, true);
                    MessageBox(hWnd, _T("Unable to open the file"), _T("ERROR"), MB_OK | MB_ICONERROR);
                    return EXIT_FAILURE;
                }
                InvalidateRect(hWnd, 0, true);
                file.close();
                
            }
            break;
            }
        case 102: {                                                                                 //Save as
            const int result = MessageBox(hWnd, _T("Are you sure you want to save the changes?"), _T("Save As"), MB_YESNOCANCEL | MB_ICONQUESTION | MB_APPLMODAL);
            switch (result)
            {
            case IDYES:
            {
                HINSTANCE hInstance;
                char buf[255] = "\0";
                ofn.lpstrFilter = _T("Text Files(*.txt)\0 * .txt\0All Files(*.*)\0 * .*\0");
                ofn.lpstrDefExt = _T("txt");
                char cCustomFilter[256] = "\0\0";
                int nFilterIndex = 0;
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.nMaxCustFilter = 256;
                ofn.nFilterIndex = nFilterIndex;
                wchar_t szFileName[MAX_PATH] = _T("");
                ofn.lpstrFile = (LPWSTR)szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.lpstrTitle = 0;
                ofn.Flags = OFN_FILEMUSTEXIST;

                if (GetSaveFileName(&ofn))
                {
                    string name_file;
                    name_file = CW2A(ofn.lpstrFile);
                    file.close();
                    file.open(name_file, ios_base::in | ios_base::app);
                    if (save_file(file)) {
                        MessageBox(hWnd, _T("The file was saved successfully"), _T("Save As"), MB_OK | MB_ICONEXCLAMATION);
                    }
                }
                
                break;
            }
            case IDNO:
                user_rects_met.clear();
                user_rects_poly.clear();
                InvalidateRect(hWnd, 0, true);
                break;
            case IDCANCEL:
                break;
            }
            break;
            }
        case 103:
            PostMessage(hWnd, WM_QUIT, 0, 0);
            break;
        case 201:                            //Visible/invisible metall
            if (draw_metall == true) draw_metall = false;
            else if (draw_metall == false) draw_metall = true;
            InvalidateRect(hWnd, 0, true);
            break;
        case 202:                            //Visible/invisible poly
            if (draw_poly == true) draw_poly = false;
            else if (draw_poly == false) draw_poly = true;
            InvalidateRect(hWnd, 0, true);
            break;
        case 203:
            user_rects_met.clear();
            user_rects_poly.clear();
            vec_met.clear();
            vec_poly.clear();
            InvalidateRect(hWnd, 0, true);
            break;
        default:
            break;
        }
        break;

    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN old_pen = (HPEN)SelectObject(hdc, pen);
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH old_brush = (HBRUSH)SelectObject(hdc, brush);

        //set black client rect
        FillRect(hdc, &r, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

        //draw pionts field
        const COLORREF grey = 0x00444444;
        for (int i = 0; i < r.right; i+= lyambda) {
            for (int j = 0; j < r.bottom; j+= lyambda) {
                SetPixel(hdc, i, j, grey);
            }
        }

        //draw metall in file
        if (draw_metall == true) {
            pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
            old_pen = (HPEN)SelectObject(hdc, pen);
            brush = CreateSolidBrush(RGB(0, 40, 255));
            old_brush = (HBRUSH)SelectObject(hdc, brush);

            for ( unsigned int i = 0; i < vec_met.size(); i++) {
                Rectangle(hdc, vec_met[i]->firts_angle_x, vec_met[i]->firts_angle_y, vec_met[i]->second_angle_x, vec_met[i]->second_angle_y);
            }

            //draw user metall
            for (unsigned int i = 0; i < user_rects_met.size(); i++) {
                Rectangle(hdc, user_rects_met[i]->firts_angle_x, user_rects_met[i]->firts_angle_y, user_rects_met[i]->second_angle_x, user_rects_met[i]->second_angle_y);
            }

            SelectObject(hdc, old_pen);
            SelectObject(hdc, old_brush);
        }

        //draw poly in file
        if (draw_poly == true) {
            pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
            old_pen = (HPEN)SelectObject(hdc, pen);
            brush = CreateSolidBrush(RGB(250, 80, 0));
            old_brush = (HBRUSH)SelectObject(hdc, brush);

            for (unsigned int i = 0; i < vec_poly.size(); i++) {
                Rectangle(hdc, vec_poly[i]->firts_angle_x, vec_poly[i]->firts_angle_y, vec_poly[i]->second_angle_x, vec_poly[i]->second_angle_y);
            }
            //draw user poly
            for (unsigned int i = 0; i < user_rects_poly.size(); i++) {
                Rectangle(hdc, user_rects_poly[i]->firts_angle_x, user_rects_poly[i]->firts_angle_y, user_rects_poly[i]->second_angle_x, user_rects_poly[i]->second_angle_y);
            }

            SelectObject(hdc, old_pen);
            SelectObject(hdc, old_brush);
        }
        
        EndPaint(hWnd, &ps);
        DeleteObject(pen);
        DeleteObject(brush);
        
        break;
        }
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
    file.close();
    return msg.wParam;
}
