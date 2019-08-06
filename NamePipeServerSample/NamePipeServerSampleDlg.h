
// NamePipeServerSampleDlg.h: 头文件
//

#pragma once


// CNamePipeServerSampleDlg 对话框
UINT ListenProc(LPVOID lpVoid);
class CNamePipeServerSampleDlg : public CDialog
{
// 构造
public:
	CNamePipeServerSampleDlg(CWnd* pParent = nullptr);	// 标准构造函数
	HANDLE m_hPipe;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAMEPIPESERVERSAMPLE_DIALOG };
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
	CEdit m_editLog;
	CString m_Log;
	afx_msg void OnButtonListen();
};
