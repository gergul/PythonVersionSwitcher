
// Python版本切换器Dlg.cpp : 实现文件
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


// CPython版本切换器Dlg 对话框




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
	//使用默认的处理方式，既填满整个格子
	return FALSE;
#else
	if (pCtrl->IsKindOf(RUNTIME_CLASS(APythonInfoItem)))
	{
		CRect rcList;
		pListCtrl->GetWindowRect(&rcList);

		rcGrid.right = rcGrid.left + rcList.Width() - 25;//25为滚动条的宽度
	}

	return FALSE;
#endif
}

// CPython版本切换器Dlg 消息处理程序

BOOL CPythonSwitchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//  扩展样式
	DWORD dwExStyle = LVS_EX_FULLROWSELECT;
	m_ctrlPythonList.SetExtendedStyle(dwExStyle);
	m_ctrlPythonList.RegResizeEvent(OnSettingRect);
	m_ctrlPythonList.SetRowHeight(25);
	m_ctrlPythonList.ShowWindow(SW_SHOW);

	m_ctrlPythonList.GetHeaderCtrl()->EnableWindow(FALSE);

	m_ctrlPythonList.InsertColumn(m_ctrlPythonList.GetHeaderCtrl()->GetItemCount(), _T("版本"), LVCFMT_CENTER, 100);
	m_ctrlPythonList.InsertColumn(m_ctrlPythonList.GetHeaderCtrl()->GetItemCount(), _T("路径"), LVCFMT_CENTER, 340);

	searchInstalledPythonFromReg();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPythonSwitchDlg::OnPaint()
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
		pi.sVer = TEXT("未知");
		pi.sPath = dlg.GetFolderPath() + '\\';
		pDlg->SetPyInfo(pi);

		m_paths.push_back(pi.sPath);
	}  
}
