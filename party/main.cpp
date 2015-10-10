#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

MSG msg;
HWND hwnd;
HDC obraz;

class Bitmap
{
    private:
        HBITMAP bitmapa;
        HDC kafelek;
    public:
        Bitmap(const char* src)
        {
            bitmapa = (HBITMAP) LoadImage(NULL, src, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

            if(!bitmapa)
            {
                MessageBox(NULL, "ERROR!!! Not found graphics!\n", "Error", MB_ICONERROR);
                exit(0);
            }

            kafelek = CreateCompatibleDC(0);
            SelectObject(kafelek, bitmapa);
        }

        operator HDC()
        {
            return kafelek;
        }

        ~Bitmap()
        {
            DeleteDC(kafelek);
            DeleteObject(bitmapa);
        }
};

const int X = 20;
const int Y = 10;

const int SIZE_b = 50;

int map[Y][X]  = {{0}};

Bitmap kaf[] = {};

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    //OKNO
    WNDCLASSEX wnd;
    wnd.cbSize = sizeof( WNDCLASSEX );
    wnd.style = 0;
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.lpfnWndProc = WindowProcedure;
    wnd.hInstance = NULL;
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon = LoadIcon(NULL, IDI_ERROR);
    wnd.hbrBackground = (HBRUSH)1;
    wnd.lpszMenuName = NULL;
    wnd.lpszClassName = "Party";
    wnd.hIconSm = LoadIcon( NULL, IDI_INFORMATION);

    srand(time(0));

    RegisterClassEx(&wnd);
    hwnd = CreateWindow("Party", "Party", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX , 500, 250, SIZE_b * X + 6, SIZE_b * Y + 28, NULL, NULL, NULL, NULL);
    ShowWindow(hwnd, 1);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);

            HDC hdc = GetDC(hwnd);
            HDC hdcBufor;
            HBITMAP hbmBuf, hbmOldBuf;

            hdcBufor = CreateCompatibleDC(hdc);
            hbmBuf = CreateCompatibleBitmap(hdc, X*SIZE_b, Y*SIZE_b);
            hbmOldBuf = (HBITMAP)SelectObject(hdcBufor, hbmBuf);

            BitBlt(hdc, 0, 0, X*SIZE_b, Y*SIZE_b, hdcBufor, 0, 0, SRCCOPY);

            DeleteObject(hbmBuf);
            DeleteDC(hdcBufor);

            EndPaint(hwnd, & ps);
            break;
        }
        case WM_KEYDOWN:
        {
            switch(( int ) wParam )
            {
                case VK_ESCAPE:
                    DestroyWindow( hwnd );
                    break;
            }
        }
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
