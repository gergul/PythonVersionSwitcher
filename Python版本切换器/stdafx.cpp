
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Python�汾�л���.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


//unicode תΪ ansi
bool WideByte2Ansi(std::string &out, const wchar_t* wstrcode)
{
	int ansiSize = WideCharToMultiByte(CP_OEMCP, 0, wstrcode, -1, NULL, 0, NULL, NULL);
	if (ansiSize == ERROR_NO_UNICODE_TRANSLATION)
	{
		//throw std::exception("Invalid UTF-8 sequence.");
		return false;
	}
	if (ansiSize == 0)
	{
		//throw std::exception("Error in conversion.");
		return false;
	}

	out.resize(ansiSize);
	int convresult = WideCharToMultiByte(CP_OEMCP, 0, wstrcode, -1, const_cast<char *>(out.c_str()), ansiSize, NULL, NULL);
	if (convresult != ansiSize)
	{
		//throw std::exception("La falla!");
		return false;
	}

	return true;
}

int SplitString(const CString str, const CString& split, CStringArray &strArray)  
{  
	strArray.RemoveAll();  
	CString strTemp = str;  
	int iIndex = 0;  
	while (1)  
	{  
		iIndex = strTemp.Find(split);  
		if(iIndex >= 0)  
		{  
			strArray.Add(strTemp.Left(iIndex));  
			strTemp = strTemp.Right(strTemp.GetLength()-iIndex-1);  
		}  
		else  
		{  
			break;  
		}  
	}  
	strArray.Add(strTemp);  

	return strArray.GetSize();  
}  