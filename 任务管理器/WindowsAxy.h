#pragma once
struct WndInfo {
	std::string szWndName;
	bool bFlag;
	DWORD dwProcessId;
	DWORD dwThreadId;
	HWND hWnd;
	std::string szPath;
	HICON hIcon;
};
struct InstallInfo {
	std::string szName;       //����
	std::string szVersion;        //����汾��
	std::string szPublish;     //�����������
	std::string szInstallData;       //�����װ����
	std::string szInstallPath;  //�����װ·��
	std::string szUnistallPath; //���ж��·��
};
class WindowsAxy
{
	using ThreadCallbackAxy = DWORD(WINAPI*)(LPVOID);
public:
	//�������
	static BOOL CreateProcessAxy(LPCTSTR, PROCESS_INFORMATION*);
	static BOOL CreateProcessAxy(LPCTSTR);
	static BOOL CreateProcessSuspend(LPCTSTR, PROCESS_INFORMATION&);
	static BOOL CreateProcessCmd(LPCTSTR);
	static BOOL CreateProcessAdmAxy(LPCTSTR);
	static std::list<std::string> GetProcessList(void);
	static std::list<PROCESSENTRY32> GetProcessInfoList(void);
	static BOOL GetProcessInfoMap(std::unordered_multimap<DWORD, std::string>&);
	static BOOL GetProcessInfoMap(std::unordered_multimap<std::string, DWORD>&);
	static BOOL GetProcessInfoMap(std::unordered_multimap<DWORD, PROCESSENTRY32>&);
	static BOOL GetProcessInfoMap(std::unordered_multimap<std::string, PROCESSENTRY32>&);
	static BOOL KillProcess(DWORD pid);
	static DWORD GetProcessId(const wchar_t* szProcessName);
	static DWORD GetProcessId(const char* szProcessName);
	static std::vector<DWORD> GetMulitiProcessId(LPCTSTR szProcessName);
	static std::string GetProcessName(DWORD pid);
	static PROCESSENTRY32 GetProcessInfo(DWORD pid);
	static std::string GetProcessFilePath(DWORD dwProcessId);
	//�߳����
	static BOOL GetThreadList(std::list<std::string>& ThreadList, DWORD nProcessId = NULL);
	static BOOL GetThreadInfoList(std::list<THREADENTRY32>& ThreadList, DWORD nProcessId = NULL);
	static BOOL GetThreadIdArray(std::vector<DWORD>& ThreadIdVector, DWORD nProcessId = NULL);
	static BOOL SuspendThread(DWORD Tid);
	static BOOL ResumeThread(DWORD Tid);
	static BOOL KillThread(DWORD Tid);
	static HANDLE CreateThreadAxy(
		_Out_ DWORD& Tid,
		_In_ ThreadCallbackAxy ThreadProc,
		_In_ LPVOID lp = NULL,
		_In_ DWORD dwCreationFlags = 0,
		_In_ SIZE_T dwStackSize = 0,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL);
	static HANDLE CreateThreadAxy(
		_In_ ThreadCallbackAxy ThreadProc,
		_In_ LPVOID lp = NULL,
		_In_ DWORD dwCreationFlags = 0,
		_In_ SIZE_T dwStackSize = 0,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL);
	static HANDLE CreateRemoteThreadAXY(HANDLE hProcess, ThreadCallbackAxy ThreadProc, LPVOID lp = NULL);
	//�ڴ����
	static BOOL WriteProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size);
	static BOOL WriteProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T size);
	static BOOL ReadProcessMemory(DWORD ProcessId, LPVOID source, LPVOID target, SIZE_T size);
	static BOOL ReadProcessMemory(HANDLE hProcess, LPVOID source, LPVOID target, SIZE_T size);
	static LPVOID MemoryAlloc(SIZE_T dwSize,\
		DWORD  flProtect = PAGE_EXECUTE_READWRITE,\
		LPVOID lpAddress = NULL,\
		DWORD  flAllocationType = MEM_COMMIT | MEM_RESERVE);
	static LPVOID RemoteMemoryAlloc(HANDLE hProcess,
		SIZE_T dwSize, 
		DWORD  flProtect = PAGE_EXECUTE_READWRITE, 
		LPVOID lpAddress = NULL, 
		DWORD  flAllocationType = MEM_COMMIT | MEM_RESERVE);
	static LPVOID RemoteMemoryAlloc(DWORD dwProcessId,
		SIZE_T dwSize,
		DWORD  flProtect = PAGE_EXECUTE_READWRITE,
		LPVOID lpAddress = NULL,
		DWORD  flAllocationType = MEM_COMMIT | MEM_RESERVE);
	static BOOL FreeMemory(LPVOID lpAddress);
	static BOOL RemoteFreeMemory(HANDLE hProcess,LPVOID lpAddress);
	static BOOL RemoteFreeMemory(DWORD dwProcessId, LPVOID lpAddress);
	static BOOL RemoteMemoryProtect(DWORD dwProcessId, LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, DWORD& flOldProtect);
	static BOOL RemoteMemoryProtect(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD  flNewProtect, DWORD& flOldProtect);

	//��ʱ�����
	static BOOL WaitUntilTime(WORD Year, WORD Month, WORD Day,WORD Hour, WORD Minute, WORD Second, WORD ms = 0);
	//ģ�����
	static BOOL GetProcessModuleList(_Out_ std::list<MODULEENTRY32>& ModuleList, _In_ DWORD dwProcessId);
	static BOOL GetProcessModuleList(_Out_ std::list<MODULEENTRY32>& ModuleList, _In_ LPCTSTR ProcessName);
	static BOOL GetProcessModuleNameList(_Out_ std::list<std::string>& ModuleList, _In_ LPCTSTR ProcessName);
	static BOOL GetProcessModuleNameList(_Out_ std::list<std::string>& ModuleList, _In_ DWORD dwProcessId);
	static BOOL GetProcessModuleMap(_Out_ std::unordered_multimap<std::string, MODULEENTRY32>& ModuleMap, _In_ LPCTSTR ProcessName);
	static BOOL GetProcessModuleMap(_Out_ std::unordered_multimap<std::string, MODULEENTRY32>& ModuleMap, _In_ DWORD dwProcessId);
	static LONG_PTR GetModuleBaseAddr(LPCTSTR ModuleName, DWORD ProcessID = 0);
	//ע�����
	static BOOL RemoteInject(DWORD dwProcessId, const wchar_t* csDllPath);
	static BOOL RemoteFreelibrary(DWORD dwProcessId, const wchar_t* csDllPath);
	static BOOL UnTraceInject(DWORD dwProcessId, const wchar_t* csDllPath);
	static BOOL RemoteInject(DWORD dwProcessId, const char* csDllPath);
	static BOOL RemoteFreelibrary(DWORD dwProcessId, const char* csDllPath);
	static BOOL UnTraceInject(DWORD dwProcessId, const char* csDllPath);
	static BOOL RemoteInject(std::string szProcessName, const wchar_t* csDllPath);
	static BOOL RemoteFreelibrary(std::string szProcessName, const wchar_t* csDllPath);
	static BOOL UnTraceInject(std::string szProcessName, const wchar_t* csDllPath);
	//�ļ����
	static BOOL ReadFile(LPCTSTR szFilePath, std::vector<unsigned char>& szBuffer);
	static char* ReadFile(LPCTSTR szFilePath);
	static BOOL ReadFile(LPCTSTR szFilePath,unsigned char* szBuffer,DWORD dwLength);
	static BOOL WriteFile(LPCTSTR szFilePath, std::string str);
	static BOOL WriteFile(LPCTSTR szFilePath, unsigned char* ucbyte, DWORD dwLength,int nPos = 0);
	static BOOL WriteFile_pushback(LPCTSTR szFilePath, std::string str);
	//���а����
	static std::string ReadClipboard();
	static BOOL WriteClipboard(const std::string&);
	//���ڲ���
	static std::list<HWND> GetWindowList();
	static std::list<WndInfo> GetWindowInfoList();
	//ע������
	static std::vector<std::string>GetAllSubKeyPath(HKEY hKey, std::string szKeyPath);
	static std::list<InstallInfo> GetInstallInfoList();
	static std::unordered_map<std::string, std::string> GetAllKeyValue(HKEY Key, const char* szPath);
	static void SetAutoRun(LPCSTR szFilePath);
	//ִ��Shell
	BOOL ExeShell(const char* cmd, char* result);
	//���ߺ���
	static wchar_t* stringToWchar_t(std::string str);
	static std::string wchar_tToString(const wchar_t* wchar);
	static bool IsDebuggerPresentAXY();
	//����ת�����
	static std::wstring Utf8ToUnicode(const std::string& strUTF8);
	static std::string UnicodeToUtf8(const std::wstring& strUnicode);
	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& wstr);
	static std::string Utf8ToANSI(const std::string& strUtf8);
	static std::string ANSIToUtf8(const std::string& strANSI);
	//
};


