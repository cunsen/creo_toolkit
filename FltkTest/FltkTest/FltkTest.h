// FltkTest.h : FltkTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFltkTestApp
// �йش���ʵ�ֵ���Ϣ������� FltkTest.cpp
//

class CFltkTestApp : public CWinApp
{
public:
	CFltkTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
