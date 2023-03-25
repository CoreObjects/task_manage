// CPageWnd.cpp: 实现文件
//

#include "pch.h"
#include "任务管理器.h"
#include "CPageWnd.h"
#include "afxdialogex.h"


// CPageWnd 对话框

IMPLEMENT_DYNAMIC(CPageWnd, CDialogEx)

CPageWnd::CPageWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CPageWnd::~CPageWnd()
{
}
void CPageWnd::InsertImage(CString csNamePath) {
	HICON hSmallIcon = ::ExtractIcon(NULL, csNamePath, 0);
	if (hSmallIcon != NULL) {
		m_ImgList.Add(hSmallIcon);
	}
	else {
		m_ImgList.Add(LoadIcon(NULL, IDI_APPLICATION));
	}

}
BOOL CPageWnd::OnInitDialog() {
	CDialogEx::OnInitDialog();
	m_ImgList.Create(18, 18, ILC_MASK | ILC_COLOR32, 0, 1);

	m_WndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_WndList.InsertColumn(0, L"窗口名", 0, 300);
	m_WndList.InsertColumn(1, L"运行状态", 0, 60);
	m_WndList.InsertColumn(2, L"进程ID", 0, 50);
	m_WndList.InsertColumn(3, L"窗口句柄", 0, 120);
	m_WndList.InsertColumn(4, L"路径", 0, 600);
	m_WndList.SetImageList(&m_ImgList, LVSIL_SMALL);
	SetWindowText(L"窗口列表");
	InitWindowList();

// 	std::list<WndInfo> wndList = WindowsAxy::GetWindowInfoList();
// 	for (auto ite = wndList.begin(); ite != wndList.end(); ite++){
// 		m_WndList.InsertItem(dwIndex, WindowsAxy::stringToWchar_t(ite->szWndName));
// 		CString str = ite->bFlag ? L"正在运行" : L"未响应";
// 		m_WndList.SetItemText(dwIndex, 1, str);
// 		str.Format(L"%d", ite->dwProcessId);
// 		m_WndList.SetItemText(dwIndex, 2, str);
// 		str.Format(L"%p", ite->hWnd);
// 		m_WndList.SetItemText(dwIndex, 3, str);
// 		m_WndList.SetItemText(dwIndex, 4, WindowsAxy::stringToWchar_t(ite->szPath));
// 		CString wcsProcessPath = WindowsAxy::stringToWchar_t(ite->szPath);
// 		InsertImage(wcsProcessPath);
// 		dwIndex++;
// 	}
	return TRUE;
}

void CPageWnd::InitWindowList() {
	CWnd* pWnd = AfxGetMainWnd()->GetWindow(GW_HWNDFIRST);
	DWORD dwIndex = 0;
	while (m_ImgList.Remove(0));
	m_WndList.DeleteAllItems();
	while (pWnd) {
		if (pWnd->IsWindowVisible() && !pWnd->GetOwner()) {
			CString csWindowName;
			pWnd->GetWindowTextW(csWindowName);
			HICON hIcon = pWnd->GetIcon(FALSE);
			csWindowName.TrimLeft();
			csWindowName.TrimRight();
			if (!csWindowName.IsEmpty()) {
				DWORD dwProcessId;
				GetWindowThreadProcessId(pWnd->m_hWnd, &dwProcessId);
				CString csProcessId;
				csProcessId.Format(L"%d", dwProcessId);
				CString csHWND;
				csHWND.Format(L"%p", pWnd->m_hWnd);
				WCHAR wcsProcessPath[MAX_PATH];
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
				GetModuleFileNameEx(hProcess, NULL, wcsProcessPath, MAX_PATH);
				CloseHandle(hProcess);
				m_WndList.InsertItem(dwIndex, csWindowName, dwIndex);
				if (IsHungAppWindow(pWnd->m_hWnd)) {
					m_WndList.SetItemText(dwIndex, 1, L"未响应");
				}
				else {
					m_WndList.SetItemText(dwIndex, 1, L"正在运行");
				}
				m_WndList.SetItemText(dwIndex, 2, csProcessId);
				m_WndList.SetItemText(dwIndex, 3, csHWND);
				m_WndList.SetItemText(dwIndex, 4, wcsProcessPath);
				//				InsertImage(wcsProcessPath);
				if (hIcon) {
					m_ImgList.Add(hIcon);
				}
				else {
					InsertImage(wcsProcessPath);
				}
				dwIndex++;
			}
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CPageWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_WndList);
}


BEGIN_MESSAGE_MAP(CPageWnd, CDialogEx)
	ON_COMMAND(ID_32783, &CPageWnd::OnFlush)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32784, &CPageWnd::OnKillProcess)
	ON_COMMAND(ID_32785, &CPageWnd::OnMAX)
	ON_COMMAND(ID_32786, &CPageWnd::OnMin)
	ON_COMMAND(ID_32787, &CPageWnd::OnShow)
	ON_COMMAND(ID_32788, &CPageWnd::OnHide)
END_MESSAGE_MAP()


// CPageWnd 消息处理程序


void CPageWnd::OnFlush() {
	// TODO: 在此添加命令处理程序代码
	InitWindowList();
}


void CPageWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint pt) {
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenuW(IDR_MENU4);
	CMenu* pPopMenu = menu.GetSubMenu(0);
	::TrackPopupMenu(pPopMenu->m_hMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);;
}


void CPageWnd::OnKillProcess() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WndList.GetFirstSelectedItemPosition() - 1;
	WCHAR csProcessId[MAX_PATH]{ 0 };
	m_WndList.GetItemText(nPos, 2, csProcessId, MAX_PATH);
	int nProcessId = _ttoi(csProcessId);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nProcessId);
	::TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
	InitWindowList();
}


void CPageWnd::OnMAX() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WndList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WndList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_MAXIMIZE);
}


void CPageWnd::OnMin() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WndList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WndList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_MINIMIZE);
}


void CPageWnd::OnShow() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WndList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WndList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_SHOW);
}


void CPageWnd::OnHide() {
	// TODO: 在此添加命令处理程序代码
	int nPos = (int)m_WndList.GetFirstSelectedItemPosition() - 1;
	CString csWnd = m_WndList.GetItemText(nPos, 3);
	DWORD dwWnd = wcstol(csWnd, NULL, 16);
	ShowWindowAsync((HWND)dwWnd, SW_HIDE);
}
