// MessageTest.h : MessageTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMessageTestApp
// �йش���ʵ�ֵ���Ϣ������� MessageTest.cpp
//

class CMessageTestApp : public CWinApp
{
public:
	CMessageTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
