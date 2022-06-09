#include "WinApp.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WinApp winapp(TEXT("Graphics"), hInstance, lpCmdLine, nCmdShow);
	if (winapp.create())
		return winapp.run();
	return 1;
}