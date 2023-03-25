#pragma once


// CPageModule 对话框

class CPageModule : public CDialogEx
{
	DECLARE_DYNAMIC(CPageModule)

public:
	CPageModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageModule();
	virtual BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitModuleList();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ModuleList;
	DWORD dwProcessId;
	afx_msg void OnFreeDll();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFlush();
};
