
// VPNMonitorDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CVPNMonitorDlg dialog
class CVPNMonitorDlg : public CDialogEx
{
// Construction
public:
	CVPNMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VPNMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	long OnNotifyIcon(WPARAM wParam,LPARAM IParam);
	NOTIFYICONDATA NotifyIcon;
	CButton m_cbMonitor;
	~CVPNMonitorDlg();
};
