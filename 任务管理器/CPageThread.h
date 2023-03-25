#pragma once


// CPageThread 对话框

class CPageThread : public CDialogEx
{
	DECLARE_DYNAMIC(CPageThread)

public:
	CPageThread(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageThread();
	virtual BOOL OnInitDialog();

	void InitThreadList();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD dwProcessID;
	CString csProcessName;
	CListCtrl m_ThreadList;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSuspend();
	afx_msg void OnKill();
	afx_msg void OnResume();
	afx_msg void OnFlush();
};
