#pragma once

// CDialogMessageLoop �Ի���

class CDialogMessageLoop : public CDialog
{
	DECLARE_DYNAMIC(CDialogMessageLoop)

public:
	CDialogMessageLoop(CWnd *pParent = NULL); // ��׼���캯��
	virtual ~CDialogMessageLoop();

	// �Ի�������
	enum
	{
		IDD = IDD_DIALOGFORMESSAGELOOP
	};

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
