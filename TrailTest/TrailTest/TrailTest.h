// TrailTest.h : TrailTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTrailTestApp
// �йش���ʵ�ֵ���Ϣ������� TrailTest.cpp
//

class CTrailTestApp : public CWinApp
{
public:
	CTrailTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
