// DialogMessageLoop.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IMEHook.h"
#include "DialogMessageLoop.h"
#include "afxdialogex.h"

// CDialogMessageLoop �Ի���

IMPLEMENT_DYNAMIC(CDialogMessageLoop, CDialog)

CDialogMessageLoop::CDialogMessageLoop(CWnd *pParent /*=NULL*/)
	: CDialog(CDialogMessageLoop::IDD, pParent)
{
}

CDialogMessageLoop::~CDialogMessageLoop()
{
}

void CDialogMessageLoop::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogMessageLoop, CDialog)
ON_WM_TIMER()
END_MESSAGE_MAP()

// CDialogMessageLoop ��Ϣ�������

BOOL CDialogMessageLoop::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1, 1, NULL);

	return TRUE; // return TRUE unless you set the focus to a control
				 // �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogMessageLoop::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)

	{
		KillTimer(1);
		OnOK();
	}
	CDialog::OnTimer(nIDEvent);
}
