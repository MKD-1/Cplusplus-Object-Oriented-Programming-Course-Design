#include "Application.h"

signed main() {
	//system("chcp 65001>nul");
	system("chcp 936>nul");
	std::wcout.imbue(std::locale("chs"));
	std::wcin.imbue(std::locale("chs"));
	HRManagementSystem management;
	bool MyDone = false;
	management.MainLoop();
	return 0;
}