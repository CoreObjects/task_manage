#pragma once


// CPageStart 对话框

class CPageStart : public CDialogEx
{
	DECLARE_DYNAMIC(CPageStart)

public:
	CPageStart(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageStart();
	virtual BOOL OnInitDialog();

	void InitStart();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_StartList;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFlush();
	afx_msg void OnAdd();


	void SetAutoRun(LPCWSTR szFilePath, CString csName);
	afx_msg void OnDelete();
};
