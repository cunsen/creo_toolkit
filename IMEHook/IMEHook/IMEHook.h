// IMEHook.h : IMEHook DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h" // ������

// CIMEHookApp
// �йش���ʵ�ֵ���Ϣ������� IMEHook.cpp
//

class CIMEHookApp : public CWinApp
{
public:
	CIMEHookApp();

	// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
