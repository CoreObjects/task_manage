
// 任务管理器Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "任务管理器.h"
#include "任务管理器Dlg.h"
#include "afxdialogex.h"
#include "CPageProcess.h"
#include "CPageWnd.h"
#include "CPageSoftware.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C任务管理器Dlg 对话框



C任务管理器Dlg::C任务管理器Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C任务管理器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(C任务管理器Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C任务管理器Dlg::OnSelchangeTab1)
END_MESSAGE_MAP()


// C任务管理器Dlg 消息处理程序

BOOL C任务管理器Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Tab.InsertItem(0, L"进程");
	m_Tab.InsertItem(1, L"窗口");
	m_Tab.InsertItem(2, L"软件");
	m_Tab.InsertItem(3, L"开机启动项");

	page1.Create(IDD_DIALOG1, &m_Tab);
	page2.Create(IDD_DIALOG2, &m_Tab);
	page3.Create(IDD_DIALOG3, &m_Tab);
	page4.Create(IDD_DIALOG6, &m_Tab);
	CRect rs;
	m_Tab.GetClientRect(rs);
	rs.top += 23;
	page1.MoveWindow(rs);
	page2.MoveWindow(rs);
	page3.MoveWindow(rs);
	page3.MoveWindow(rs);
	page4.MoveWindow(rs);
	page1.ShowWindow(SW_SHOW);
	this->SetWindowTextW(L"任务管理器");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C任务管理器Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C任务管理器Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C任务管理器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C任务管理器Dlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) {
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = m_Tab.GetCurSel();
	switch (nCurSel) {
	case 0:page1.ShowWindow(SW_SHOW); page2.ShowWindow(SW_HIDE); page3.ShowWindow(SW_HIDE); page4.ShowWindow(SW_HIDE); break;
	case 1:page1.ShowWindow(SW_HIDE); page2.ShowWindow(SW_SHOW); page3.ShowWindow(SW_HIDE); page4.ShowWindow(SW_HIDE); break;
	case 2:page1.ShowWindow(SW_HIDE); page2.ShowWindow(SW_HIDE); page3.ShowWindow(SW_SHOW); page4.ShowWindow(SW_HIDE); break;
	case 3:page1.ShowWindow(SW_HIDE); page2.ShowWindow(SW_HIDE); page3.ShowWindow(SW_HIDE); page4.ShowWindow(SW_SHOW); break;
	default:
		break;
	}

	*pResult = 0;
}
