// APythonInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "PythonSwitch.h"
#include "PythonSwitchDlg.h"
#include "APythonInfoItem.h"
#include "afxdialogex.h"
#include "customlistctrl\listctrlcustom.h"
#include <fstream>


// APythonInfo 对话框

IMPLEMENT_DYNAMIC(APythonInfoItem, CDialogEx)

APythonInfoItem::APythonInfoItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(APythonInfoItem::IDD, pParent)
	, m_pList(NULL)
{

}

APythonInfoItem::~APythonInfoItem()
{
}

void APythonInfoItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VERSION, m_editVersion);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editPath);
}


BOOL APythonInfoItem::OnInitDialog()
{
	BOOL bRet = CDialogEx::OnInitDialog();


	return bRet;
}

BEGIN_MESSAGE_MAP(APythonInfoItem, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_RUN, &APythonInfoItem::OnBnClickedBtnRun)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SETEV, &APythonInfoItem::OnBnClickedBtnSetev)
END_MESSAGE_MAP()


// APythonInfo 消息处理程序


void APythonInfoItem::SetPyInfo(const PyINFO& info)
{
	m_editVersion.SetWindowText(info.sVer);
	m_editPath.SetWindowText(info.sPath);
}

std::string cmd;
unsigned _stdcall CreatePyConsoleProc(void* param)
{
	//system("%comspec% /k _path_.bat");
	cmd = "%comspec% /k \"" + cmd + "\"";
	system(cmd.c_str());
	return 0;
}

void APythonInfoItem::OnBnClickedBtnRun()
{
	//std::ofstream outfile("_path_.bat", std::ios::ate);   //打开文件，设置写入方式为覆盖写入

	CString sPath;
	m_editPath.GetWindowText(sPath);
	CString sAllPath = TEXT("set \"Path=");
	sAllPath += sPath + TEXT(";") + sPath + TEXT("\\Scripts\\") + TEXT(";%Path%\"");
#ifdef _UNICODE
	std::string s;
	WideByte2Ansi(s, sAllPath.GetString());
	const char* p = s.c_str();
	DWORD lenWrite = s.length()-sizeof(sAllPath[0]);
#else
	const char* p = sAllPath.GetString();
	DWORD lenWrite = sAllPath.GetLength();
#endif
// 	outfile << p << std::endl;
// 	outfile << "cls" << std::endl;
// 	outfile << "@Python -VER" << std::endl;
// 	outfile.close();

	cmd = p;
	_beginthreadex(NULL, 0, CreatePyConsoleProc, NULL, NULL, NULL);	
}

void APythonInfoItem::OnPaint()
{
	CRect   rect;  
	CPaintDC   dc(this);  
	GetClientRect(rect);  
	dc.FillSolidRect(rect,RGB(255, 255, 255));
}


void APythonInfoItem::OnBnClickedBtnSetev()
{
	CString thisPath;
	m_editPath.GetWindowText(thisPath);
	if (thisPath.IsEmpty())
		return ;

	CString sPathNew;

	std::vector<CString> pyPaths = ((CPythonSwitchDlg*)GetParent())->GetPyPaths();

	TCHAR szPath[102400];	
	DWORD nLen = GetEnvironmentVariable(TEXT("Path"), szPath, 102400);
	szPath[nLen] = '\0';
	CString sPath = szPath;
	CStringArray paths;
	int nPaths = SplitString(sPath, TEXT(";"), paths);
	int nPyPaths = pyPaths.size();
	for (int i=0; i<nPaths; ++i)
	{
		bool bDel = false;
		CString apath = paths[i];
		apath.MakeLower();
		if (apath[apath.GetLength()-1] == '\\')
			apath = apath.Left(apath.GetLength()-1);
		for (int j=0; j<nPyPaths; ++j)
		{
			CString aPyPath = pyPaths[j];
			aPyPath.MakeLower();
			if (aPyPath[aPyPath.GetLength()-1] == '\\')
				aPyPath = aPyPath.Left(aPyPath.GetLength()-1);
			if (apath == aPyPath)
			{
				bDel = true;
				break;
			}
			aPyPath += TEXT("\\scripts"); 
			if (apath == aPyPath)
			{
				bDel = true;
				break;
			}
		}

		if (!bDel)
		{
			sPathNew += paths[i] + ';';
		}
	}

	if (thisPath[thisPath.GetLength()-1] == '\\')
		thisPath = thisPath.Left(thisPath.GetLength()-1);
	sPathNew = thisPath + ';' + thisPath + TEXT("\\Scripts;") + sPathNew;
	
	CString sSetCmd = TEXT("setx \"Path\" \"") + sPathNew + "\" /m";

#ifdef _UNICODE
	std::string s;
	WideByte2Ansi(s, sSetCmd.GetString());
	const char* p = s.c_str();
#else
	const char* p = sSetCmd.GetString();
#endif

	system(p);

	AfxMessageBox(TEXT("设置成功。"), MB_OK);
}
