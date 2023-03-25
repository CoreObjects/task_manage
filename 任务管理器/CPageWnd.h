#pragma once


// CPageWnd 对话框

class CPageWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CPageWnd)

public:
	CPageWnd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageWnd();
	void InsertImage(CString csNamePath);
	virtual BOOL OnInitDialog();

	void InitWindowList();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_WndList;
	CImageList m_ImgList;
	afx_msg void OnFlush();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKillProcess();
	afx_msg void OnMAX();
	afx_msg void OnMin();
	afx_msg void OnShow();
	afx_msg void OnHide();
};
