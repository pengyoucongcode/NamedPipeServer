
// NamePipeServerSampleDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "NamePipeServerSample.h"
#include "NamePipeServerSampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNamePipeServerSampleDlg 对话框



CNamePipeServerSampleDlg::CNamePipeServerSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_NAMEPIPESERVERSAMPLE_DIALOG, pParent)
	, m_Log(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNamePipeServerSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Text(pDX, IDC_EDIT_LOG, m_Log);
}

BEGIN_MESSAGE_MAP(CNamePipeServerSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, &CNamePipeServerSampleDlg::OnButtonListen)
END_MESSAGE_MAP()


// CNamePipeServerSampleDlg 消息处理程序

BOOL CNamePipeServerSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNamePipeServerSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNamePipeServerSampleDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNamePipeServerSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNamePipeServerSampleDlg::OnButtonListen()
{
	// TODO: 在此添加控件通知处理程序代码
	// 创建命名管道
	m_hPipe = CreateNamedPipe("\\\\.\\Pipe\\NamePipeSample", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 0, 0, 1000, NULL);
	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		m_Log = "创建命名管道失败！\r\n";
		UpdateData(FALSE);
		return;
	}
	else
	{
		m_Log = "创建命名管道成功！\r\n";
		UpdateData(FALSE);
		AfxBeginThread(ListenProc, this);
	}
}
// 监听线程函数
UINT ListenProc(LPVOID lpVoid)
{
	char buffer[1024];	// 数据缓存
	DWORD dwReadBytes;	// 定义读取数据的字节个数的变量
	// 获取对话框句柄
	CNamePipeServerSampleDlg* pDlg = (CNamePipeServerSampleDlg*)lpVoid;
	if (ConnectNamedPipe(pDlg->m_hPipe, NULL) == FALSE)
	{
		// 等待客户端连接
		CloseHandle(pDlg->m_hPipe);
		pDlg->m_Log += "与命名管道客户端建立连接失败！\r\n";
		pDlg->m_editLog.SetWindowText(pDlg->m_Log);
		return 0;
	}
	else
	{
		pDlg->m_Log += "与命名管道客户端建立连接！\r\n";
		pDlg->m_editLog.SetWindowText(pDlg->m_Log);
	}
	if (ReadFile(pDlg->m_hPipe, buffer, sizeof(buffer), &dwReadBytes, NULL) == FALSE)
	{
		CloseHandle(pDlg->m_hPipe);
		pDlg->m_Log += "从管道读取数据失败！\r\n";
	}
	else
	{
		buffer[dwReadBytes] = '\0';
		pDlg->m_Log += "接收到客户端命名管道发送的数据=\r\n";
		pDlg->m_Log += CString(buffer);
	}
	if (DisconnectNamedPipe(pDlg->m_hPipe) == FALSE)
		pDlg->m_Log += "终止连接失败！\r\n";
	else
	{
		CloseHandle(pDlg->m_hPipe);
		pDlg->m_Log += "终止连接成功！\r\n";
	}
	pDlg->m_editLog.SetWindowText(pDlg->m_Log);
	return 1;
}
