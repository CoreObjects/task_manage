// CPageModule.cpp: 实现文件
//

#include "pch.h"
#include "任务管理器.h"
#include "CPageModule.h"
#include "afxdialogex.h"


// CPageModule 对话框

IMPLEMENT_DYNAMIC(CPageModule, CDialogEx)

CPageModule::CPageModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CPageModule::~CPageModule()
{
}

BOOL CPageModule::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SetWindowTextW(L"模块列表");
	m_ModuleList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ModuleList.InsertColumn(0, L"模块名", 0, 100);
	m_ModuleList.InsertColumn(1, L"模块路径", 0, 300);
	m_ModuleList.InsertColumn(2, L"模块基址", 0, 150);
	m_ModuleList.InsertColumn(3, L"模块大小", 0, 100);
	InitModuleList();
	return TRUE;
}

void CPageModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ModuleList);
}


void CPageModule::InitModuleList() {
	m_ModuleList.DeleteAllItems();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, dwProcessId);
	if (hSnap != NULL) {
		MODULEENTRY32 me32 = { sizeof(me32) };
		BOOL bRet = Module32First(hSnap, &me32);
		DWORD dwIndex = 0;
		while (bRet) {
			m_ModuleList.InsertItem(dwIndex, me32.szModule);
			m_ModuleList.SetItemText(dwIndex, 1, me32.szExePath);
			CString csBaseAddr;
			csBaseAddr.Format(L"0x%p", me32.modBaseAddr);
			m_ModuleList.SetItemText(dwIndex, 2, csBaseAddr);
			CString csBaseSize;
			csBaseSize.Format(L"0x%08X", me32.modBaseSize);
			m_ModuleList.SetItemText(dwIndex, 3, csBaseSize);
			bRet = Module32Next(hSnap, &me32);
			dwIndex++;
		}
		CloseHandle(hSnap);
	}
}

BEGIN_MESSAGE_MAP(CPageModule, CDialogEx)
	ON_COMMAND(ID_32781, &CPageModule::OnFreeDll)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32782, &CPageModule::OnFlush)
END_MESSAGE_MAP()


// CPageModule 消息处理程序


void CPageModule::OnFreeDll() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ModuleList.GetFirstSelectedItemPosition() - 1;
	CString csModulePath = m_ModuleList.GetItemText(nPos, 1);
	WindowsAxy::RemoteFreelibrary(dwProcessId, csModulePath.GetBuffer());
	InitModuleList();
}


void CPageModule::OnContextMenu(CWnd* /*pWnd*/, CPoint pt) {
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU3);
	CMenu* pPopMenu = menu.GetSubMenu(0);
	::TrackPopupMenu(pPopMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);;
}


void CPageModule::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	InitModuleList();
}
