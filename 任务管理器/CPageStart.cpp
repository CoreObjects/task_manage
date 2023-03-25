// CPageStart.cpp: 实现文件
//

#include "pch.h"
#include "任务管理器.h"
#include "CPageStart.h"
#include "afxdialogex.h"


// CPageStart 对话框

IMPLEMENT_DYNAMIC(CPageStart, CDialogEx)

CPageStart::CPageStart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent) {

}

CPageStart::~CPageStart() {
}
//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run

//计算机\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run\QQDisabled
BOOL CPageStart::OnInitDialog() {
	CDialogEx::OnInitDialog();
	m_StartList.InsertColumn(0, L"软件名称", 0, 100);
	m_StartList.InsertColumn(1, L"启动路径", 0, 400);
	m_StartList.InsertColumn(2, L"注册表路径", 0, 200);
	m_StartList.InsertColumn(3, L"Key", 0, 100);
	m_StartList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	InitStart();

	return TRUE;
}

void CPageStart::InitStart() {
	m_StartList.DeleteAllItems();
	std::unordered_map<std::string, std::string> s1 = WindowsAxy::GetAllKeyValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");
	std::unordered_map<std::string, std::string> s2 = WindowsAxy::GetAllKeyValue(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	std::unordered_map<std::string, std::string> s3 = WindowsAxy::GetAllKeyValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	DWORD dwIndex = 0;
	for (auto ite = s1.begin(); ite != s1.end(); ite++) {
		m_StartList.InsertItem(dwIndex, WindowsAxy::StringToWString(ite->first).c_str());
		m_StartList.SetItemText(dwIndex, 1, WindowsAxy::StringToWString(ite->second).c_str());
		m_StartList.SetItemText(dwIndex, 2, L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");
		m_StartList.SetItemText(dwIndex, 3, L"HKEY_LOCAL_MACHINE");
		dwIndex++;
	}
	for (auto ite = s3.begin(); ite != s3.end(); ite++) {
		m_StartList.InsertItem(dwIndex, WindowsAxy::StringToWString(ite->first).c_str());
		m_StartList.SetItemText(dwIndex, 1, WindowsAxy::StringToWString(ite->second).c_str());
		m_StartList.SetItemText(dwIndex, 2, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
		m_StartList.SetItemText(dwIndex, 3, L"HKEY_LOCAL_MACHINE");
		dwIndex++;
	}
	for (auto ite = s2.begin(); ite != s2.end(); ite++) {
		m_StartList.InsertItem(dwIndex, WindowsAxy::StringToWString(ite->first).c_str());
		m_StartList.SetItemText(dwIndex, 1, WindowsAxy::StringToWString(ite->second).c_str());
		m_StartList.SetItemText(dwIndex, 2, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
		m_StartList.SetItemText(dwIndex, 3, L"HKEY_CURRENT_USER");
		dwIndex++;
	}

}

void CPageStart::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_StartList);
}


BEGIN_MESSAGE_MAP(CPageStart, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32793, &CPageStart::OnFlush)
	ON_COMMAND(ID_32792, &CPageStart::OnAdd)
	ON_COMMAND(ID_32791, &CPageStart::OnDelete)
END_MESSAGE_MAP()


// CPageStart 消息处理程序


void CPageStart::OnContextMenu(CWnd* /*pWnd*/, CPoint pt) {
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU6);
	CMenu* pPopMenu = menu.GetSubMenu(0);
	::TrackPopupMenu(pPopMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);;
}


void CPageStart::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	InitStart();
}


void CPageStart::OnAdd() {
	// TODO: 在此添加命令处理程序代码

	CFileDialog cFileDlg(TRUE, NULL, NULL, NULL, L"EXE|*.exe||", this);
	cFileDlg.DoModal();
	CString szPath = cFileDlg.GetPathName();
	CString csName = cFileDlg.GetFileName();

	SetAutoRun(szPath.GetBuffer(), csName);
}

void CPageStart::SetAutoRun(LPCWSTR szFilePath, CString csName) {
	HKEY hKey;
	CString strRegPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, strRegPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		RegSetValueExW(hKey, csName, 0, REG_SZ, (LPBYTE)szFilePath, (lstrlen(szFilePath) + 1) * sizeof(TCHAR));
	}
	RegCloseKey(hKey);
	InitStart();
}


void CPageStart::OnDelete() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_StartList.GetFirstSelectedItemPosition() - 1;
	CString csPath = m_StartList.GetItemText(nPos, 2);
	CString csName = m_StartList.GetItemText(nPos, 0);
	CString csRoot = m_StartList.GetItemText(nPos, 3);
	HKEY hRoot;;
	if (csRoot == L"HKEY_LOCAL_MACHINE") {
		hRoot = HKEY_LOCAL_MACHINE;
	}
	else {
		hRoot = HKEY_CURRENT_USER;
	}
	HKEY hKey;
	CStringA strRegPath = WindowsAxy::wchar_tToString(csPath.GetBuffer()).c_str();

	if (RegOpenKeyExA(hRoot, strRegPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		RegDeleteValueW(hKey, csName);
	}
	RegCloseKey(hKey);
	InitStart();
}
