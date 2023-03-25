// CPageThread.cpp: 实现文件
//

#include "pch.h"
#include "任务管理器.h"
#include "CPageThread.h"
#include "afxdialogex.h"


// CPageThread 对话框

IMPLEMENT_DYNAMIC(CPageThread, CDialogEx)

CPageThread::CPageThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent) {

}

CPageThread::~CPageThread() {
}

BOOL CPageThread::OnInitDialog() {
	CDialogEx::OnInitDialog();
	m_ThreadList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString csTitle;
	csTitle.Format(L"进程%s的线程列表", csProcessName.GetBuffer());
	SetWindowTextW(csTitle);
	m_ThreadList.InsertColumn(0, L"所属进程ID", 0, 100);
	m_ThreadList.InsertColumn(1, L"线程ID", 0, 100);
	m_ThreadList.InsertColumn(1, L"线程优先级", 0, 100);
	InitThreadList();


	return TRUE;
}

void CPageThread::InitThreadList() {
	m_ThreadList.DeleteAllItems();
	std::list<THREADENTRY32> teList;
	WindowsAxy::GetThreadInfoList(teList, dwProcessID);
	DWORD dwIndex = 0;
	for (auto ite = teList.begin(); ite != teList.end(); ite++) {
		CString str;
		str.Format(L"%d", ite->th32OwnerProcessID);
		m_ThreadList.InsertItem(dwIndex, str);
		str.Format(L"%d", ite->tpBasePri);
		m_ThreadList.SetItemText(dwIndex, 1, str);
		str.Format(L"%d", ite->th32ThreadID);
		m_ThreadList.SetItemText(dwIndex, 2, str);
	}
}

void CPageThread::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ThreadList);
}


BEGIN_MESSAGE_MAP(CPageThread, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32777, &CPageThread::OnSuspend)
	ON_COMMAND(ID_32778, &CPageThread::OnKill)
	ON_COMMAND(ID_32779, &CPageThread::OnResume)
	ON_COMMAND(ID_32780, &CPageThread::OnFlush)
END_MESSAGE_MAP()


// CPageThread 消息处理程序


void CPageThread::OnContextMenu(CWnd* /*pWnd*/, CPoint pt) {
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu* pPopMenu = menu.GetSubMenu(0);
	::TrackPopupMenu(pPopMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);;
}


void CPageThread::OnSuspend() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ThreadList.GetFirstSelectedItemPosition() - 1;
	CString cstid = L"Tid:" + m_ThreadList.GetItemText(nPos, 2);
	DWORD dwTid = _ttoi(m_ThreadList.GetItemText(nPos, 2));
	WindowsAxy::SuspendThread(dwTid);
	InitThreadList();
}


void CPageThread::OnKill() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ThreadList.GetFirstSelectedItemPosition() - 1;
	CString cstid = L"Tid:" + m_ThreadList.GetItemText(nPos, 2);
	DWORD dwTid = _ttoi(m_ThreadList.GetItemText(nPos, 2));
	WindowsAxy::KillThread(dwTid);
	InitThreadList();
}


void CPageThread::OnResume() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_ThreadList.GetFirstSelectedItemPosition() - 1;
	/*	CString cstid = L"Tid:" + m_ThreadList.GetItemText(nPos, 1);*/
	DWORD dwTid = _ttoi(m_ThreadList.GetItemText(nPos, 2));
	WindowsAxy::ResumeThread(dwTid);
	InitThreadList();
}


void CPageThread::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	InitThreadList();
}
