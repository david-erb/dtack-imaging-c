#include <windows.h>
int PASCAL WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow)
{
  ExitWindowsEx(EWX_FORCE|EWX_REBOOT, 0L);
  return 0;
}
