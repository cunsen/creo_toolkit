// CoordTrf.h : CoordTrf DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCoordTrfApp
// �йش���ʵ�ֵ���Ϣ������� CoordTrf.cpp
//

class CCoordTrfApp : public CWinApp
{
public:
	CCoordTrfApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
