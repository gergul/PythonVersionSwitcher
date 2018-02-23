/********************************************************************
*　 文件名：RegOperator.h
*　 文件描述：对注册表的常见操作进行封装
*　 创建人： RainLeaf, 2009年4月10日
*　 版本号：1.0
*　 修改记录：
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
	HKEY m_hKey;													/* 保存当前操作根键的句柄						*/

public:
	CRegOperator( HKEY hKey = HKEY_LOCAL_MACHINE );					/* 构造函数，默认参数为：HKEY_LOCAL_MACHINE		*/
	BOOL SetHKEY( LPCTSTR strKey );									/* 设置当前操作的根键							*/
	BOOL OpenKey( LPCTSTR lpSubKey, bool bX64=false );								/* 读写的形式打开注册表							*/
	void Close();													/* 关闭键句柄									*/

	BOOL CreateKey( LPCTSTR lpSubKey );								/* 创建的形式打开注册表							*/
	BOOL DeleteKey( LPCTSTR lphKey, LPCTSTR lpSubKey );				/* 删除相应的子键（子键为空）					*/
	BOOL DeleteValue( LPCTSTR lpValueName );						/* 删除子键处的相应的键值						*/	
	BOOL SaveKey( LPCTSTR lpFileName );								/* 把当前键值保存到指定文件						*/
	BOOL RestoreKey( LPCTSTR lpFileName );							/* 从指定注册表文件中恢复						*/

	BOOL Read( LPCTSTR lpValueName, CString* lpVal );				/* 读出REG_SZ类型的值							*/
	BOOL Read( LPCTSTR lpValueName, DWORD* pdwVal );				/* 读出DWORD类型的值							*/
	BOOL Read( LPCTSTR lpValueName, int* pnVal );					/* 读出INT类型的值								*/
	BOOL Write( LPCTSTR lpSubKey, LPCTSTR lpVal );					/* 写入REG_SZ类型值								*/
	BOOL Write( LPCTSTR lpSubKey, DWORD dwVal );					/* 写入DWORD类型值								*/
	BOOL Write( LPCTSTR lpSubKey, int nVal );						/* 写入INT类型值								*/

	BOOL EnumSub( 
		OUT std::vector<CString>* subTreeNames=NULL, 
		OUT std::vector<CString>* subValueNames=NULL);				/* 枚举											*/

	virtual ~CRegOperator();
};

#endif