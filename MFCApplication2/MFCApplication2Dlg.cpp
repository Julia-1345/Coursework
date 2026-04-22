
// MFCApplication2Dlg.cpp: файл реализации
//
#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

// Реализация
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


// Диалоговое окно CMFCApplication2Dlg
CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent), m_textColor(RGB(0, 0, 0))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_STN_CLICKED(IDC_STATIC_RESULT, &CMFCApplication2Dlg::OnStnClickedStaticResult)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CMFCApplication2Dlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication2Dlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_QUESTION, &CMFCApplication2Dlg::OnEnChangeEditQuestion)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplication2Dlg
BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	return TRUE;  
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CMFCApplication2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT) 
	{
		pDC->SetTextColor(m_textColor); 
		pDC->SetBkMode(TRANSPARENT);    
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void CMFCApplication2Dlg::OnBnClickedButton1()
{
	CString question;
	GetDlgItemText(IDC_EDIT_QUESTION, question);

	if (question.IsEmpty())
	{
		SetDlgItemText(IDC_STATIC_RESULT, L"Введите вопрос для анализа");
		return;
	}

	question.MakeLower();

	auto CalculateQ = [](const CString& q) -> int
		{
			int L = q.GetLength();
			int G = 0;
			int C = 0;

			std::vector<CString> weakWords =
			{
				L"любимый", L"цвет", L"еда", L"блюдо",
				L"кличка", L"животное", L"фильм", L"песня",
				L"музыка", L"город"
			};

			std::vector<CString> specificWords =
			{
				L"улица", L"город", L"школа", L"страна", L"год", L"дата"
			};

			for (auto word : weakWords)
				if (q.Find(word) != -1) G++;

			for (auto word : specificWords)
				if (q.Find(word) != -1) C++;

			for (int i = 0; i < q.GetLength(); i++)
				if (q[i] >= L'0' && q[i] <= L'9')
				{
					C++;
					break;
				}

			int Q = (L / 10) + (2 * C) - (2 * G);
			if (Q < 0) Q = 0;

			return Q;
		};

	int Q = CalculateQ(question);

	CString result;
	CString recommendations;

	if (Q <= 1)
	{
		result = L"Оценка: Низкая надежность";
		m_textColor = RGB(255, 0, 0); 
		recommendations = L"Рекомендации:\r\n- Сделайте вопрос длиннее\r\n- Добавьте год или дату события\r\n- Укажите место (город, улицу, школу)\r\n- Добавьте уникальное событие, известное только вам";
	}
	else if (Q <= 4)
	{
		result = L"Оценка: Средняя надежность";
		m_textColor = RGB(255, 165, 0);
		recommendations = L"Рекомендации:\r\n- Добавьте больше конкретики\r\n- Избегайте общих слов (любимый, цвет, еда и т.д.)";
	}
	else
	{
		result = L"Оценка: Высокая надежность";
		m_textColor = RGB(0, 128, 0); 
		recommendations = L"Вопрос достаточно надёжен.";
	}

	SetDlgItemText(IDC_STATIC_RESULT, result + L"\r\n" + recommendations);
	Invalidate();
}

void CMFCApplication2Dlg::OnStnClickedStaticResult()
{
	// TODO: добавьте свой код обработчика уведомлений
}

void CMFCApplication2Dlg::OnBnClickedButtonFile()
{
	CFileDialog dlg(TRUE, L"txt", NULL, OFN_FILEMUSTEXIST, L"Text files (*.txt)|*.txt||");

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();

		CStdioFile file;
		CString allResults;

		if (file.Open(path, CFile::modeRead | CFile::typeText))
		{
			CString line;

			while (file.ReadString(line))
			{
				if (line.IsEmpty()) continue;

				line.MakeLower();

				auto CalculateQ = [](const CString& q) -> int
					{
						int L = q.GetLength();
						int G = 0;
						int C = 0;

						std::vector<CString> weakWords =
						{
							L"любимый", L"цвет", L"еда", L"блюдо",
							L"кличка", L"животное", L"фильм", L"песня",
							L"музыка", L"город"
						};

						std::vector<CString> specificWords =
						{
							L"улица", L"город", L"школа", L"страна", L"год", L"дата"
						};

						for (auto word : weakWords)
							if (q.Find(word) != -1) G++;

						for (auto word : specificWords)
							if (q.Find(word) != -1) C++;

						for (int i = 0; i < q.GetLength(); i++)
							if (q[i] >= L'0' && q[i] <= L'9')
							{
								C++;
								break;
							}

						int Q = (L / 10) + (2 * C) - (2 * G);
						if (Q < 0) Q = 0;

						return Q;
					};

				int Q = CalculateQ(line);

				CString result;
				CString advice;

				if (Q <= 1)
				{
					result = L"Низкая надежность";
					advice = L"Рекомендации: сделайте вопрос длиннее, добавьте год, место или уникальное событие.";
					m_textColor = RGB(255, 0, 0);
				}
				else if (Q <= 4)
				{
					result = L"Средняя надежность";
					advice = L"Рекомендации: добавьте больше конкретики, избегайте общих слов.";
					m_textColor = RGB(255, 165, 0);
				}
				else
				{
					result = L"Высокая надежность";
					advice = L"Вопрос достаточно надёжен.";
					m_textColor = RGB(0, 128, 0);
				}

				allResults += L"Вопрос: " + line + L"\r\n" +
					L"Оценка: " + result + L"\r\n" +
					advice + L"\r\n\r\n";
			}

			file.Close();
		}

		SetDlgItemText(IDC_STATIC_RESULT, allResults);

		Invalidate();
	}
}

void CMFCApplication2Dlg::OnBnClickedCancel()
{
	SetDlgItemText(IDC_EDIT_QUESTION, L"");
	SetDlgItemText(IDC_STATIC_RESULT, L"");
}

void CMFCApplication2Dlg::OnEnChangeEditQuestion()
{
	// TODO:  Добавьте код элемента управления
}
