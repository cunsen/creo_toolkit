#pragma once

// CDialogInput �Ի���

class CDialogInput : public CDialog
{
	DECLARE_DYNAMIC(CDialogInput)

public:
	CDialogInput(CWnd *pParent = NULL); // ��׼���캯��
	virtual ~CDialogInput();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOGINPUT
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
