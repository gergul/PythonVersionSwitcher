
// Python�汾�л���.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPython�汾�л���App:
// �йش����ʵ�֣������ Python�汾�л���.cpp
//

class CPython�汾�л���App : public CWinApp
{
public:
	CPython�汾�л���App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPython�汾�л���App theApp;