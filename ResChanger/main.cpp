#include <iostream>
#include <windows.h>

using namespace std;

static HWND mainwindow;
static HWND hdbutton;
static HWND fhdbutton;

#define HDBTN (100)
#define FHDBTN (99)
#define FREQBTN (98)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define WINDOW_WIDTH 260
#define WINDOW_HEIGHT 230
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 30

bool SetScreenResolution(int nDisplay, int nWidth, int nHeight, float freq);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{

	WNDCLASSEX wndclass;
	ZeroMemory(&wndclass, sizeof(WNDCLASSEX));

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.lpfnWndProc = WindowProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = "Teszt";
	wndclass.hbrBackground = CreateSolidBrush(RGB(1, 33, 112));
	wndclass.hIcon = (HICON)LoadImage(hInstance, "Ricon.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
	wndclass.hIconSm = (HICON)LoadImage(hInstance, "Ricon.ico", IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);

	RegisterClassEx(&wndclass);

	mainwindow = CreateWindowEx(0, wndclass.lpszClassName, "ResChanger", WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	//fhdbutton = CreateWindow("BUTTON", "1080p 60Hz", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_RIGHTBUTTON, WINDOW_WIDTH-200, WINDOW_HEIGHT+(WINDOW_HEIGHT/2)+30, 100, 30, mainwindow, (HMENU)FHDBTN, (HINSTANCE)GetWindowLong(mainwindow, GWL_HINSTANCE), NULL);
	//hdbutton = CreateWindow("BUTTON", "720p 75Hz", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_RIGHTBUTTON,  WINDOW_WIDTH-200, WINDOW_HEIGHT+(WINDOW_HEIGHT/2) + 30, 100, 30, mainwindow, (HMENU)HDBTN, (HINSTANCE)GetWindowLong(mainwindow, GWL_HINSTANCE), NULL);

	int btn1_posx = (WINDOW_WIDTH / 2) - (BUTTON_WIDTH/2);
	int btn1_posy = (WINDOW_HEIGHT / 2) - 60;

	int btn2_posx = (WINDOW_WIDTH / 2) - (BUTTON_WIDTH / 2);
	int btn2_posy = (WINDOW_HEIGHT / 2);

	fhdbutton = CreateWindow("BUTTON", "1080p 60Hz", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_RIGHTBUTTON, btn1_posx, btn1_posy, BUTTON_WIDTH, BUTTON_HEIGHT, mainwindow, (HMENU)FHDBTN, (HINSTANCE)GetWindowLong(mainwindow, GWL_HINSTANCE), NULL);
	hdbutton = CreateWindow("BUTTON", "720p 75Hz", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_RIGHTBUTTON, btn2_posx, btn2_posy, BUTTON_WIDTH, BUTTON_HEIGHT, mainwindow, (HMENU)HDBTN, (HINSTANCE)GetWindowLong(mainwindow, GWL_HINSTANCE), NULL);

	ShowWindow(mainwindow, nCmdShow);
	UpdateWindow(mainwindow);

	int resx = GetSystemMetrics(SM_CXFULLSCREEN);
	int resy = GetSystemMetrics(SM_CYFULLSCREEN);

	if (resx == 1920)
	{
		EnableWindow(fhdbutton, false);
	}

	if (resx == 1280)
	{
		EnableWindow(hdbutton, false);
	}

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (GetAsyncKeyState(VK_F1))
		{
			SetScreenResolution(0, 1280, 720, 75.025f);
			EnableWindow(hdbutton, false);
			EnableWindow(fhdbutton, true);
			SetScreenResolution(1, 1280, 1024, 75.025f);
		}

		if (GetAsyncKeyState(VK_F2))
		{
			SetScreenResolution(0, 1920, 1080, 60.0f);
			EnableWindow(hdbutton, true);
			EnableWindow(fhdbutton, false);
			SetScreenResolution(1, 1280, 1024, 75.025f);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

bool SetScreenResolution(int nDisplay, int nWidth, int nHeight, float freq)
{
	//DWORD DispNum = 0;
	DISPLAY_DEVICE DisplayDevice;
	ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
	DisplayDevice.cb = sizeof(DisplayDevice);

	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(DEVMODE);
	
	
	if (EnumDisplayDevices(NULL, nDisplay, &DisplayDevice, 0))
	{
		if (!EnumDisplaySettings(DisplayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
			return false;

		devMode.dmPelsWidth = nWidth;
		devMode.dmPelsHeight = nHeight;
		devMode.dmDisplayFrequency = static_cast<DWORD>(freq);
		devMode.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH | DM_DISPLAYFREQUENCY;

		DWORD dwFlags = CDS_UPDATEREGISTRY | CDS_GLOBAL | CDS_FULLSCREEN;
		int a = ChangeDisplaySettingsEx(DisplayDevice.DeviceName, &devMode, NULL, dwFlags, NULL);
		if (a != DISP_CHANGE_SUCCESSFUL)
		{
			char str[133];
			sprintf_s(str, sizeof(str), "Hiba: %d", a);
			MessageBox(mainwindow, str, "ResChanger", MB_OK);
			return false;
		}
	}
	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == HDBTN) {
				if (SetScreenResolution(0, 1280, 720, 75.025f) == 1 && 
					SetScreenResolution(1, 1280, 1024, 75.025f) == 1)
				{
					EnableWindow(hdbutton, false);
					EnableWindow(fhdbutton, true);
				}
			}

			if (LOWORD(wParam) == FHDBTN) {
				if (SetScreenResolution(0, 1920, 1080, 60.0f) == 1 &&
					SetScreenResolution(1, 1280, 1024, 75.025f) == 1)
				{
					EnableWindow(hdbutton, true);
					EnableWindow(fhdbutton, false);
				}
			}
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}