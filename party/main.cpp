#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

MSG msg;
HWND hwnd;
HDC obraz;

struct p
{
    int x;
    int y;
}ludzik;

void new_game()
{
    ludzik.x = 200;
    ludzik.y = 300;
}

int shoot_v;
int ludzik_size = 82;

struct shoot
{
    int x, y;
};

std::vector <shoot>strzaly;

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

const int X = 1000;
const int Y = 500;

int map[Y][X]  = {{0}};

Bitmap kaf[] = {"grafika/tlo.bmp", "grafika/ludzik.bmp", "grafika/strzal.bmp"};

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
    hwnd = CreateWindow("Party", "Party", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX , 300, 250, X, Y, NULL, NULL, NULL, NULL);
    ShowWindow(hwnd, 1);

    msg.message = WM_NULL;
    new_game();
    int last = GetTickCount();

    while (msg.message != WM_QUIT)
    {
       if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
       {
          TranslateMessage (&msg);
          DispatchMessage (&msg);
       }
       else
       {
            //logika


            for(int i = 0; i < strzaly.size(); i++)
            {
                if(strzaly[i].y == 0)
                    strzaly.erase(strzaly.begin()+i);
                else
                    strzaly[i].y -=2;
            }

            SendMessage(hwnd, WM_PAINT, 0, 0);
       }
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
            hbmBuf = CreateCompatibleBitmap(hdc, X, Y);
            hbmOldBuf = (HBITMAP)SelectObject(hdcBufor, hbmBuf);

        //wyswietlanie gry

            BitBlt(hdcBufor, 0, 0, X, Y, kaf[0], 0, 0, SRCCOPY);

            //ludzik
            BitBlt(hdcBufor, ludzik.x, ludzik.y, X, Y, kaf[1], 0, 0, SRCCOPY);

            //strzaly
            for(int i = 0; i < strzaly.size(); i++)
                BitBlt(hdcBufor, strzaly[i].x, strzaly[i].y, 20, 20, kaf[2], 0, 0, SRCCOPY);

        //koniec wyswietlania

            BitBlt(hdc, 0, 0, X, Y, hdcBufor, 0, 0, SRCCOPY);

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
                case VK_LEFT:
                    ludzik.x -= 10;
                    break;
                case VK_RIGHT:
                    ludzik.x += 10;
                    break;
            }

            switch(( int ) wParam )
            {
                case VK_SPACE:
                    shoot new_s;
                    new_s.x = ludzik.x;
                    new_s.y = ludzik.y;

                    strzaly.push_back(new_s);
                    break;
            }

            SendMessage(hwnd, WM_PAINT, wParam, lParam);
        }
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
