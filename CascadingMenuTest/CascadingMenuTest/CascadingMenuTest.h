// CascadingMenuTest.h : CascadingMenuTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCascadingMenuTestApp
// �йش���ʵ�ֵ���Ϣ������� CascadingMenuTest.cpp
//

class CCascadingMenuTestApp : public CWinApp
{
public:
	CCascadingMenuTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
