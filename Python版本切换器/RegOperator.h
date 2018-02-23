/********************************************************************
*�� �ļ�����RegOperator.h
*�� �ļ���������ע���ĳ����������з�װ
*�� �����ˣ� RainLeaf, 2009��4��10��
*�� �汾�ţ�1.0
*�� �޸ļ�¼��
*********************************************************************/
#include <afx.h>
#include <assert.h>
#include <vector>

#ifndef REGOP_H
#define REGOP_H

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

class CRegOperator
{
private:
	HKEY m_hKey;													/* ���浱ǰ���������ľ��						*/

public:
	CRegOperator( HKEY hKey = HKEY_LOCAL_MACHINE );					/* ���캯����Ĭ�ϲ���Ϊ��HKEY_LOCAL_MACHINE		*/
	BOOL SetHKEY( LPCTSTR strKey );									/* ���õ�ǰ�����ĸ���							*/
	BOOL OpenKey( LPCTSTR lpSubKey, bool bX64=false );								/* ��д����ʽ��ע���							*/
	void Close();													/* �رռ����									*/

	BOOL CreateKey( LPCTSTR lpSubKey );								/* ��������ʽ��ע���							*/
	BOOL DeleteKey( LPCTSTR lphKey, LPCTSTR lpSubKey );				/* ɾ����Ӧ���Ӽ����Ӽ�Ϊ�գ�					*/
	BOOL DeleteValue( LPCTSTR lpValueName );						/* ɾ���Ӽ�������Ӧ�ļ�ֵ						*/	
	BOOL SaveKey( LPCTSTR lpFileName );								/* �ѵ�ǰ��ֵ���浽ָ���ļ�						*/
	BOOL RestoreKey( LPCTSTR lpFileName );							/* ��ָ��ע����ļ��лָ�						*/

	BOOL Read( LPCTSTR lpValueName, CString* lpVal );				/* ����REG_SZ���͵�ֵ							*/
	BOOL Read( LPCTSTR lpValueName, DWORD* pdwVal );				/* ����DWORD���͵�ֵ							*/
	BOOL Read( LPCTSTR lpValueName, int* pnVal );					/* ����INT���͵�ֵ								*/
	BOOL Write( LPCTSTR lpSubKey, LPCTSTR lpVal );					/* д��REG_SZ����ֵ								*/
	BOOL Write( LPCTSTR lpSubKey, DWORD dwVal );					/* д��DWORD����ֵ								*/
	BOOL Write( LPCTSTR lpSubKey, int nVal );						/* д��INT����ֵ								*/

	BOOL EnumSub( 
		OUT std::vector<CString>* subTreeNames=NULL, 
		OUT std::vector<CString>* subValueNames=NULL);				/* ö��											*/

	virtual ~CRegOperator();
};

#endif