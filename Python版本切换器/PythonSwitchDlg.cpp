
// Python�汾�л���Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PythonSwitch.h"
#include "PythonSwitchDlg.h"
#include "afxdialogex.h"
#include "RegOperator.h"
#include "APythonInfoItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPython�汾�л���Dlg �Ի���




CPythonSwitchDlg::CPythonSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPythonSwitchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPythonSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PYTHONS, m_ctrlPythonList);
}


BEGIN_MESSAGE_MAP(CPythonSwitchDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ADD_GREEN, &CPythonSwitchDlg::OnBnClickedBtnAddGreen)
END_MESSAGE_MAP()

BOOL CALLBACK OnSettingRect(CListCtrlCustom *pListCtrl, CWnd *pCtrl, CRect &rcGrid)
{
#if 0
	//ʹ��Ĭ�ϵĴ���ʽ����������������
	return FALSE;
#else
	if (pCtrl->IsKindOf(RUNTIME_CLASS(APythonInfoItem)))
	{
		CRect rcList;
		pListCtrl->GetWindowRect(&rcList);

		rcGrid.right = rcGrid.left + rcList.Width() - 25;//25Ϊ�������Ŀ��
	}

	return FALSE;
#endif
}

// CPython�汾�л���Dlg ��Ϣ�������

BOOL CPythonSwitchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//  ��չ��ʽ
	DWORD dwExStyle = LVS_EX_FULLROWSELECT;
	m_ctrlPythonList.SetExtendedStyle(dwExStyle);
	m_ctrlPythonList.RegResizeEvent(OnSettingRect);
	m_ctrlPythonList.SetRowHeight(25);
	m_ctrlPythonList.ShowWindow(SW_SHOW);

	m_ctrlPythonList.GetHeaderCtrl()->EnableWindow(FALSE);

	m_ctrlPythonList.InsertColumn(m_ctrlPythonList.GetHeaderCtrl()->GetItemCount(), _T("�汾"), LVCFMT_CENTER, 100);
	m_ctrlPythonList.InsertColumn(m_ctrlPythonList.GetHeaderCtrl()->GetItemCount(), _T("·��"), LVCFMT_CENTER, 340);

	searchInstalledPythonFromReg();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPythonSwitchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPythonSwitchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPythonSwitchDlg::searchInstalledPythonFromReg()
{
	{//
		CRegOperator ro;
		CString sSubKey = TEXT("SOFTWARE\\Python\\PythonCore");
		if (ro.SetHKEY(TEXT("HKEY_LOCAL_MACHINE")) && 
			ro.OpenKey(sSubKey, true))
		{
			std::vector<CString> keys;
			ro.EnumSub(&keys, NULL);
			for (int i=0; i<keys.size(); ++i)
			{
				CRegOperator reg;
				if (!reg.SetHKEY(TEXT("HKEY_LOCAL_MACHINE")) || 
					!reg.OpenKey(sSubKey + TEXT("\\") + keys[i] + TEXT("\\InstallPath"), true))
				{
					continue;
				}

				CString sPyPath;
				reg.Read(TEXT(""), &sPyPath);
				
				APythonInfoItem* pDlg = new APythonInfoItem;
				pDlg->Create(APythonInfoItem::IDD, &m_ctrlPythonList);
				pDlg->SetList(&m_ctrlPythonList);

				CRect rcCtrl;
				pDlg->GetWindowRect(&rcCtrl);
				m_ctrlPythonList.SetRowHeight(rcCtrl.Height());
				
				int nRow = m_ctrlPythonList.InsertItem(m_ctrlPythonList.GetItemCount(), TEXT(""));
				m_ctrlPythonList.SetItemExCtrl(nRow, 0, pDlg);

				PyINFO pi;
				pi.sVer = keys[i];
				pi.sPath = sPyPath;
				pDlg->SetPyInfo(pi);

				m_paths.push_back(sPyPath);
			}
		}
	}

	{
		CRegOperator ro;
		CString sSubKey = TEXT("Software\\Python\\PythonCore");
		if (ro.SetHKEY(TEXT("HKEY_CURRENT_USER")) && 
			ro.OpenKey(sSubKey, true))
		{
			std::vector<CString> keys;
			ro.EnumSub(&keys, NULL);
			for (int i=0; i<keys.size(); ++i)
			{
				CRegOperator reg;
				if (!reg.SetHKEY(TEXT("HKEY_CURRENT_USER")) || 
					!reg.OpenKey(sSubKey + TEXT("\\") + keys[i] + TEXT("\\InstallPath"), true))
				{
					continue;
				}

				CString sPyPath;
				reg.Read(TEXT(""), &sPyPath);

				APythonInfoItem* pDlg = new APythonInfoItem;
				pDlg->Create(APythonInfoItem::IDD, &m_ctrlPythonList);
				pDlg->SetList(&m_ctrlPythonList);

				CRect rcCtrl;
				pDlg->GetWindowRect(&rcCtrl);
				m_ctrlPythonList.SetRowHeight(rcCtrl.Height());

				int nRow = m_ctrlPythonList.InsertItem(m_ctrlPythonList.GetItemCount(), TEXT(""));
				m_ctrlPythonList.SetItemExCtrl(nRow, 0, pDlg);

				PyINFO pi;
				pi.sVer = keys[i];
				pi.sPath = sPyPath;
				pDlg->SetPyInfo(pi);

				m_paths.push_back(sPyPath);
			}
		}
	}
}

void CPythonSwitchDlg::OnBnClickedBtnAddGreen()
{
	CString filter;  
	filter = TEXT("python.exe|*.exe|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);  
	if(dlg.DoModal()==IDOK)  
	{ 		
		APythonInfoItem* pDlg = new APythonInfoItem;
		pDlg->Create(APythonInfoItem::IDD, &m_ctrlPythonList);
		pDlg->SetList(&m_ctrlPythonList);

		CRect rcCtrl;
		pDlg->GetWindowRect(&rcCtrl);
		m_ctrlPythonList.SetRowHeight(rcCtrl.Height());

		int nRow = m_ctrlPythonList.InsertItem(m_ctrlPythonList.GetItemCount(), TEXT(""));
		m_ctrlPythonList.SetItemExCtrl(nRow, 0, pDlg);

		PyINFO pi;
		pi.sVer = TEXT("δ֪");
		pi.sPath = dlg.GetFolderPath() + '\\';
		pDlg->SetPyInfo(pi);

		m_paths.push_back(pi.sPath);
	}  
}
