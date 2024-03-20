#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
using namespace std;

/*
This program uses the windows class INPUT contained in the windows.h header file to mimic user keyboard inputs. 

WARNING: improper use of this may cause issues with your machine
*/

// FILE TO READ COMMANDS FROM
string cmdfile = "examplecommandlist.txt";

//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#define winlen 139

// windows keys
string winkeylist[winlen] = { 
	"LMB", "RMB", "CTRLBRKPROC", "MMB", "X1MB", "X2MB", "BACKSPACE", "TAB", "0xA", "0xB", "CLR", "ENTER", "SHIFT", "CTRL", "ALT", "PAUSE", "CAPSLOCK", "SPACE", "PAGEUP", 
	"PAGE DOWN", "END", "HOME", "LEFTARROW", "UPARROW", "RIGHTARROW", "DOWN ARROW", "SELECT", "PRINT", "EXECUTE", "PRINT SCREEN", "INSERT", "DEL", "HELP", "0", "1", "2", 
	"3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "LEFT_WIN", 
	"RIGHTWIN", "APPKEY", "CPUSLEEP", "NUM0", "NUM1", "NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", "MULT", "ADD", "SEP", "SUB", "DECIMAL", 
	"DIV", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "NUMLOCK", 
	"SCROLLLOCK", "LEFTSHIFT", "RIGHTSHIFT", "LEFTCTRL", "RIGHTCTRL", "LEFTALT", "RIGHTALT", "BROWSERBACK", "BROWSERFORWARD", "BROWSERREFRESH", "BROWSERSTOP", "BROWSERSEARCH", 
	"BROWSERFAVORITE", "BROWSER START", "VOLMUTE", "VOLDOWN", "VOLUP", "NEXTTRACK", "PREVIOUSTRACK", "STOPMEDIA", "PLAYMEDIA", "STARTMAIL", "SELECT MEDIA", "STARTAPP1", "STARTAPP2"};
const int wincodelist[winlen] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x10, 0x11, 0x12, 0x13, 0x14, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
	0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 
	0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 
	0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x90, 0x91, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 
	0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7 };

// wincmd structure and function
struct wincmd {
	int wincode;	// windows character code
	int type;		// input type: 1 = keyboard, 2 = 
	int wScan;
	int time;		// optional delay in milliseconds: 0 = execute now
	int dwExtraInfo;
	int dwFlags;
};
wincmd buildcmd(int wincode = 0, int type = 1, int wScan = 0, int time = 0, int dwExtraInfo = 0, int dwFlags = 0) {
	wincmd outcmd;
	outcmd.wincode = wincode;
	outcmd.type = type;
	outcmd.wScan = wScan;
	outcmd.time = time;
	outcmd.dwExtraInfo = dwExtraInfo;
	outcmd.dwFlags = dwFlags;
	return outcmd;
};

// windows input class
class windowsInput {
private:

	// default command schemes
	wincmd defkeycmd = buildcmd();
	wincmd nullcmd = buildcmd(-1, -1, -1, -1, -1, -1);

public:
	wincmd cmdtype(int modeselect) {
		if (modeselect == 0) {
			return nullcmd;
		}
		else if (modeselect == 1) {
			return defkeycmd;
		}
		else if (modeselect == 2) {
			//return defmousecmd;
		}
		else {
			cerr << "cmdtype could not find selected mode, returning defkeycmd" << endl;
			return defkeycmd;
		}
	}

	int findwinkey(string key) {
		for (int i = 0; i < winlen; i++) {
			if (key == winkeylist[i]) return i;
		}
		cerr << "invalid windows key" << endl;
		return -1;
	}

	int sendwinkey(string key, int mode = 1) {
		wincmd incmd = cmdtype(mode);
		int wincodeindex = findwinkey(key);
		incmd.wincode = wincodelist[wincodeindex];

		INPUT ip1;

		// command assembly
		ip1.type = incmd.type;
		ip1.ki.wScan = incmd.wScan;
		ip1.ki.time = incmd.time;
		ip1.ki.dwExtraInfo = incmd.dwExtraInfo;

		// key press
		ip1.ki.wVk = incmd.wincode;
		ip1.ki.dwFlags = incmd.dwFlags;
		SendInput(1, &ip1, sizeof(INPUT));

		// key release
		ip1.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip1, sizeof(INPUT));

		return 0;
	}

	int multisend(string cmdlist[], int listsize, int mode = 1) {
		wincmd* incmd = new wincmd[listsize];
		INPUT* ip = new INPUT[listsize];

		// assembles the commands
		for (int i = 0; i < listsize; i++) {
			incmd[i] = cmdtype(1);
			incmd[i].wincode = wincodelist[findwinkey(cmdlist[i])];
			ip[i].type = incmd[i].type;
			ip[i].ki.wScan = incmd[i].wScan;
			ip[i].ki.time = incmd[i].time;
			ip[i].ki.dwExtraInfo = incmd[i].dwExtraInfo;
		}

		// presses keys
		for (int i = 0; i < listsize; i++) {
			ip[i].ki.wVk = incmd[i].wincode;
			ip[i].ki.dwFlags = incmd[i].dwFlags;
		}
		SendInput(listsize, ip, sizeof(INPUT));

		// releases keys
		for (int i = 0; i < listsize; i++) {
			ip[i].ki.dwFlags = KEYEVENTF_KEYUP;
		}
		SendInput(listsize, ip, sizeof(INPUT));

		delete[] ip;
		delete[] incmd;
		return 0;
	}
};

// takes a string delimited by colons (:) and splits it into a maximum of 5 commandds
int commandsplitter(string* outlist, string searchstring) {
	int i = 0;
	int cmd = 0;

	while (searchstring[i] != '\0' && searchstring[i] != '#') {
		if (searchstring[i] == ':') {
			cmd++;
		}
		else if (searchstring[i] != ' ') {
			outlist[cmd] += searchstring[i];
		}
		i++;
	}
	return cmd + 1;
}

int main()
{
	windowsInput key1;
	string cmdlist[5];	// max of 5 simultaneous commands
	int cmdlen = 0; // length of cmdlist array

	fstream fin;
	fin.open(cmdfile, std::ios::in);

	string instring;
	while (!fin.eof() && fin.is_open()) {
		getline(fin, instring);
		cmdlen = commandsplitter(cmdlist, instring);
		
		// prints command list for debugging purposes
		for (int i = 0; i < cmdlen; i++) {
			cout << "cmdlist[" << i << "] = " << cmdlist[i];
		}
		cout << endl;

		key1.multisend(cmdlist, cmdlen);
		
		// clear cmdlist
		for (int i = 0; i < cmdlen; i++) {
			cmdlist[i] = "";
		}
		cmdlen = 0;
	}
	fin.close();
	cout << "PRESS ANY KEY AND ENTER TO EXIT\n";
	cin >> cmdlist[0];
}
