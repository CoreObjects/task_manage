#pragma once


// CPageSoftware 对话框

class CPageSoftware : public CDialogEx
{
	DECLARE_DYNAMIC(CPageSoftware)

public:
	CPageSoftware(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageSoftware();
	virtual BOOL OnInitDialog();

	void InitSoftware();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Software;
	afx_msg void OnFree();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnFlush();
};
