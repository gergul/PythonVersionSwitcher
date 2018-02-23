
// Python�汾�л���Dlg.h : ͷ�ļ�
//

#pragma once
#include "customlistctrl\listctrlcustom.h"
#include <vector>
#include "APythonInfoItem.h"


// CPython�汾�л���Dlg �Ի���
class CPythonSwitchDlg : public CDialogEx
{
// ����
public:
	CPythonSwitchDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PYTHON_DIALOG };

	std::vector<CString> GetPyPaths() { return m_paths; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void searchInstalledPythonFromReg();

private:
	std::vector<CString> m_paths;
	
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListCtrlCustom m_ctrlPythonList;
	afx_msg void OnBnClickedBtnAddGreen();
};
