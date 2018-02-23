
// Python版本切换器Dlg.h : 头文件
//

#pragma once
#include "customlistctrl\listctrlcustom.h"
#include <vector>
#include "APythonInfoItem.h"


// CPython版本切换器Dlg 对话框
class CPythonSwitchDlg : public CDialogEx
{
// 构造
public:
	CPythonSwitchDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PYTHON_DIALOG };

	std::vector<CString> GetPyPaths() { return m_paths; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void searchInstalledPythonFromReg();

private:
	std::vector<CString> m_paths;
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListCtrlCustom m_ctrlPythonList;
	afx_msg void OnBnClickedBtnAddGreen();
};
