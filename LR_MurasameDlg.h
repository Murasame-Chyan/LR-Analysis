
// LR_MurasameDlg.h: 头文件
//

#pragma once


// CLRMurasameDlg 对话框
class CLRMurasameDlg : public CDialogEx
{
// 构造
public:
	CLRMurasameDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LR_MURASAME_DIALOG };
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
	void CLRMurasameDlg_clr_Val();
	void loadMenu();
	void loadList_FirFolo();

public:
	// 文法规则文本
	CString rule;
	// 文本获取路径
	CString rule_Path;
	// firstfollow集 显示控件
	CListCtrl setCtrl;

	afx_msg void OnClickedButtonParse();
};
