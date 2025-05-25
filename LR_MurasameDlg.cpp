
// LR_MurasameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LR_Murasame.h"
#include "LR_MurasameDlg.h"
#include "afxdialogex.h"

#include "grammar.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int READ_BUFF_SIZE = 128;
const int SET_COLUMN_WIDTH[3] = {100, 300, 300};
LR_Grammar lr_grammar;

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
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_FILE_OPEN, &CAboutDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAboutDlg::OnFileSave)
END_MESSAGE_MAP()

// CLRMurasameDlg 对话框
CLRMurasameDlg::CLRMurasameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LR_MURASAME_DIALOG, pParent)
	, rule(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLRMurasameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_rule, rule);
	DDX_Control(pDX, IDC_LIST_set, setCtrl);
}

BEGIN_MESSAGE_MAP(CLRMurasameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CAboutDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CAboutDlg::OnFileSave)
	ON_BN_CLICKED(IDC_BUTTON_parse, &CLRMurasameDlg::OnClickedButtonParse)
END_MESSAGE_MAP()

// CLRMurasameDlg 消息处理程序
BOOL CLRMurasameDlg::OnInitDialog()
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
	// 1. new menu & listctrl
	loadMenu();
	loadList_FirFolo();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLRMurasameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLRMurasameDlg::OnPaint()
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
HCURSOR CLRMurasameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 主程序控件加载
void CLRMurasameDlg::loadMenu()
{
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	SetMenu(&menu);
}
void CLRMurasameDlg::loadList_FirFolo()
{	// 列表网格线
	DWORD listStyle = setCtrl.GetExtendedStyle();
	listStyle |= LVS_EX_FULLROWSELECT;
	listStyle |= LVS_EX_GRIDLINES;
	setCtrl.SetExtendedStyle(listStyle);
	// 列标题
	setCtrl.InsertColumn(0, L"非终结符", LVCFMT_CENTER, SET_COLUMN_WIDTH[0]);
	setCtrl.InsertColumn(1, L"first集", LVCFMT_CENTER, SET_COLUMN_WIDTH[1]);
	setCtrl.InsertColumn(2, L"follow集", LVCFMT_CENTER, SET_COLUMN_WIDTH[2]);
}
// 清理主程序变量
void CLRMurasameDlg::CLRMurasameDlg_clr_Val()
{
	this->rule.Empty();
	this->rule_Path.Empty();
}

// 菜单-打开文件
void CAboutDlg::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	// 1.清除原文
	CLRMurasameDlg* mainDlg = (CLRMurasameDlg*)AfxGetMainWnd();
	mainDlg->CLRMurasameDlg_clr_Val();
	// 执行其他清除（语法只在确保安全时清除 见下 if( read.is_open() ) 处）
	
	mainDlg->UpdateData(FALSE);
	TCHAR fileFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*");
	CFileDialog fileSelector(TRUE, _T("txt"), NULL, 0, fileFilter, this);
	CString c_FilePath;
	if (IDOK == fileSelector.DoModal())
	{
		c_FilePath = fileSelector.GetPathName();
		if (mainDlg)
		{
			mainDlg->rule_Path = c_FilePath;
			char buf[READ_BUFF_SIZE];
			// 逐行获取文件内rule
			std::ifstream read(CW2A(c_FilePath.GetString()));
			if (read.is_open())
			{	// 其他清除
				lr_grammar.grammar_clr_Val();

				while (!read.eof())
				{
					read.getline(buf, READ_BUFF_SIZE - 1);
					mainDlg->rule += CString(buf) + CString("\r\n");
				}
				mainDlg->UpdateData(FALSE);
			}
			else
			{
				MessageBox(_T("打开失败"));
			}
			read.close();
		}
	}
}

// 菜单-保存文件
void CAboutDlg::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	// 获取主窗口
	CLRMurasameDlg* mainDlg = (CLRMurasameDlg*)AfxGetMainWnd();
	mainDlg->UpdateData(TRUE);	// 将用户手写的变量更新到类内
	// 1. 默认扩展名 & 默认文件名
	CString defExtension = L"txt";
	CString defFileName = L"Your_Rule_FileName.txt";
	TCHAR fileFilter[] = _T("文本文件(*.txt)|*.txt");
	CFileDialog fileSavior(FALSE, defExtension, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, fileFilter, NULL);
	CString c_path = _T("");
	if (IDOK == fileSavior.DoModal())
	{
		c_path = fileSavior.GetPathName();
		if (mainDlg)
		{
			std::ofstream write(CW2A(c_path.GetString()));
			if (write.is_open())
			{
				write << CW2A(mainDlg->rule.GetString());
				
				MessageBox(_T("保存成功。"));
			}
			else
			{
				MessageBox(_T("保存失败。"));
			}
			write.close();
		}
	}
}

// 解析按钮
void CLRMurasameDlg::OnClickedButtonParse()
{
	// TODO: 在此添加控件通知处理程序代码
	// 1. 初始化grammar 获取变量
	if (lr_grammar.parseGrammar(rule))
		MessageBox(_T("解析成功"));
	else
		MessageBox(_T("解析失败"));
	
	// 2. 处理first、follow集

}
