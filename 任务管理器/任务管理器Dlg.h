
// 任务管理器Dlg.h: 头文件
//

#pragma once
#include "CPageProcess.h"
#include "CPageSoftware.h"
#include "CPageWnd.h"
#include "CPageStart.h"

// C任务管理器Dlg 对话框
class C任务管理器Dlg : public CDialogEx
{
// 构造
public:
	C任务管理器Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	CPageProcess page1;
	CPageWnd page2;
	CPageSoftware page3;
	CPageStart page4;
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
};
