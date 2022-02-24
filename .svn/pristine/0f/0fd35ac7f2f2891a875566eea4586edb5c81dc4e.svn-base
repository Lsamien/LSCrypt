
// LSCryptProDlg.h : 头文件
//

#pragma once


// CLSCryptProDlg 对话框
class CLSCryptProDlg : public CDialogEx
{
// 构造
public:
	CLSCryptProDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LSCRYPTPRO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	HMODULE m_hmod;
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
	afx_msg void OnBnClickedButReg();
	afx_msg void OnBnClickedButUnreg();
	afx_msg void OnBnClickedExten();
	afx_msg void OnBnClickedReg();
};
