#pragma once


// CPageProcess 对话框

class CPageProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CPageProcess)

public:
	CPageProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPageProcess();
	virtual BOOL OnInitDialog();

	void OnInitProcessList();

	void InsertImage(CString csNamePath);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ProcessList;
	CImageList m_ImgList;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKillProcess();
	afx_msg void OnFlush();
	afx_msg void OnThreadList();
	afx_msg void OnOpenModuleList();
	afx_msg void OnInjectdll();
	afx_msg void OnInjectShellcode();
};
