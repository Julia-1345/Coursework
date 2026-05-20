#pragma once
#include "afxrich.h"
#include <vector>

struct HistoryItem
{
	CString question;
	CString answer;
};

class CMFCApplication2Dlg : public CDialogEx
{
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	COLORREF GetColorByQ(int Q);
	void     AppendColoredText(const CString& text, COLORREF color); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnLbnSelchangeListHistory();

	DECLARE_MESSAGE_MAP()

public:
	CRichEditCtrl m_resultEdit;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetColoredText(const CString& text, COLORREF color);

	void AddToHistory(const CString& question, const CString& answer);

	std::vector<HistoryItem> m_historyData;
};