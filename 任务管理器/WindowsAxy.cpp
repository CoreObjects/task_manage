#include "pch.h"
#include "WindowsAxy.h"
/*
#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <xlocbuf>
#include <iostream>
#include <tchar.h>
#include <psapi.h>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
*/
BOOL WindowsAxy::CreateProcessAxy(LPCTSTR buffer, PROCESS_INFORMATION* pi) {
	LPCTSTR szCommandLine = buffer;//"C:\\Users\\Core_Objects\\Desktop\\CodeinEX.exe";
	STARTUPINFO si = { 0 }; /*= { sizeof(si); }*/
	si.cb = sizeof(si);
	BOOL bRet = ::CreateProcess(
		szCommandLine,//不在此指定可执行文件的文件名
		NULL, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		pi);
	return bRet;
}

BOOL WindowsAxy::CreateProcessAxy(LPCTSTR buffer) {
	PROCESS_INFORMATION pi;
	LPCTSTR szCommandLine = buffer;//"C:\\Users\\Core_Objects\\Desktop\\CodeinEX.exe";
	STARTUPINFO si = { 0 }; /*= { sizeof(si); }*/
	si.cb = sizeof(si);
	BOOL bRet = ::CreateProcess(
		szCommandLine,//不在此指定可执行文件的文件名
		NULL, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	if (bRet) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return bRet;
}

BOOL WindowsAxy::CreateProcessSuspend(LPCTSTR buffer, PROCESS_INFORMATION& pi) {
	LPCTSTR szCommandLine = buffer;//"C:\\Users\\Core_Objects\\Desktop\\CodeinEX.exe";
	STARTUPINFO si = { 0 }; /*= { sizeof(si); }*/
	si.cb = sizeof(si);
	BOOL bRet = ::CreateProcess(
		szCommandLine,//不在此指定可执行文件的文件名
		NULL, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_SUSPENDED,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	return bRet;
}

BOOL WindowsAxy::CreateProcessCmd(LPCTSTR buffer) {
	PROCESS_INFORMATION pi;
	TCHAR szCommandLine[MAX_PATH]{ 0 };
#ifdef UNICODE
	memcpy(szCommandLine, buffer, (wcslen(buffer) + 1) * 2);
#else
	memcpy(szCommandLine, buffer, strlen(buffer) + 1);
#endif
	STARTUPINFO si = { 0 }; /*= { sizeof(si); }*/
	si.cb = sizeof(si);
	BOOL bRet = ::CreateProcess(
		NULL,//不在此指定可执行文件的文件名
		szCommandLine, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	if (bRet) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return bRet;
}

BOOL WindowsAxy::CreateProcessAdmAxy(LPCTSTR buffer) {
	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = _T("runas");
	sei.lpFile = buffer;
	sei.nShow = SW_SHOWNORMAL;
	return ShellExecuteEx(&sei);
}

std::list<std::string> WindowsAxy::GetProcessList(void) {
	std::list<std::string> strlist;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	BOOL bRet = Process32First(hSnapshot, &pi);
	std::string str;
	while (bRet) {
#ifdef UNICODE
		str = wchar_tToString(pi.szExeFile) + ": PID = " + std::to_string(pi.th32ProcessID);
#else
		str = std::string(pi.szExeFile) + ": PID = " + std::to_string(pi.th32ProcessID);
#endif // UNICODE
		strlist.push_back(str);
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
	return strlist;
}

std::list<PROCESSENTRY32> WindowsAxy::GetProcessInfoList(void) {
	std::list<PROCESSENTRY32> ProcessInfolist;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pi{ sizeof(pi) };
	BOOL bRet = Process32First(hSnapshot, &pi);
	while (bRet) {
		ProcessInfolist.push_back(pi);
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
	return ProcessInfolist;
}

BOOL WindowsAxy::GetProcessInfoMap(std::unordered_multimap<DWORD, std::string>& Pid_name) {
	Pid_name.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;
	PROCESSENTRY32 pi{ sizeof(pi) };
	BOOL bRet = Process32First(hSnapshot, &pi);
	std::string name;
	DWORD pid;
	while (bRet) {
#ifdef UNICODE
		name = wchar_tToString(pi.szExeFile);
#else
		name = std::string(pi.szExeFile);
#endif // UNICODE
		pid = pi.th32ProcessID;
		Pid_name.insert(make_pair(pid, name));
		bRet = Process32Next(hSnapshot, &pi);
	}
	CloseHandle(hSnapshot);
	return TRUE;
}

BOOL WindowsAxy::GetProcessInfoMap(std::unordered_multimap<std::string, DWORD>& Name_Pid) {
	Name_Pid.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;
	PROCESSENTRY32 pi{ sizeof(pi) };
	BOOL bRet = Process32First(hSnapshot, &pi);
	std::string name;
	DWORD pid;
	DWORD dwIndex = 0;
	while (bRet) {
#ifdef UNICODE
		name = wchar_tToString(pi.szExeFile);
#else
		name = std::string(pi.szExeFile);
#endif // UNICODE
		pid = pi.th32ProcessID;
		Name_Pid.insert(make_pair(name, pid));
		bRet = Process32Next(hSnapshot, &pi);
		dwIndex++;
	}

	CloseHandle(hSnapshot);
	return TRUE;
}

BOOL WindowsAxy::GetProcessInfoMap(std::unordered_multimap<DWORD, PROCESSENTRY32>& mapProcessInfo) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	PROCESSENTRY32 pi{ sizeof(pi) };
	BOOL bRet = Process32First(hSnap, &pi);
	while (bRet) {
		mapProcessInfo.insert(std::make_pair(pi.th32ProcessID, pi));
		bRet = Process32Next(hSnap, &pi);
	}
	CloseHandle(hSnap);
	return TRUE;
}

BOOL WindowsAxy::GetProcessInfoMap(std::unordered_multimap<std::string, PROCESSENTRY32>& mapProcessInfo) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	PROCESSENTRY32 pi{ sizeof(pi) };
	BOOL bRet = Process32First(hSnap, &pi);
	while (bRet) {
#ifdef UNICODE
		mapProcessInfo.insert(std::make_pair(wchar_tToString(pi.szExeFile), pi));
#else
		mapProcessInfo.insert(std::make_pair(std::string(pi.szExeFile), pi));
#endif
		bRet = Process32Next(hSnap, &pi);
	}
	CloseHandle(hSnap);
	return TRUE;
}

BOOL WindowsAxy::KillProcess(DWORD pid) {
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (hProcess != NULL)
		return TerminateProcess(hProcess, 0);
	else return FALSE;
}

DWORD WindowsAxy::GetProcessId(const wchar_t* szProcessName) {
	std::unordered_multimap<std::string, DWORD> Name_Pid;
	GetProcessInfoMap(Name_Pid);
	auto ite = Name_Pid.find(wchar_tToString(szProcessName));

	if (ite != Name_Pid.end())
		return ite->second;
	else
		return 0xFFFFFFFF;
}

DWORD WindowsAxy::GetProcessId(const char* szProcessName) {
	std::unordered_multimap<std::string, DWORD> Name_Pid;
	GetProcessInfoMap(Name_Pid);
	auto ite = Name_Pid.find(szProcessName);
	if (ite != Name_Pid.end())
		return ite->second;
	else
		return 0xFFFFFFFF;
}

std::vector<DWORD> WindowsAxy::GetMulitiProcessId(LPCTSTR szProcessName) {
	std::unordered_multimap<std::string, DWORD> Name_Pid;
	GetProcessInfoMap(Name_Pid);
#ifdef UNICODE
	std::string strProcessName = wchar_tToString(szProcessName);
	auto m = Name_Pid.find(strProcessName);
	std::vector<DWORD> ans;
	for (int k = 0; k != Name_Pid.count(strProcessName); k++, m++)
		ans.push_back(m->second);
	return ans;
#else
	auto m = Name_Pid.find(szProcessName);
	std::vector<DWORD> ans;
	for (int k = 0; k != Name_Pid.count(szProcessName); k++, m++)
		ans.push_back(m->second);
	return ans;
#endif // UNICODE

}

std::string WindowsAxy::GetProcessName(DWORD pid) {
	std::unordered_multimap<DWORD, std::string> Pid_Name;
	GetProcessInfoMap(Pid_Name);
	auto ite = Pid_Name.find(pid);
	if (ite != Pid_Name.end())
		return ite->second;
	else
		return "NO_Find";
}

PROCESSENTRY32 WindowsAxy::GetProcessInfo(DWORD pid) {
	std::unordered_multimap<DWORD, PROCESSENTRY32> mapProcessInfo;
	GetProcessInfoMap(mapProcessInfo);
	auto ite = mapProcessInfo.find(pid);
	if (ite != mapProcessInfo.end())
		return ite->second;
	else {
		return PROCESSENTRY32();
	}
}

std::string WindowsAxy::GetProcessFilePath(DWORD dwProcessId) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	std::string ans;
	if (hProcess != INVALID_HANDLE_VALUE) {
		CHAR csFilePath[MAX_PATH]{ 0 };
		GetModuleFileNameExA(hProcess, NULL, csFilePath, MAX_PATH);
		ans = csFilePath;
		CloseHandle(hProcess);
	}
	return ans;
}

BOOL WindowsAxy::GetThreadList(std::list<std::string>& ThreadList, DWORD nProcessId /*= NULL*/) {
	ThreadList.clear();
	std::string str;
	DWORD dwPID = nProcessId;//遍历系统内部所有的线程
	HANDLE hThreadSnap = 0;
	//在创建快照的API里,第一个参数，代表着要获取什么TH32CS_SNAPTHRBAD代表获取系统内所有的线程
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwPID);
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		//异常返回,代表线程快照，创建失败
		return FALSE;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do {
		if (dwPID == NULL) {
			str = "线程ID:" + std::to_string(stcTe32.th32ThreadID);
			ThreadList.push_back(str);
		}
		else if (dwPID == stcTe32.th32OwnerProcessID) {
			str = "线程ID:" + std::to_string(stcTe32.th32ThreadID);
			ThreadList.push_back(str);
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	return TRUE;
}

BOOL WindowsAxy::GetThreadInfoList(std::list<THREADENTRY32>& ThreadList, DWORD nProcessId /*= NULL*/) {
	ThreadList.clear();
	DWORD dwPID = nProcessId;//遍历系统内部所有的线程
	HANDLE hThreadSnap = 0;
	//在创建快照的API里,第一个参数，代表着要获取什么TH32CS_SNAPTHRBAD代表获取系统内所有的线程
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		//异常返回,代表线程快照，创建失败
		return FALSE;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do {
		if (dwPID == NULL) {
			ThreadList.push_back(stcTe32);
		}
		else if (dwPID == stcTe32.th32OwnerProcessID) {
			ThreadList.push_back(stcTe32);
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	return TRUE;
}

BOOL WindowsAxy::GetThreadIdArray(std::vector<DWORD>& ThreadIdVector, DWORD nProcessId /*= NULL*/) {
	ThreadIdVector.clear();
	DWORD dwPID = nProcessId;//遍历系统内部所有的线程
	HANDLE hThreadSnap = 0;
	//在创建快照的API里,第一个参数，代表着要获取什么TH32CS_SNAPTHRBAD代表获取系统内所有的线程
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		//异常返回,代表线程快照，创建失败
		return FALSE;
	}
	THREADENTRY32 stcTe32 = { sizeof(THREADENTRY32) };
	Thread32First(hThreadSnap, &stcTe32);
	do {
		if (dwPID == NULL) {
			ThreadIdVector.push_back(stcTe32.th32ThreadID);
		}
		else if (dwPID == stcTe32.th32OwnerProcessID) {
			ThreadIdVector.push_back(stcTe32.th32ThreadID);
		}
	} while (Thread32Next(hThreadSnap, &stcTe32));
	return TRUE;
}

BOOL WindowsAxy::SuspendThread(DWORD Tid) {
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Tid);
	if (hThread) {
		::SuspendThread(hThread);
		CloseHandle(hThread);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL WindowsAxy::ResumeThread(DWORD Tid) {
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Tid);
	if (hThread) {
		::ResumeThread(hThread);
		CloseHandle(hThread);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL WindowsAxy::KillThread(DWORD Tid) {
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Tid);
	if (hThread) {
		::TerminateThread(hThread, 0);
		CloseHandle(hThread);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

HANDLE WindowsAxy::CreateThreadAxy(_Out_ DWORD& Tid, _In_ ThreadCallbackAxy ThreadProc, _In_ LPVOID lp /*= NULL*/, _In_ DWORD dwCreationFlags /*= 0*/, _In_ SIZE_T dwStackSize /*= 0*/, _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes /*= NULL*/) {
	return CreateThread(lpThreadAttributes, dwStackSize, ThreadProc, lp, dwCreationFlags, &Tid);
}

HANDLE WindowsAxy::CreateThreadAxy(_In_ ThreadCallbackAxy ThreadProc, _In_ LPVOID lp /*= NULL*/, _In_ DWORD dwCreationFlags /*= 0*/, _In_ SIZE_T dwStackSize /*= 0*/, _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes /*= NULL*/) {
	DWORD Tid;
	return CreateThread(lpThreadAttributes, dwStackSize, ThreadProc, lp, dwCreationFlags, &Tid);
}


HANDLE WindowsAxy::CreateRemoteThreadAXY(HANDLE hProcess, ThreadCallbackAxy ThreadProc, LPVOID lp /*= NULL*/) {
	return CreateRemoteThread(hProcess, NULL, NULL, ThreadProc, lp, NULL, NULL);
}

BOOL WindowsAxy::WriteProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (hProcess != INVALID_HANDLE_VALUE) {
		BOOL bret = ::WriteProcessMemory(hProcess, source, target, size, NULL);
		CloseHandle(hProcess);
		return bret;
	}
	else return FALSE;
}

BOOL WindowsAxy::WriteProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T size) {
	return ::WriteProcessMemory(hProcess, source, target, size, NULL);
}

BOOL WindowsAxy::ReadProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (hProcess != NULL) {
		return ::ReadProcessMemory(hProcess, source, target, size, NULL);
		CloseHandle(hProcess);
	}
	else return FALSE;
}

BOOL WindowsAxy::ReadProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T size) {
	return ::ReadProcessMemory(hProcess, source, target, size, nullptr);
}

LPVOID WindowsAxy::MemoryAlloc(SIZE_T dwSize, DWORD flProtect /*= PAGE_EXECUTE_READWRITE*/, LPVOID lpAddress /*= NULL*/, DWORD flAllocationType /*= MEM_COMMIT | MEM_RESERVE*/) {
	return VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect);
}

LPVOID WindowsAxy::RemoteMemoryAlloc(HANDLE hProcess, SIZE_T dwSize, DWORD flProtect /*= PAGE_EXECUTE_READWRITE*/, LPVOID lpAddress /*= NULL*/, DWORD flAllocationType /*= MEM_COMMIT | MEM_RESERVE*/) {
	return VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
}

LPVOID WindowsAxy::RemoteMemoryAlloc(DWORD dwProcessId, SIZE_T dwSize, DWORD flProtect /*= PAGE_EXECUTE_READWRITE*/, LPVOID lpAddress /*= NULL*/, DWORD flAllocationType /*= MEM_COMMIT | MEM_RESERVE*/) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	else {
		lpAddress = VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
		CloseHandle(hProcess);
		return lpAddress;
	}

}

BOOL WindowsAxy::FreeMemory(LPVOID lpAddress) {
	return VirtualFree(lpAddress, 0, MEM_RELEASE);
}

BOOL WindowsAxy::RemoteFreeMemory(HANDLE hProcess, LPVOID lpAddress) {
	return VirtualFreeEx(hProcess, lpAddress, 0, MEM_RELEASE);
}

BOOL WindowsAxy::RemoteFreeMemory(DWORD dwProcessId, LPVOID lpAddress) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	else {
		BOOL bRet = VirtualFreeEx(hProcess, lpAddress, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return bRet;
	}
}

BOOL WindowsAxy::RemoteMemoryProtect(DWORD dwProcessId, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, DWORD& flOldProtect) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	else {
		BOOL bRet = VirtualProtectEx(hProcess, lpAddress, dwSize, flNewProtect, &flOldProtect);
		CloseHandle(hProcess);
		return bRet;
	}
}

BOOL WindowsAxy::RemoteMemoryProtect(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, DWORD& flOldProtect) {
	return VirtualProtectEx(hProcess, lpAddress, dwSize, flNewProtect, &flOldProtect);
}


BOOL WindowsAxy::WaitUntilTime(WORD Year, WORD Month, WORD Day, WORD Hour, WORD Minute, WORD Second, WORD ms /*= 0*/) {
	HANDLE g_WaitableTimer = CreateWaitableTimerW(NULL, FALSE, L"TIME1");
	if (g_WaitableTimer == NULL) {
		return FALSE;
	}
	else {
		SYSTEMTIME st;
		FILETIME ftLocal, ftUTC;
		LARGE_INTEGER liUTC;
		st.wYear = Year;
		st.wMonth = Month;
		st.wDayOfWeek = 0;
		st.wDay = Day;
		st.wHour = Hour;
		st.wMinute = Minute;
		st.wSecond = Second;
		st.wMilliseconds = ms;
		SystemTimeToFileTime(&st, &ftLocal);
		LocalFileTimeToFileTime(&ftLocal, &ftUTC);
		liUTC.LowPart = ftUTC.dwLowDateTime;
		liUTC.HighPart = ftUTC.dwHighDateTime;
		//liUTC.QuadPart = -100000000;
		SetWaitableTimer(g_WaitableTimer, &liUTC, 0, NULL, NULL, FALSE);
		WaitForSingleObject(g_WaitableTimer, INFINITE);
		CloseHandle(g_WaitableTimer);
		return TRUE;
	}

}

BOOL WindowsAxy::GetProcessModuleList(_Out_ std::list<MODULEENTRY32>& ModuleList, _In_ LPCTSTR ProcessName) {
	DWORD ProcessID = GetProcessId(ProcessName);
	return GetProcessModuleList(ModuleList, ProcessID);
}

BOOL WindowsAxy::GetProcessModuleList(_Out_ std::list<MODULEENTRY32>& ModuleList, _In_ DWORD dwProcessId) {
	ModuleList.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
		ModuleList.push_back(me32);

		bRet = Module32Next(hSnapshot, &me32);
	}
	CloseHandle(hSnapshot);
	return TRUE;
}

BOOL WindowsAxy::GetProcessModuleNameList(_Out_ std::list<std::string>& ModuleList, _In_ LPCTSTR ProcessName) {
	DWORD ProcessID = GetProcessId(ProcessName);
	return GetProcessModuleNameList(ModuleList, ProcessID);
}

BOOL WindowsAxy::GetProcessModuleNameList(_Out_ std::list<std::string>& ModuleList, _In_ DWORD dwProcessId) {
	ModuleList.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
#ifdef UNICODE
		ModuleList.push_back(wchar_tToString(me32.szModule));
#else
		ModuleList.push_back(std::string(me32.szModule));
#endif // UNICODE
		bRet = Module32Next(hSnapshot, &me32);
	}
	return TRUE;
}

BOOL WindowsAxy::GetProcessModuleMap(_Out_ std::unordered_multimap<std::string, MODULEENTRY32>& ModuleMap, _In_ LPCTSTR ProcessName) {
	DWORD ProcessID = GetProcessId(ProcessName);
	return GetProcessModuleMap(ModuleMap, ProcessID);
}

BOOL WindowsAxy::GetProcessModuleMap(_Out_ std::unordered_multimap<std::string, MODULEENTRY32>& ModuleMap, _In_ DWORD dwProcessId) {
	ModuleMap.clear();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hSnapshot, &me32);
	while (bRet) {
#ifdef UNICODE
		ModuleMap.insert(make_pair(wchar_tToString(me32.szModule), me32));
#else
		ModuleMap.insert(make_pair(std::string(me32.szModule), me32));
#endif
		bRet = Module32Next(hSnapshot, &me32);
	}
	return TRUE;
}

LONG_PTR WindowsAxy::GetModuleBaseAddr(LPCTSTR ModuleName, DWORD ProcessID) {
	if (ProcessID == 0) {
		ProcessID = GetCurrentProcessId();
	}
	std::unordered_multimap<std::string, MODULEENTRY32> ModuleMap;
	GetProcessModuleMap(ModuleMap, ProcessID);
#ifdef UNICODE
	auto ite = ModuleMap.find(wchar_tToString(ModuleName));
#else
	auto ite = ModuleMap.find(std::string(ModuleName));
#endif // UNICODE
	return (LONG_PTR)ite->second.modBaseAddr;
	// 	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
	// 	if (hSnapshot == INVALID_HANDLE_VALUE)
	// 	{
	// 		return 0;
	// 	}
	// 	MODULEENTRY32 me32;
	// 	me32.dwSize = sizeof(MODULEENTRY32);
	// 	BOOL bRet = Module32First(hSnapshot, &me32);
	// 	while (bRet) {
	// 		//std::cout << WindowsAxy::wchar_tToString(me32.szModule) << std::endl;
	// 
	// 		int nFlag = wcscmp(me32.szModule, ModuleName);
	// 		if (nFlag == 0)
	// 		{
	// 			return (DWORD)me32.modBaseAddr;
	// 		}
	// 		bRet = Module32Next(hSnapshot, &me32);
	// 
	// 	}
	// 	return -1;
}



BOOL WindowsAxy::RemoteInject(DWORD dwProcessId, const wchar_t* csDllPath) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, MAX_PATH * 2, MEM_COMMIT, PAGE_READWRITE);
	if (lpBuffer == NULL) {
		CloseHandle(hProcess);
		return FALSE;
	}
	SIZE_T dwRealWrite;
	BOOL bRet = ::WriteProcessMemory(hProcess, lpBuffer, csDllPath, (wcslen(csDllPath) + 1) * 2, &dwRealWrite);
	if (!bRet) {
		CloseHandle(hProcess);
		return FALSE;
	}
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, lpBuffer, 0, NULL);
	if (hThread == NULL) {
		CloseHandle(hProcess);
		return FALSE;
	}
	WaitForSingleObject(hThread, -1);
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, lpBuffer, 0, MEM_RELEASE);
	CloseHandle(hProcess);
	return TRUE;
}

BOOL WindowsAxy::RemoteInject(DWORD dwProcessId, const char* csDllPath) {
	std::wstring wcsDllPath = StringToWString(csDllPath);
	return RemoteInject(dwProcessId, wcsDllPath.c_str());
}

BOOL WindowsAxy::RemoteInject(std::string szProcessName, const wchar_t* csDllPath) {
	return RemoteInject(GetProcessId(szProcessName.c_str()), csDllPath);
}

BOOL WindowsAxy::RemoteFreelibrary(DWORD dwProcessId, const wchar_t* csDllPath) {
	MODULEENTRY32 me32 = { sizeof(me32) };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, dwProcessId);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	BOOL bRet = Module32First(hSnap, &me32);
	while (bRet) {
		if (!wcscmp(csDllPath, (const wchar_t*)me32.szExePath) || !wcscmp(csDllPath, (const wchar_t*)me32.szModule)) {
			LONG_PTR dwModeBase = (LONG_PTR)me32.modBaseAddr;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
			if (hProcess == INVALID_HANDLE_VALUE) {
				CloseHandle(hSnap);
				return FALSE;
			}
			HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, (LPVOID)dwModeBase, 0, NULL);
			if (hThread == NULL) {
				CloseHandle(hSnap);
				CloseHandle(hProcess);
				return FALSE;
			}
			WaitForSingleObject(hThread, -1);
			CloseHandle(hThread);
			CloseHandle(hProcess);
			CloseHandle(hSnap);
			return TRUE;
		}
		bRet = Module32Next(hSnap, &me32);
	}
	CloseHandle(hSnap);
	return FALSE;
}

BOOL WindowsAxy::RemoteFreelibrary(DWORD dwProcessId, const char* csDllPath) {
	std::wstring wcsDllPath = StringToWString(csDllPath);
	return RemoteFreelibrary(dwProcessId, wcsDllPath.c_str());
}

BOOL WindowsAxy::RemoteFreelibrary(std::string szProcessName, const wchar_t* csDllPath) {
	return RemoteFreelibrary(GetProcessId(szProcessName.c_str()), csDllPath);
}

BOOL WindowsAxy::UnTraceInject(DWORD dwProcessId, const wchar_t* csDllPath) {
	RemoteInject(dwProcessId, csDllPath);
	MODULEENTRY32 me32 = { sizeof(me32) };
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, dwProcessId);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return false;
	}
	BOOL bRet = Module32First(hSnap, &me32);
	LONG_PTR dwModeBase = 0, dwModeSize = 0;
	while (bRet) {
		if (!wcscmp(csDllPath, (const wchar_t*)me32.szExePath) || !wcscmp(csDllPath, (const wchar_t*)me32.szModule)) {
			dwModeBase = (LONG_PTR)me32.modBaseAddr;
			dwModeSize = (LONG_PTR)me32.modBaseSize;
			break;
		}
		bRet = Module32Next(hSnap, &me32);
	}
	CloseHandle(hSnap);
	if (!bRet) {
		return false;
	}
	char* szBuffer = new char[dwModeSize] {0};
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE) {
		//		CloseHandle(hSnap);
		return false;
	}
	bRet = ::ReadProcessMemory(hProcess, (LPVOID)dwModeBase, szBuffer, dwModeSize, NULL);
	if (!bRet) {
		CloseHandle(hProcess);
		return false;
	}
	bRet = RemoteFreelibrary(dwProcessId, csDllPath);
	if (!bRet) {
		CloseHandle(hProcess);
		return false;
	}
	LPVOID lpModule = VirtualAllocEx(hProcess, (LPVOID)dwModeBase, dwModeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (lpModule == NULL) {
		CloseHandle(hProcess);
		return false;
	}
	bRet = ::WriteProcessMemory(hProcess, (LPVOID)dwModeBase, szBuffer, dwModeSize, NULL);
	CloseHandle(hProcess);
	return bRet;
}

BOOL WindowsAxy::UnTraceInject(DWORD dwProcessId, const char* csDllPath) {
	std::wstring wcsDllPath = StringToWString(csDllPath);
	return UnTraceInject(dwProcessId, wcsDllPath.c_str());
}

BOOL WindowsAxy::UnTraceInject(std::string szProcessName, const wchar_t* csDllPath) {
	return UnTraceInject(GetProcessId(szProcessName.c_str()), csDllPath);
}

BOOL WindowsAxy::ReadFile(LPCTSTR szFilePath, std::vector<unsigned char>& szBuffer) {
	HANDLE hFile = CreateFile(szFilePath, \
		GENERIC_READ, \
		0, NULL, \
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwRealRead;
	DWORD dwLength = GetFileSize(hFile, NULL);
	char* szbuffer = new char[dwLength];
	BOOL bRet = ::ReadFile(hFile, szbuffer, dwLength, &dwRealRead, NULL);
	for (DWORD i = 0; i < dwLength; i++) {
		szBuffer.push_back(szbuffer[i]);
	}
	delete[]szbuffer;
	CloseHandle(hFile);
	return bRet;
}

char* WindowsAxy::ReadFile(LPCTSTR szFilePath) {
	HANDLE hFile = CreateFile(szFilePath, \
		GENERIC_READ, \
		0, NULL, \
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwRealRead;
	DWORD dwLength = GetFileSize(hFile, NULL);
	char* szbuffer = new char[dwLength] {0};
	::ReadFile(hFile, szbuffer, dwLength, &dwRealRead, NULL);
	CloseHandle(hFile);
	return szbuffer;
}

BOOL WindowsAxy::ReadFile(LPCTSTR szFilePath, unsigned char* szBuffer, DWORD dwLength) {
	HANDLE hFile = CreateFile(szFilePath, \
		GENERIC_READ, \
		0, NULL, \
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwRealRead;
	BOOL bRet = ::ReadFile(hFile, szBuffer, dwLength, &dwRealRead, NULL);
	CloseHandle(hFile);
	return bRet;
}

BOOL WindowsAxy::WriteFile(LPCTSTR szFilePath, std::string str) {
	HANDLE hFile = CreateFile(szFilePath, \
		GENERIC_WRITE, \
		0, NULL, \
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwNumberOfBytesWritten;
	BOOL bRet = ::WriteFile(hFile, str.c_str(), (DWORD)str.length(), &dwNumberOfBytesWritten, NULL);
	CloseHandle(hFile);
	return bRet;
}



BOOL WindowsAxy::WriteFile(LPCTSTR szFilePath, unsigned char* ucbyte, DWORD dwLength, int nPos /*= 0*/) {
	HANDLE hFile = CreateFile(szFilePath, \
		GENERIC_WRITE, \
		0, NULL, \
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwNumberOfBytesWritten;
	DWORD dwFliePointer = SetFilePointer(hFile, nPos, NULL, FILE_BEGIN);
	BOOL bRet = ::WriteFile(hFile, ucbyte, dwLength, &dwNumberOfBytesWritten, NULL);
	CloseHandle(hFile);
	return bRet;
}

BOOL WindowsAxy::WriteFile_pushback(LPCTSTR szFilePath, std::string str) {
	HANDLE hFile = CreateFile(szFilePath, \
		GENERIC_WRITE, \
		0, NULL, \
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwNumberOfBytesWritten;
	DWORD dwFliePointer = SetFilePointer(hFile, 0, NULL, FILE_END);
	BOOL bRet = ::WriteFile(hFile, str.c_str(), (DWORD)str.length(), &dwNumberOfBytesWritten, NULL);
	CloseHandle(hFile);
	return bRet;
}



std::string WindowsAxy::ReadClipboard() {
	std::string szBuffer;
	if (OpenClipboard(NULL)) {
		if (IsClipboardFormatAvailable(CF_TEXT)) {
			HGLOBAL hGlobalClip = GetClipboardData(CF_TEXT);
			if (hGlobalClip)
				szBuffer = std::string((char*)GlobalLock(hGlobalClip));
			GlobalUnlock(hGlobalClip);
		}
		CloseClipboard();
	}
	return szBuffer;
}

BOOL WindowsAxy::WriteClipboard(const std::string& str) {
	BOOL bRet = OpenClipboard(NULL);
	if (bRet) {
		char* szBuffer = NULL;
		EmptyClipboard();
		HGLOBAL hGlobalClip = GlobalAlloc(GHND, str.length() + 1);
		if (hGlobalClip) {
			LPVOID lpBuffer = GlobalLock(hGlobalClip);
			if (lpBuffer != NULL) {
				memcpy(lpBuffer, str.c_str(), str.length() + 1);
			}
			GlobalUnlock(hGlobalClip);
			SetClipboardData(CF_TIFF, hGlobalClip);
			CloseClipboard();
			return TRUE;
		}
	}
	return FALSE;
}

std::list<HWND> WindowsAxy::GetWindowList() {
	HWND hWnd = GetTopWindow(0);
	std::list<HWND> hWndList;
	hWnd = GetWindow(hWnd, GW_HWNDFIRST);
	while (hWnd) {
		if (IsWindowVisible(hWnd) && !GetParent(hWnd)) {
			hWndList.push_back(hWnd);
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	return hWndList;
}

std::list<WndInfo> WindowsAxy::GetWindowInfoList() {
	WndInfo stcWndInfo;
	std::list<WndInfo> listWndInfo;
	HWND hWnd = GetTopWindow(0);
	hWnd = GetWindow(hWnd, GW_HWNDFIRST);
	while (hWnd) {
		if (IsWindowVisible(hWnd) && !GetParent(hWnd) && hWnd) {
			CHAR csWindowsName[MAX_PATH]{ 0 };
			int nLen = ::GetWindowTextLengthA(hWnd);
			::GetWindowTextA(hWnd, csWindowsName, nLen + 1);
			stcWndInfo.szWndName = csWindowsName;
			if (!stcWndInfo.szWndName.empty()) {
				stcWndInfo.bFlag = !IsHungAppWindow(hWnd);
				stcWndInfo.dwThreadId = GetWindowThreadProcessId(hWnd, &(stcWndInfo.dwProcessId));
				stcWndInfo.hWnd = hWnd;
				stcWndInfo.hIcon = (HICON)::SendMessage(hWnd, WM_GETICON, FALSE, 0);
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, stcWndInfo.dwProcessId);
				GetModuleFileNameExA(hProcess, NULL, csWindowsName, MAX_PATH);
				CloseHandle(hProcess);
				stcWndInfo.szPath = csWindowsName;
				if (stcWndInfo.hIcon == NULL) {
#ifdef UNICODE
					stcWndInfo.hIcon = ::ExtractIcon(NULL, StringToWString(stcWndInfo.szPath).c_str(), 0);
#else
					stcWndInfo.hIcon = ::ExtractIcon(NULL, stcWndInfo.szPath.c_str(), 0);
#endif // UNICODE
					if (stcWndInfo.hIcon == NULL) {
						stcWndInfo.hIcon = LoadIcon(NULL, IDI_APPLICATION);
					}
				}
				listWndInfo.push_back(stcWndInfo);
			}
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	return listWndInfo;
}

std::vector<std::string> WindowsAxy::GetAllSubKeyPath(HKEY hKey, std::string szKeyPath) {
	HKEY key;
	DWORD dwBufferSize = 255;
	std::vector<std::string> ans;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, szKeyPath.c_str(), 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS) {
		char szSubKeyPath[MAX_PATH]{ 0 };
		DWORD dwIndex = 0;
		while (SHEnumKeyExA(key, dwIndex++, szSubKeyPath, &dwBufferSize) != ERROR_NO_MORE_ITEMS) {
			dwBufferSize = MAX_PATH;
			std::string str = szKeyPath + "\\" + szSubKeyPath;
			ans.push_back(str);
		}
		RegCloseKey(hKey);
	}
	return ans;
}

std::list<InstallInfo> WindowsAxy::GetInstallInfoList() {
	std::vector<std::string> szSubKeyPath = GetAllSubKeyPath(HKEY_LOCAL_MACHINE, \
		"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	DWORD dwSize = MAX_PATH;
	DWORD dwType = REG_SZ;
	HKEY hSubKey;
	InstallInfo stcSoftInfo;
	std::list<InstallInfo> InstallInfoList;
	for (auto ite = szSubKeyPath.begin(); ite != szSubKeyPath.end(); ite++) {
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, ite->c_str(), 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS) {
			char szBuffer[MAX_PATH]{ 0 };
			RegQueryValueExA(hSubKey, "DisplayName", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = MAX_PATH;
			stcSoftInfo.szName = szBuffer;
			if (stcSoftInfo.szName.empty()) {
				RegCloseKey(hSubKey);
				continue;
			}
			RegQueryValueExA(hSubKey, "DisplayVersion", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = MAX_PATH;
			stcSoftInfo.szVersion = szBuffer;
			RegQueryValueExA(hSubKey, "InstallLocation", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = MAX_PATH;
			stcSoftInfo.szInstallPath = szBuffer;
			RegQueryValueExA(hSubKey, "Publisher", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = MAX_PATH;
			stcSoftInfo.szPublish = szBuffer;
			RegQueryValueExA(hSubKey, "UninstallString", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = MAX_PATH;
			stcSoftInfo.szUnistallPath = szBuffer;
			RegQueryValueExA(hSubKey, "InstallDate", NULL, &dwType, (LPBYTE)szBuffer, &dwSize);
			dwSize = MAX_PATH;
			stcSoftInfo.szInstallData = szBuffer;
			RegCloseKey(hSubKey);
			InstallInfoList.push_back(stcSoftInfo);
		}
	}
	return InstallInfoList;
}

std::unordered_map<std::string, std::string> WindowsAxy::GetAllKeyValue(HKEY Key, const char* szPath) {
	std::unordered_map<std::string, std::string> mapNameValue;
	HKEY cpp_key;
	LPCSTR cpp_data = szPath;
	//尝试打开注册表，‘HKEY_LOCAL_MACHINE’要打开的根表；‘cpp_data’要打开的子表项；‘0’固定值；‘KEY_ALL_ACCESS’申请的权限；‘&cpp_key’返回句柄；
	if (ERROR_SUCCESS == RegOpenKeyExA(Key, cpp_data, 0, KEY_READ, &cpp_key)) {
		DWORD dwIndex = 0, NameSize, NameCnt, NameMaxLen, Type;
		DWORD KeySize, KeyCnt, KeyMaxLen, DateSize, MaxDateLen;
		if (ERROR_SUCCESS == RegQueryInfoKeyA(cpp_key, NULL, NULL, 0, &KeyCnt, &KeyMaxLen, NULL, &NameCnt, &NameMaxLen, &MaxDateLen, NULL, NULL)) {
			for (DWORD dwIndex = 0; dwIndex < NameCnt; dwIndex++)    //枚举键值
			{
				DateSize = MaxDateLen + 1;
				NameSize = NameMaxLen + 1;
				char* szValueName = (char*)malloc(NameSize);
				LPBYTE szValueDate = (LPBYTE)malloc(DateSize);
				RegEnumValueA(cpp_key, dwIndex, szValueName, &NameSize, NULL, &Type, szValueDate, &DateSize);//读取键值
				if (Type == REG_SZ) {
					mapNameValue.insert(make_pair(std::string(szValueName), std::string((char*)szValueDate)));
				}
			}
		}
	}
	RegCloseKey(cpp_key);//关闭句柄
	return mapNameValue;
}

void WindowsAxy::SetAutoRun(LPCSTR szFilePath) {
	/*	if (GetModuleFileNameA(NULL, (LPSTR)szFilePath, MAX_PATH)) {*/
	HKEY hKey;
	CStringA strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, strRegPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		RegSetValueExA(hKey, "AutoRun", 0, REG_SZ, (LPBYTE)szFilePath, (strlen(szFilePath) + 1) * sizeof(TCHAR));
	}
	RegCloseKey(hKey);
	/*	}*/
}

// BOOL WindowsAxy::ExeShell(const char* cmd, char* result) {
// 	char szBuffer[MAX_PATH] = { 0 };
// 	FILE* pipe = _popen(cmd, "r");
// 	while (!feof(pipe)) {
// 		if (fgets(szBuffer, MAX_PATH, pipe)) {
// 			strcat(result, szBuffer);
// 		}
// 	}
// 	_pclose(pipe);
// 	return TRUE;
// }

wchar_t* WindowsAxy::stringToWchar_t(std::string str) {
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t* wszUtf8 = new wchar_t[(LONG64)len + 1];
	memset(wszUtf8, 0, (LONG64)len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	return wszUtf8;
}

std::string WindowsAxy::wchar_tToString(const wchar_t* wchar) {
	std::string szDst;
	wchar_t* wText = (wchar_t*)wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char* psText;
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[]psText;
	return szDst;
}
#ifndef _WIN64
bool WindowsAxy::IsDebuggerPresentAXY() {
	bool bRet = FALSE;
	__asm {
		mov eax, fs: [0x30]
		mov al, byte ptr[eax + 2]
		mov bRet, al
	}
	return bRet;
}
#endif


std::wstring WindowsAxy::Utf8ToUnicode(const std::string& strUTF8) {
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	if (len == 0) {
		return L"";
	}

	wchar_t* pRes = new wchar_t[len];
	if (pRes == NULL) {
		return L"";
	}

	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;

	return result;
}

std::string WindowsAxy::UnicodeToUtf8(const std::wstring& strUnicode) {
	int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0) {
		return "";
	}
	char* pRes = new char[len];
	if (pRes == NULL) {
		return "";
	}
	WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

std::wstring WindowsAxy::StringToWString(const std::string& str) {
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (len == 0) {
		return L"";
	}

	wchar_t* pRes = new wchar_t[len];
	if (pRes == NULL) {
		return L"";
	}
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pRes, len);
	pRes[len - 1] = L'\0';
	std::wstring result = pRes;
	delete[] pRes;
	return result;
}

std::string WindowsAxy::WStringToString(const std::wstring& wstr) {
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0) {
		return "";
	}

	char* pRes = new char[len];
	if (pRes == NULL) {
		return "";
	}

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

std::string WindowsAxy::Utf8ToANSI(const std::string& strUtf8) {
	return WStringToString(Utf8ToUnicode(strUtf8));
}

std::string WindowsAxy::ANSIToUtf8(const std::string& strANSI) {
	return UnicodeToUtf8(StringToWString(strANSI));
}




