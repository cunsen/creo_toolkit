// IMEHook.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "IMEHook.h"
#include "DialogInput.h"
#include "DialogMessageLoop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region MFC
//
// TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CIMEHookApp

BEGIN_MESSAGE_MAP(CIMEHookApp, CWinApp)
END_MESSAGE_MAP()

// CIMEHookApp ����

CIMEHookApp::CIMEHookApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CIMEHookApp ����

CIMEHookApp theApp;

// CIMEHookApp ��ʼ��

BOOL CIMEHookApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion

HHOOK hHook = NULL;
CDialogInput *dlg = NULL;

LRESULT CALLBACK __stdcall GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LPMSG lpMsg = (LPMSG)lParam;

	if (dlg != NULL)
	{
		if (lpMsg->message == WM_IME_ENDCOMPOSITION)
		{
			CDialogMessageLoop dialog;
			dialog.DoModal();
		}
	}

	return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void ShowWin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxEnableControlContainer();
	if (dlg != NULL)
		delete (dlg);
	dlg = new CDialogInput();
	dlg->Create(IDD_DIALOGINPUT);
	dlg->ShowWindow(SW_SHOW);
}

extern "C" int user_initialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxInitRichEdit2();
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxEnableControlContainer();
	ProError status;
	uiCmdCmdId showModelessDialog;
	status = ProMenubarMenuAdd("unicodechineseinput", "unicodechineseinput", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("unicodechineseinput", "unicodechineseinput", "showdialog", NULL, PRO_B_TRUE, MSGFILE);
	status = ProCmdActionAdd("ShowDIALOG_Act", (uiCmdCmdActFn)ShowWin, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &showModelessDialog);
	status = ProMenubarmenuPushbuttonAdd("unicodechineseinput", "showdialog", "showdialog", "showdialogtips", NULL, PRO_B_TRUE, showModelessDialog, MSGFILE);

	hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, NULL, GetCurrentThreadId());
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (dlg != NULL)
		delete (dlg);
	UnhookWindowsHookEx(hHook);
}
