// CPageSoftware.cpp: 实现文件
//

#include "pch.h"
#include "任务管理器.h"
#include "CPageSoftware.h"
#include "afxdialogex.h"


// CPageSoftware 对话框

IMPLEMENT_DYNAMIC(CPageSoftware, CDialogEx)

CPageSoftware::CPageSoftware(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CPageSoftware::~CPageSoftware()
{
}
/*
struct InstallInfo {
	std::string szName;       //名称
	std::string szVersion;        //软件版本号
	std::string szPublish;     //软件发布厂商
	std::string szInstallData;       //软件安装日期
	std::string szInstallPath;  //软件安装路径
	std::string szUnistallPath; //软件卸载路径
};
*/
BOOL CPageSoftware::OnInitDialog() {
	CDialogEx::OnInitDialog();
	m_Software.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_Software.InsertColumn(0, L"软件名称", 0, 100);
	m_Software.InsertColumn(1, L"软件版本号", 0, 80);
	m_Software.InsertColumn(2, L"软件发布厂商", 0, 80);
	m_Software.InsertColumn(3, L"软件安装日期", 0, 80);
	m_Software.InsertColumn(4, L"软件安装路径", 0, 200);
	m_Software.InsertColumn(5, L"软件卸载路径", 0, 450);
	InitSoftware();


	return TRUE;
}

void CPageSoftware::InitSoftware() {
	m_Software.DeleteAllItems();
	std::list<InstallInfo> stcList = WindowsAxy::GetInstallInfoList();
	DWORD dwIndex = 0;
	for (auto ite = stcList.begin(); ite != stcList.end(); ite++) {
		m_Software.InsertItem(dwIndex, WindowsAxy::StringToWString(ite->szName).c_str());
		m_Software.SetItemText(dwIndex, 1, WindowsAxy::StringToWString(ite->szVersion).c_str());
		m_Software.SetItemText(dwIndex, 2, WindowsAxy::StringToWString(ite->szPublish).c_str());
		m_Software.SetItemText(dwIndex, 3, WindowsAxy::StringToWString(ite->szInstallData).c_str());
		m_Software.SetItemText(dwIndex, 4, WindowsAxy::StringToWString(ite->szInstallPath).c_str());
		m_Software.SetItemText(dwIndex, 5, WindowsAxy::StringToWString(ite->szUnistallPath).c_str());
		dwIndex++;
	}
}

void CPageSoftware::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Software);
}


BEGIN_MESSAGE_MAP(CPageSoftware, CDialogEx)
	ON_COMMAND(ID_32789, &CPageSoftware::OnFree)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_Menu, &CPageSoftware::OnFlush)
END_MESSAGE_MAP()


// CPageSoftware 消息处理程序





void CPageSoftware::OnFree() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_Software.GetFirstSelectedItemPosition() - 1;
	WCHAR strPath[MAX_PATH]{ 0 };
	m_Software.GetItemText(nPos, 5, strPath, MAX_PATH);
	std::string str = WindowsAxy::wchar_tToString(strPath);
	if (str[0] == '\"') {
		system(str.c_str());
	}
	else {
		str.insert(str.begin(), '\"');
		str.push_back('\"');
		system(str.c_str());
	}
	
}


void CPageSoftware::OnContextMenu(CWnd* /*pWnd*/, CPoint pt) {
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU5);
	CMenu* pPopMenu = menu.GetSubMenu(0);
	::TrackPopupMenu(pPopMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);;
}


void CPageSoftware::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	InitSoftware();
}
