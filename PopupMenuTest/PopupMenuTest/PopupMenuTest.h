// PopupMenuTest.h : PopupMenuTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPopupMenuTestApp
// �йش���ʵ�ֵ���Ϣ������� PopupMenuTest.cpp
//

class CPopupMenuTestApp : public CWinApp
{
public:
	CPopupMenuTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
