#pragma once
#include "afxwin.h"
#include "customlistctrl\listctrlcustom.h"

typedef struct tag_pyinfo
{
	CString sVer;
	CString sPath;
} PyINFO;


// APythonInfo 对话框

class APythonInfoItem : public CDialogEx
{
	DECLARE_DYNAMIC(APythonInfoItem)

public:
	APythonInfoItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~APythonInfoItem();

// 对话框数据
	enum { IDD = IDD_DLG_AINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void SetPyInfo(const PyINFO& info);
	void SetList(CListCtrlCustom* pList) { m_pList = pList; }
	
private:
	CEdit m_editVersion;
	CEdit m_editPath;
	CListCtrlCustom* m_pList;

public:
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedRadioSelected();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnSetev();
};
