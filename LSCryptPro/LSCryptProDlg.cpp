
// LSCryptProDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LSCryptPro.h"
#include "LSCryptProDlg.h"
#include "afxdialogex.h"
#include "LSCryptinc.h"
#include "RegEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLSCryptProDlg 对话框



CLSCryptProDlg::CLSCryptProDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLSCryptProDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLSCryptProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLSCryptProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_REG, &CLSCryptProDlg::OnBnClickedButReg)
	ON_BN_CLICKED(IDC_BUT_UNREG, &CLSCryptProDlg::OnBnClickedButUnreg)
	ON_BN_CLICKED(IDC_EXTEN, &CLSCryptProDlg::OnBnClickedExten)
	ON_BN_CLICKED(IDC_REG, &CLSCryptProDlg::OnBnClickedReg)
END_MESSAGE_MAP()


// CLSCryptProDlg 消息处理程序

BOOL CLSCryptProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CButton * pButReg = (CButton *)GetDlgItem(IDC_REG);
	if(LoadDLL())
	{
		if(fnDLLCheckeRegister())
		{
			pButReg->SetCheck(1);
		}
	}
	UnLoadDLL();
	CButton * pBut = (CButton *)GetDlgItem(IDC_EXTEN);
	if(AssocUFCCheck(HKEY_CLASSES_ROOT, NULL))
	{
		pBut->SetCheck(1);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLSCryptProDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLSCryptProDlg::OnPaint()
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
HCURSOR CLSCryptProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLSCryptProDlg::OnBnClickedButReg()
{
	// TODO: Add your control notification handler code here
	if(LoadDLL())
	{
		if(fnDLLCheckeRegister())
		{
			fnDllUnregisterServer();
		}else
		{
			fnDllRegisterServer();
		}
	}
	UnLoadDLL();
}


void CLSCryptProDlg::OnBnClickedButUnreg()
{
	// TODO: Add your control notification handler code here
	if(LoadDLL())
		fnDllUnregisterServer();
	UnLoadDLL();
}


void CLSCryptProDlg::OnBnClickedExten()
{
	// TODO: Add your control notification handler code here
	CButton * pBut = (CButton *)GetDlgItem(IDC_EXTEN);
	int icheck = pBut->GetCheck();
	if (icheck)
	{
		if(AssocEx(_T("LSC")))
		{
			DOutPut(L"关联扩展名.LSC文件成功\n");
		}
	}
	else
	{
		if(AssocEx(_T("LSC"),FALSE))
		{
			DOutPut(L"取消关联扩展名.LSC文件成功\n");
		}
	}
}


void CLSCryptProDlg::OnBnClickedReg()
{
	// TODO: Add your control notification handler code here
	CButton * pBut = (CButton *)GetDlgItem(IDC_REG);
	int icheck = pBut->GetCheck();
	if(LoadDLL())
	{
		if(icheck)
		{
			fnDllRegisterServer();
		}else
		{
			fnDllUnregisterServer();
		}
		if(fnDLLCheckeRegister()!=icheck)
		{
			DOutPut(L"添加到右键菜单失败\n");
		}else
		{
			DOutPut(L"添加到右键菜单成功\n");
		}
	}
	UnLoadDLL();
}
