// CPageProcess.cpp: 实现文件
//

#include "pch.h"
#include "任务管理器.h"
#include "CPageProcess.h"
#include "afxdialogex.h"
#include "WindowsAxy.h"
#include "CPageThread.h"
#include "CPageModule.h"
// CPageProcess 对话框

IMPLEMENT_DYNAMIC(CPageProcess, CDialogEx)

CPageProcess::CPageProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent) {

}

CPageProcess::~CPageProcess() {
}

BOOL CPageProcess::OnInitDialog() {
	CDialogEx::OnInitDialog();

//	m_ImgList.Create(18, 18, ILC_MASK | ILC_COLOR32, 0, 1);
	m_ProcessList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ProcessList.InsertColumn(0, L"进程名", 0, 100);
	m_ProcessList.InsertColumn(1, L"进程ID", 0, 50);
	m_ProcessList.InsertColumn(2, L"父进程ID", 0, 60);
	m_ProcessList.InsertColumn(3, L"线程数", 0, 50);
	m_ProcessList.InsertColumn(4, L"优先级", 0, 50);
	m_ProcessList.InsertColumn(5, L"进程路径", 0, 450);
	m_ProcessList.InsertColumn(6, L"文件创建时间", 0, 180);
	//	m_ProcessList.SetImageList(&m_ImgList, LVSIL_SMALL);
	OnInitProcessList();

	return TRUE;
}

void CPageProcess::OnInitProcessList() {
	m_ProcessList.DeleteAllItems();
	std::list<PROCESSENTRY32> listProcessInfo = WindowsAxy::GetProcessInfoList();
	DWORD dwIndex = 0;
	//	while (m_ImgList.Remove(0));
	for (auto ite = listProcessInfo.begin(); ite != listProcessInfo.end(); ite++) {
		m_ProcessList.InsertItem(dwIndex, ite->szExeFile);
		CString str; str.Format(L"%d", ite->th32ProcessID);
		m_ProcessList.SetItemText(dwIndex, 1, str);
		str.Format(L"%d", ite->th32ParentProcessID);
		m_ProcessList.SetItemText(dwIndex, 2, str);
		str.Format(L"%d", ite->cntThreads);
		m_ProcessList.SetItemText(dwIndex, 3, str);
		str.Format(L"%d", ite->pcPriClassBase);
		m_ProcessList.SetItemText(dwIndex, 4, str);
		m_ProcessList.SetItemText(dwIndex, 5, WindowsAxy::StringToWString(WindowsAxy::GetProcessFilePath(ite->th32ProcessID)).c_str());
//		InsertImage(WindowsAxy::StringToWString(WindowsAxy::GetProcessFilePath(ite->th32ProcessID)).c_str());
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ite->th32ProcessID);
		if (hProcess != INVALID_HANDLE_VALUE) {
			CString csTime;
			WIN32_FIND_DATAW FindFileData;
			FindFirstFile(WindowsAxy::StringToWString(WindowsAxy::GetProcessFilePath(ite->th32ProcessID)).c_str(), &FindFileData);
			SYSTEMTIME systime;
			FileTimeToSystemTime(&FindFileData.ftCreationTime, &systime);
			csTime.Format(L"%d-%d-%d %02d:%02d:%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
			m_ProcessList.SetItemText(dwIndex, 6, csTime);
			CloseHandle(hProcess);
		}
		dwIndex++;
	}
}


// void CPageProcess::InsertImage(CString csNamePath) {
// 	HICON hSmallIcon = ::ExtractIcon(NULL, csNamePath, 0);
// 	if (hSmallIcon != NULL) {
// 		m_ImgList.Add(hSmallIcon);
// 	}
// 	else {
// 		m_ImgList.Add(LoadIcon(NULL, IDI_APPLICATION));
// 	}
// }

void CPageProcess::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProcessList);
}


BEGIN_MESSAGE_MAP(CPageProcess, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32771, &CPageProcess::OnKillProcess)
	ON_COMMAND(ID_32776, &CPageProcess::OnFlush)
	ON_COMMAND(ID_32772, &CPageProcess::OnThreadList)
	ON_COMMAND(ID_32773, &CPageProcess::OnOpenModuleList)
	ON_COMMAND(ID_32774, &CPageProcess::OnInjectdll)
	ON_COMMAND(ID_32775, &CPageProcess::OnInjectShellcode)
END_MESSAGE_MAP()


// CPageProcess 消息处理程序


void CPageProcess::OnContextMenu(CWnd* pWnd, CPoint pt) {
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	CMenu* pPopMenu = menu.GetSubMenu(0);
	::TrackPopupMenu(pPopMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);;
}


void CPageProcess::OnKillProcess() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	DWORD dwProcessId = _ttoi(m_ProcessList.GetItemText(nPos, 1));
	WindowsAxy::KillProcess(dwProcessId);
	OnInitDialog();
}


void CPageProcess::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	OnInitDialog();
}


void CPageProcess::OnThreadList() {
	// TODO: 在此添加命令处理程序代码
	CPageThread page;
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition();
	nPos -= 1;
	CString csPID = m_ProcessList.GetItemText(nPos, 1);
	page.dwProcessID = _ttoi(csPID);
	page.csProcessName = m_ProcessList.GetItemText(nPos, 0);
	page.DoModal();

}


void CPageProcess::OnOpenModuleList() {
	// TODO: 在此添加命令处理程序代码
	CPageModule page;
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	CString csPID = m_ProcessList.GetItemText(nPos, 1);
	page.dwProcessId = _ttoi(csPID);
	page.DoModal();
}


void CPageProcess::OnInjectdll() {
	// TODO: 在此添加命令处理程序代码
	CFileDialog fileDlg(TRUE, NULL, L"选择dll", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"动态链接库|*.dll||");
	fileDlg.DoModal();
	CString csFilePath = fileDlg.GetPathName();
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	DWORD dwProcessId = _ttoi(m_ProcessList.GetItemText(nPos, 1));
	WindowsAxy::RemoteInject(dwProcessId, csFilePath);

}


void CPageProcess::OnInjectShellcode() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ProcessList.GetFirstSelectedItemPosition() - 1;
	DWORD dwProcessId = _ttoi(m_ProcessList.GetItemText(nPos, 1));
	CFileDialog cFileDlg(TRUE, NULL, NULL, NULL, L"二进制文件|*.bin|AllFiles|*.*||", this);
	cFileDlg.DoModal();
	CString szPath = cFileDlg.GetPathName();
	HANDLE hFile = CreateFile(szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwLength = GetFileSize(hFile, NULL);
		CHAR* szBuffer = new CHAR[dwLength]{ 0 };
		DWORD dwReadRead;
		BOOL bRet = ReadFile(hFile, szBuffer, dwLength, &dwReadRead, NULL);
		if (bRet) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
			if (hProcess != INVALID_HANDLE_VALUE) {
				LPVOID lpBuffer = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				if (lpBuffer) {
					SIZE_T dwRealWrite;
					BOOL bRetWrite = WriteProcessMemory(hProcess, lpBuffer, szBuffer, dwLength, &dwRealWrite);
					if (bRetWrite) {
						HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpBuffer, NULL, 0, NULL);
						if (hThread)CloseHandle(hThread);
					}
				}
				CloseHandle(hProcess);
			}
		}
		CloseHandle(hFile);
	}
}
