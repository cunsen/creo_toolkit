// CurrentFolder.h : CurrentFolder DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCurrentFolderApp
// �йش���ʵ�ֵ���Ϣ������� CurrentFolder.cpp
//

class CCurrentFolderApp : public CWinApp
{
public:
	CCurrentFolderApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
