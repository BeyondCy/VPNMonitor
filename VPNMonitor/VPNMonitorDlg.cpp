
// VPNMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VPNMonitor.h"
#include "VPNMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CVPNMonitorDlg dialog




CVPNMonitorDlg::CVPNMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVPNMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVPNMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_cbMonitor);
}

BEGIN_MESSAGE_MAP(CVPNMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_NC,&CVPNMonitorDlg::OnNotifyIcon)
	ON_BN_CLICKED(IDC_BUTTON1, &CVPNMonitorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


afx_msg long CVPNMonitorDlg::OnNotifyIcon(WPARAM wParam,LPARAM IParam)
{
	if(IParam==WM_LBUTTONUP)
	{
		ShowWindow(SW_SHOW);
	}
	if(IParam==WM_RBUTTONUP)
	{
		ShowWindow(SW_HIDE);
	}
	return 0;
}
// CVPNMonitorDlg message handlers

BOOL CVPNMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Add notify icon
	NotifyIcon.cbSize=sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hWnd=m_hWnd;
	lstrcpy(NotifyIcon.szTip,_T("VPN Monitor is running..."));
	NotifyIcon.uCallbackMessage=WM_NC;
	NotifyIcon.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&NotifyIcon);

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		 return FALSE;
	 }
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVPNMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVPNMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVPNMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void ToMonitor(void *dlg)
{
	CVPNMonitorDlg *pdlg = (CVPNMonitorDlg *)dlg;
	SOCKET s;
	sockaddr_in *addr = (sockaddr_in*)malloc(sizeof(sockaddr_in));
	while (true){
		WinExec(_T("cmd.exe /c ipconfig /flushdns"), SW_HIDE);
		ZeroMemory(addr, sizeof(sockaddr_in));
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		HOSTENT *h =gethostbyname(_T("www.twitter.com"));
		if (h==NULL)
		{
			MessageBox(NULL, _T("DNS Server down, mointor stop now"), _T("WARMING"), MB_ICONWARNING);
			pdlg->m_cbMonitor.EnableWindow(TRUE);
			pdlg->m_cbMonitor.SetWindowTextA(_T("begin Monitor"));
			pdlg->ShowWindow(SW_SHOW);
			return ;
		}
		addr->sin_family = AF_INET;
		addr->sin_port =htons(80);
		MoveMemory(&addr->sin_addr.S_un.S_addr,h->h_addr_list[0],sizeof(ULONG));
		int result = connect(s, (const sockaddr*)addr, sizeof(sockaddr));
		if (result == SOCKET_ERROR)
		{
			int errorcode = WSAGetLastError();
			MessageBox(NULL, _T("WARMING: YOU VPN IS DOWN, MONITOR IS STOP NOW."), _T("WARMING"), MB_ICONWARNING);
			pdlg->m_cbMonitor.EnableWindow(TRUE);
			pdlg->m_cbMonitor.SetWindowTextA(_T("begin Monitor"));
			pdlg->ShowWindow(SW_SHOW);
			return ;
		}
		closesocket(s);
		Sleep(1000);
		//freeaddrinfo(pdnsinfo);
	}
	
}
void CVPNMonitorDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	m_cbMonitor.SetWindowText(_T("Monitoring..."));
	m_cbMonitor.EnableWindow(FALSE);
	_beginthread(ToMonitor, 0, (void*)this);
}
CVPNMonitorDlg::~CVPNMonitorDlg()
{
	Shell_NotifyIcon(NIM_DELETE,&NotifyIcon);
}

