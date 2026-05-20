#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string>
#include <afxrich.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

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

CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_RESULT, m_resultEdit);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &CMFCApplication2Dlg::OnBnClickedButtonFile)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMFCApplication2Dlg::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMFCApplication2Dlg::OnBnClickedClear)
    ON_LBN_SELCHANGE(IDC_LIST_HISTORY, &CMFCApplication2Dlg::OnLbnSelchangeListHistory)
END_MESSAGE_MAP()

BOOL CMFCApplication2Dlg::OnInitDialog()
{
    AfxInitRichEdit2();

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

    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_QUESTION);

    if (pEdit)
    {
        pEdit->ModifyStyle(
            0,
            ES_MULTILINE |
            ES_WANTRETURN |
            ES_AUTOVSCROLL |
            WS_VSCROLL
        );
    }

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

        SendMessage(
            WM_ICONERASEBKGND,
            reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
            0
        );

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

BOOL CMFCApplication2Dlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN &&
        pMsg->wParam == VK_RETURN)
    {
        CWnd* focus = GetFocus();

        if (focus &&
            focus->GetDlgCtrlID() == IDC_EDIT_QUESTION)
        {
            focus->SendMessage(WM_CHAR, VK_RETURN, 0);
            return TRUE;
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CMFCApplication2Dlg::SetColoredText(
    const CString& text,
    COLORREF color)
{
    m_resultEdit.SetWindowTextW(text);

    CHARFORMAT2 cf;
    ZeroMemory(&cf, sizeof(cf));

    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = color;

    m_resultEdit.SetSel(0, -1);
    m_resultEdit.SetSelectionCharFormat(cf);
}

void CMFCApplication2Dlg::AddToHistory(const CString& question, const CString& answer)
{
    m_historyData.push_back({ question, answer });

    CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_HISTORY);
    if (!pList) return;

    pList->ResetContent();

    for (int i = 0; i < (int)m_historyData.size(); i++)
    {
        pList->AddString(m_historyData[i].question);
    }
}

COLORREF CMFCApplication2Dlg::GetColorByQ(int Q)
{
    if (Q <= 1)      return RGB(255, 0, 0);
    else if (Q <= 4) return RGB(255, 200, 0);
    else             return RGB(0, 180, 0);
}

void CMFCApplication2Dlg::AppendColoredText(const CString& text, COLORREF color)
{
    long len = m_resultEdit.GetTextLength();
    m_resultEdit.SetSel(len, len);

    CHARFORMAT2 cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = color;

    m_resultEdit.SetSelectionCharFormat(cf);
    m_resultEdit.ReplaceSel(text);
}

void CMFCApplication2Dlg::OnBnClickedButton1()
{
    CString text;
    GetDlgItemText(IDC_EDIT_QUESTION, text);

    if (text.IsEmpty())
    {
        SetDlgItemText(IDC_EDIT_RESULT, L"Введите вопрос для анализа");
        return;
    }

    m_resultEdit.SetWindowTextW(L"");

    int start = 0;
    CString line = text.Tokenize(L"\r\n", start);

    bool hasAnyResult = false;

    while (!line.IsEmpty())
    {
        line.Trim();

        if (line.IsEmpty())
        {
            line = text.Tokenize(L"\r\n", start);
            continue;
        }

        CString originalLine = line;
        CString lower = line;
        CharLower(lower.GetBuffer());
        lower.ReleaseBuffer();

        bool hasLatin = false;
        for (int i = 0; i < lower.GetLength(); i++)
        {
            wchar_t ch = lower[i];
            if ((ch >= L'a' && ch <= L'z'))
            {
                hasLatin = true;
                break;
            }
        }

        if (hasLatin)
        {
            CString errorMsg =
                L"Вопрос: " + originalLine + L"\r\n\r\n" +
                L"Ошибка: вопрос должен быть на русском языке\r\n\r\n";

            AppendColoredText(errorMsg, RGB(180, 0, 180)); 

            line = text.Tokenize(L"\r\n", start);
            continue;
        }

        int L = lower.GetLength();
        if (L > 200) L = 200;

        int G = 0;
        int C = 0;

        std::vector<CString> weakWords =
        {
            L"любимый", L"цвет", L"еда", L"блюдо",
            L"кличка", L"животное", L"фильм",
            L"песня", L"музыка", L"город",
            L"лет", L"возраст", L"сколько", L"старый",
            L"родился", L"рост", L"вес", L"имя",    
            L"день", L"месяц",
        };

        std::vector<CString> specificWords =
        {
            L"улица", L"школа", L"страна", L"дата"
            L"номер", L"адрес", L"учитель", L"больница",
            L"район", 
        };

        for (auto& word : weakWords)
            if (lower.Find(word) != -1) G++;

        for (auto& word : specificWords)
            if (lower.Find(word) != -1) C++;

        for (int i = 0; i < lower.GetLength(); i++)
        {
            if (iswdigit(lower[i]))
            {
                C++;
                break;
            }
        }

        int Q = (L / 10) + (2 * C) - (2 * G);
        if (Q < 0) Q = 0;

        COLORREF color = GetColorByQ(Q);

        CString result, recommendations;

        if (Q <= 1)
        {
            result = L"Низкая надёжность";
            recommendations =
                L"Рекомендации:\r\n"
                L"— Избегайте общих вопросов о предпочтениях\r\n"
                L"— Добавьте конкретный факт из вашей биографии\r\n"
                L"— Используйте название места, даты или события\r\n"
                L"— Хороший пример: «На какой улице была моя первая школа?»";
        }
        else if (Q <= 4)
        {
            result = L"Средняя надёжность";
            recommendations =
                L"Рекомендации:\r\n"
                L"— Добавьте конкретику, известную только вам\r\n"
                L"— Уточните место, дату или имя собственное\r\n"
                L"— Избегайте фактов, которые можно найти в соцсетях\r\n"
                L"— Хороший пример: «Название улицы, где я жил в 2023 году?»";
        }
        else
        {
            result = L"Высокая надёжность";
            recommendations =
                L"Рекомендации:\r\n"
                L"— Вопрос сформулирован хорошо\r\n"
                L"— Убедитесь, что помните ответ точно\r\n"
                L"— Не сообщайте ответ третьим лицам\r\n";
        }
        
        CString answer =
            L"Оценка: " + result + L"\r\n\r\n" +
            recommendations;

        CString header =
            L"Вопрос: " + originalLine + L"\r\n\r\n" +
            L"Оценка: " + result + L"\r\n\r\n";

        CString recs = recommendations + L"\r\n\r\n";

        AppendColoredText(header, color);
        AppendColoredText(recs, RGB(0, 0, 0));

        AddToHistory(originalLine, answer);

        hasAnyResult = true;

        line = text.Tokenize(L"\r\n", start);
    }

    if (!hasAnyResult)
    {
        AppendColoredText(
            L"Нет корректных вопросов для анализа",
            RGB(180, 0, 180)
        );
    }
}

void CMFCApplication2Dlg::OnBnClickedButtonFile()
{
    CFileDialog dlg(
        TRUE,
        L"txt",
        NULL,
        OFN_FILEMUSTEXIST,
        L"Text files (*.txt)|*.txt||"
    );

    if (dlg.DoModal() == IDOK)
    {
        CString path = dlg.GetPathName();
        CFile file;

        if (file.Open(path, CFile::modeRead | CFile::typeBinary))
        {
            UINT len = (UINT)file.GetLength();

            std::vector<char> buffer(len + 1);
            file.Read(buffer.data(), len);
            buffer[len] = '\0';
            file.Close();

            int size = MultiByteToWideChar(
                CP_UTF8,
                0,
                buffer.data(),
                -1,
                NULL,
                0
            );

            CString text;

            MultiByteToWideChar(
                CP_UTF8,
                0,
                buffer.data(),
                -1,
                text.GetBuffer(size),
                size
            );

            text.ReleaseBuffer();

            SetDlgItemText(IDC_EDIT_QUESTION, text);
            OnBnClickedButton1();
        }
    }
}

void CMFCApplication2Dlg::OnBnClickedClear()
{
    SetDlgItemText(IDC_EDIT_QUESTION, L"");
    SetDlgItemText(IDC_EDIT_RESULT, L"");
}

void CMFCApplication2Dlg::OnLbnSelchangeListHistory()
{
    CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_HISTORY);
    if (!pList) return;

    int sel = pList->GetCurSel();
    if (sel == LB_ERR) return;

    if (sel >= 0 && sel < (int)m_historyData.size())
    {
        SetDlgItemText(IDC_EDIT_RESULT,
            m_historyData[sel].answer);
    }
}

void CMFCApplication2Dlg::OnBnClickedButtonSave()
{
    if (m_historyData.empty())
    {
        AfxMessageBox(L"История пуста");
        return;
    }

    CFileDialog dlg(
        FALSE,
        L"txt",
        L"history.txt",
        OFN_OVERWRITEPROMPT,
        L"Text files (*.txt)|*.txt||"
    );

    if (dlg.DoModal() == IDOK)
    {
        // Собираем весь текст в одну строку
        CString allText;
        for (const auto& item : m_historyData)
        {
            allText +=
                L"Вопрос: " + item.question + L"\r\n" +
                item.answer + L"\r\n\r\n";
        }

        // Конвертируем из Unicode в UTF-8
        int size = WideCharToMultiByte(
            CP_UTF8, 0,
            allText, -1,
            NULL, 0,
            NULL, NULL
        );

        std::vector<char> buffer(size);
        WideCharToMultiByte(
            CP_UTF8, 0,
            allText, -1,
            buffer.data(), size,
            NULL, NULL
        );

        CFile file;
        if (file.Open(dlg.GetPathName(),
            CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
        {
            // Пишем UTF-8 BOM
            unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
            file.Write(bom, 3);

            // Пишем текст (без финального нулевого байта)
            file.Write(buffer.data(), size - 1);
            file.Close();

            AfxMessageBox(L"История успешно сохранена");
        }
        else
        {
            AfxMessageBox(L"Ошибка сохранения файла");
        }
    }
}