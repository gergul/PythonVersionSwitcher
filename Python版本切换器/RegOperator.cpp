/********************************************************************
*�� �ļ�����RegOperator.cpp
*�� �ļ���������ͷ�ļ��ж�����ĳ�Ա����������ʵ��
*�� �����ˣ� RainLeaf, 2009��4��10��
*�� �汾�ţ�1.0
*�� �޸ļ�¼��
*********************************************************************/
#include "stdafx.h"
#include "RegOperator.h"

/*============================================================
* �� �� ����CRegOperator
* �Ρ�������HKEY [IN] : Ĭ����HKEY_LOCAL_MACHINE
* �������������캯������ʼ������
* �� �� ֵ����
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

CRegOperator::CRegOperator( HKEY hKey )
{
	m_hKey = HKEY_LOCAL_MACHINE;
}

/*============================================================
* �� �� ����~CRegOperator
* �Ρ�������NULL [IN]
* ���������������������رմ򿪵�ע�����
* �� �� ֵ����
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

CRegOperator::~CRegOperator()
{
	Close();
}

/*============================================================
* �� �� ����Close
* �Ρ�������NULL [IN]
* �����������رմ򿪼��ľ��
* �� �� ֵ��void
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

void CRegOperator::Close()
{
	if( m_hKey )
	{
		/* ����ǿս��йر� */
		RegCloseKey( m_hKey );
		m_hKey = NULL;
	}	
}

/*============================================================
* �� �� ����SetHKEY
* �Ρ�������LPCTSTR [IN] : ����ֵ
* �����������ɴ���ĸ����ַ������õ�ǰ�����ĸ�����ֵ
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::SetHKEY( LPCTSTR strKey )
{
	assert( m_hKey );
	assert( strKey );

	/* ������бȽ� */
	if( 0 == _tcscmp(strKey, TEXT("HKEY_CLASSES_ROOT")) )
	{
		m_hKey = HKEY_CLASSES_ROOT;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_CURRENT_USER")) )
	{
		m_hKey = HKEY_CURRENT_USER;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_LOCAL_MACHINE")) )
	{
		m_hKey = HKEY_LOCAL_MACHINE;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_USERS")) )
	{
		m_hKey = HKEY_USERS;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_PERFORMANCE_DATA")) )
	{
		m_hKey = HKEY_PERFORMANCE_DATA;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_PERFORMANCE_TEXT")) )
	{
		m_hKey = HKEY_PERFORMANCE_TEXT;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_PERFORMANCE_NLSTEXT")) )
	{
		m_hKey = HKEY_PERFORMANCE_NLSTEXT;
		return TRUE;
	}
	/* �Բ���ϵͳ�汾���в��� */
#if(WINVER >= 0x0400)
	if( 0 == _tcscmp(strKey, TEXT("HKEY_CURRENT_CONFIG")) )
	{
		m_hKey = HKEY_CURRENT_CONFIG;
		return TRUE;
	}
	if( 0 == _tcscmp(strKey, TEXT("HKEY_DYN_DATA")) )
	{
		m_hKey = HKEY_DYN_DATA;
		return TRUE;
	}
#endif

	return FALSE;
}

/*============================================================
* �� �� ����OpenKey
* �Ρ�������LPCTSTR [IN] : �Ӽ��ַ���
* ����������ͨ�������Ӽ����ַ�����ע�����Ӧ��λ��
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::OpenKey( LPCTSTR lpSubKey, bool bX64/*=false*/  )
{
	assert( m_hKey );
	assert( lpSubKey );

	DWORD opt = KEY_ALL_ACCESS;
	if (bX64)
		opt |= KEY_WOW64_64KEY;

	HKEY hKey;
	long lReturn = RegOpenKeyEx( m_hKey, lpSubKey, 0L, opt, &hKey );

	if( ERROR_SUCCESS == lReturn )
	{
		/* �ɹ����򽫴򿪵ľ������ */
		m_hKey = hKey;

		return TRUE;
	}
	/* ��ʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����CreateKey
* �Ρ�������LPCTSTR [IN] : �Ӽ��ַ���
* ����������ͨ�������Ӽ����ַ����򿪣������Ӽ������ߴ������������Ӽ�����Ӧ���Ӽ�
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::CreateKey( LPCTSTR lpSubKey )
{
	assert( m_hKey );
	assert( lpSubKey );

	HKEY hKey;
	DWORD dw;
	long lReturn = RegCreateKeyEx( m_hKey, lpSubKey, 0L, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw);

	if( ERROR_SUCCESS == lReturn )
	{
		/* �ɹ��򿪻��ߴ����򽫾������ */
		m_hKey = hKey;

		return TRUE;
	}
	/* �򿪻��ߴ���ʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����DeleteKey
* �Ρ�������LPCTSTR LPCTSTR [IN] : ����ֵ �Ӽ�ֵ
* ����������ͨ������ĸ������Ӽ������Ӽ�ɾ��ɾ�������ܰ����Ӽ�
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

//BOOL CRegOperator::DeleteKey( HKEY hKey, LPCTSTR lpSubKey )
BOOL CRegOperator::DeleteKey( LPCTSTR lphKey, LPCTSTR lpSubKey )
{
	assert( lphKey );
	assert( lpSubKey );
	assert( m_hKey );

	SetHKEY( lphKey );
	long lReturn = RegDeleteValue( m_hKey, lpSubKey );

	if( ERROR_SUCCESS == lReturn )
	{
		/* ɾ���ɹ� */
		return TRUE;
	}
	/* ɾ��ʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����DeleteValue
* �Ρ�������LPCTSTR [IN] : Ҫɾ����ֵ������
* ����������ͨ�������ֵ���ƣ�ɾ����Ӧ�ļ�ֵ
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::DeleteValue( LPCTSTR lpValueName )
{
	assert( m_hKey );
	assert( lpValueName );

	long lReturn = RegDeleteValue( m_hKey, lpValueName );

	if( ERROR_SUCCESS == lReturn )
	{
		/* ɾ���ɹ� */
		return TRUE;
	}
	/* ɾ��ʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����SaveKey
* �Ρ�������LPCTSTR [IN] : ��������ļ���
* ����������ͨ��������ļ����ƣ������Ӧ���Ӽ�
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::SaveKey( LPCTSTR lpFileName )
{
	assert( m_hKey );
	assert( lpFileName );

	long lReturn = RegSaveKey( m_hKey, lpFileName, NULL );

	if( ERROR_SUCCESS == lReturn )
	{
		/* ����ɹ� */
		return TRUE;
	}
	/* ����ʧ�� */	
	return FALSE;
}

/*============================================================
* �� �� ����RestoreKey
* �Ρ�������LPCTSTR [IN] : ���ָ����ļ���
* ����������ͨ���ļ����ƣ������е���ע�����
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::RestoreKey( LPCTSTR lpFileName )
{
	assert( m_hKey );
	assert( lpFileName );

	long lReturn = RegRestoreKey( m_hKey, lpFileName, 0);

	if( ERROR_SUCCESS == lReturn )
	{
		/* ����ɹ� */
		return TRUE;
	}
	/* ����ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����Read
* �Ρ�������LPCTSTR CString [IN] : ��ֵ ��ȡֵ��key����Ϊ���ַ���ʱ��ȡĬ��ֵ
* ������������ָ��λ�õ�CString���͵�ֵ����
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::Read( LPCTSTR lpValueName, CString* lpVal )
{
	assert( m_hKey );
	assert( lpValueName );
	assert( lpVal );

	DWORD dwType = 0;
	DWORD dwSize = 2047;
	TCHAR szString[2048];
	memset( szString, 0, 2048 * sizeof(TCHAR) );

	long lReturn = RegQueryValueEx( m_hKey, lpValueName, NULL, &dwType, (BYTE *)szString, &dwSize );

	if( ERROR_SUCCESS == lReturn )
	{
		/* ��ѯ�ɹ� */
		*lpVal = szString;

		return TRUE;
	}
	/* ��ѯʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����Read
* �Ρ�������LPCTSTR DWORD [IN] : ��ֵ ��ȡֵ��key����Ϊ���ַ���ʱ��ȡĬ��ֵ
* ������������ָ��λ��DWORD���͵�ֵ����
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::Read( LPCTSTR lpValueName, DWORD* pdwVal )
{
	assert( m_hKey );
	assert( lpValueName );
	assert( pdwVal );

	DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDest;
	long lReturn = RegQueryValueEx( m_hKey, lpValueName, NULL, &dwType, (BYTE *)&dwDest, &dwSize );

	if( ERROR_SUCCESS == lReturn )
	{
		/* ��ѯ�ɹ� */
		*pdwVal = dwDest;

		return TRUE;
	}
	/* ��ѯʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����Read
* �Ρ�������LPCTSTR int [IN] : ��ֵ ��ȡֵ��key����Ϊ���ַ���ʱ��ȡĬ��ֵ
* ������������ָ��λ��int���͵�ֵ����
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::Read( LPCTSTR lpValueName, int* pnVal)
{
	assert( m_hKey );
	assert( lpValueName );
	assert( pnVal );

	DWORD dwType;
	DWORD dwSize=sizeof(DWORD);
	DWORD dwDest;
	long lReturn = RegQueryValueEx( m_hKey, lpValueName, NULL, &dwType, (BYTE *)&dwDest, &dwSize );

	if( ERROR_SUCCESS == lReturn )
	{
		/* ��ѯ�ɹ� */
		*pnVal=(int)dwDest;

		return TRUE;
	}

	return FALSE;	
}

/*============================================================
* �� �� ����Write
* �Ρ�������LPCTSTR LPCTSTR [IN] : ��ֵ ��ȡֵ��key����Ϊ���ַ���ʱ��ȡĬ��ֵ
* ������������LPCTSTR���͵�ֵд��ָ��λ��
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::Write( LPCTSTR lpValueName, LPCTSTR lpValue )
{
	assert( m_hKey );
	assert( lpValueName );
	assert( lpValue );	

	long lReturn = RegSetValueEx( m_hKey, lpValueName, 0L, REG_SZ, (const BYTE *) lpValue, (_tcslen(lpValue)*sizeof(lpValue[0])) +1 );

	if( ERROR_SUCCESS == lReturn )
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����Write
* �Ρ�������LPCTSTR DWORD [IN] : ��ֵ д��ֵ��key����Ϊ���ַ���ʱ��ȡĬ��ֵ
* ������������DWORD���͵�ֵд��ָ��λ��
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::Write( LPCTSTR lpSubKey, DWORD dwVal )
{
	assert( m_hKey );
	assert( lpSubKey );

	long lReturn = RegSetValueEx( m_hKey, lpSubKey, 0L, REG_DWORD, (const BYTE *) &dwVal, sizeof(DWORD) );

	if( ERROR_SUCCESS == lReturn )
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;	
}

/*============================================================
* �� �� ����Write
* �Ρ�������LPCTSTR int [IN] : ��ֵ д��ֵ��key����Ϊ���ַ���ʱ��ȡĬ��ֵ
* ������������������д��ָ��λ��
* �� �� ֵ��BOOL
* �׳��쳣��
* �������ߣ�RainLeaf, 2009��4��10��
*============================================================*/

BOOL CRegOperator::Write( LPCTSTR lpSubKey, int nVal )
{
	assert( m_hKey );
	assert( lpSubKey );

	DWORD dwValue;
	dwValue=(DWORD)nVal;

	long lReturn = RegSetValueEx( m_hKey, lpSubKey, 0L, REG_DWORD, (const BYTE *) &dwValue, sizeof(DWORD) );

	if( ERROR_SUCCESS == lReturn )
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;	
}

BOOL CRegOperator::EnumSub(OUT std::vector<CString>* subTreeNames/*=NULL*/, OUT std::vector<CString>* subValueNames/*=NULL*/)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name  
	TCHAR  achValue[MAX_VALUE_NAME];   
	DWORD cchValue = MAX_VALUE_NAME;
	DWORD    cbName;                   // size of name string   
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name   
	DWORD    cchClassName = MAX_PATH;  // size of class string   
	DWORD    cSubKeys=0;               // number of subkeys
	DWORD    cValues;              // number of values for key   
	DWORD    cbMaxSubKey;              // longest subkey size   
	DWORD    cchMaxClass;              // longest class string
	DWORD    cchMaxValue;          // longest value name   
	DWORD    cbMaxValueData;       // longest value data   
	DWORD    cbSecurityDescriptor; // size of security descriptor   
	FILETIME ftLastWriteTime;      // last write time   

	DWORD i, retCode;   

	 

	// Get the class name and the value count.   
	retCode = RegQueryInfoKey(  
		m_hKey,                    // key handle   
		achClass,                // buffer for class name   
		&cchClassName,           // size of class string   
		NULL,                    // reserved   
		&cSubKeys,               // number of subkeys   
		&cbMaxSubKey,            // longest subkey size   
		&cchMaxClass,            // longest class string   
		&cValues,                // number of values for this key   
		&cchMaxValue,            // longest value name   
		&cbMaxValueData,         // longest value data   
		&cbSecurityDescriptor,   // security descriptor   
		&ftLastWriteTime);       // last write time   

	if (retCode != ERROR_SUCCESS)
		return FALSE;

	// Enumerate the subkeys, until RegEnumKeyEx fails.  
	if (cSubKeys && subTreeNames)  
	{  
		//printf( "\nNumber of subkeys: %d\n", cSubKeys);  

		for (i=0; i<cSubKeys; i++)   
		{   
			cbName = MAX_KEY_LENGTH;
			achKey[0] = '\0';
			retCode = RegEnumKeyEx(m_hKey, i,  
				achKey,   
				&cbName,   
				NULL,   
				NULL,   
				NULL,   
				&ftLastWriteTime);   
			if (retCode == ERROR_SUCCESS)   
			{  
				//_tprintf(TEXT("(%d) %s\n"), i+1, achKey);  
				subTreeNames->push_back(achKey);
			}  
		}  
	}   

	// Enumerate the key values.   
	if (cValues && subValueNames)
	{  
		printf( "\nNumber of values: %d\n", cValues);

		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++)
		{
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';   
			retCode = RegEnumValue(m_hKey, i,   
				achValue,   
				&cchValue,   
				NULL,   
				NULL,  
				NULL,  
				NULL);  

			if (retCode == ERROR_SUCCESS )   
			{   
				//_tprintf(TEXT("(%d) %s\n"), i+1, achValue);   
				subValueNames->push_back(achValue);
			}   
		}  
	}

	return TRUE;
}
