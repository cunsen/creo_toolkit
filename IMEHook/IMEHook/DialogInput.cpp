// DialogInput.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IMEHook.h"
#include "DialogInput.h"
#include "afxdialogex.h"

// CDialogInput �Ի���

IMPLEMENT_DYNAMIC(CDialogInput, CDialog)

CDialogInput::CDialogInput(CWnd *pParent /*=NULL*/)
	: CDialog(CDialogInput::IDD, pParent)
{
}

CDialogInput::~CDialogInput()
{
}

void CDialogInput::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogInput, CDialog)
END_MESSAGE_MAP()

// CDialogInput ��Ϣ�������
