// NavigatorpaneTest.h : NavigatorpaneTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNavigatorpaneTestApp
// �йش���ʵ�ֵ���Ϣ������� NavigatorpaneTest.cpp
//

class CNavigatorpaneTestApp : public CWinApp
{
public:
	CNavigatorpaneTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
