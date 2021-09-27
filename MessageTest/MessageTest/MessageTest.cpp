// MessageTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MessageTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
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

// CMessageTestApp

BEGIN_MESSAGE_MAP(CMessageTestApp, CWinApp)
END_MESSAGE_MAP()

// CMessageTestApp ����

CMessageTestApp::CMessageTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMessageTestApp ����

CMessageTestApp theApp;

// CMessageTestApp ��ʼ��

BOOL CMessageTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void ShowMessageText()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	AfxMessageBox(_T("��ʾһ�����͵���Ϣ"));
	status = ProMessageDisplay(MSGFILE, "IMI_TestMessage");

	AfxMessageBox(_T("��ʾ��ʾ���͵���Ϣ"));
	status = ProMessageDisplay(MSGFILE, "IMI_Prompt");

	AfxMessageBox(_T("��ʾ��Ϣ���͵���Ϣ"));
	status = ProMessageDisplay(MSGFILE, "IMI_Info");

	AfxMessageBox(_T("��ʾ�������͵���Ϣ"));
	status = ProMessageDisplay(MSGFILE, "IMI_Warning");

	AfxMessageBox(_T("��ʾ�������͵���Ϣ"));
	status = ProMessageDisplay(MSGFILE, "IMI_Error");

	AfxMessageBox(_T("��ʾ���ش������͵���Ϣ"));
	status = ProMessageDisplay(MSGFILE, "IMI_Critical");
}

void InputMessage()
{
	ProError status;
	wchar_t wcharString[MAXMESSAGESTRINGLENGTH] = L"�����ÿ��ַ���";
	int intRange[2], intValue = 14;
	intRange[0] = 1;
	intRange[1] = 100;
	double doubleValue = 3.1415926;

	status = ProMessageDisplay(MSGFILE, "IMI_WCharStringPrompt|||%0w", wcharString);
	status = ProMessageStringRead(MAXMESSAGESTRINGLENGTH, wcharString);
	if (status != PRO_TK_NO_ERROR)
	{
		if (status != PRO_TK_MSG_USER_QUIT)
		{
			status = ProWstringCopy(L"�����ÿ��ַ���", wcharString, PRO_VALUE_UNUSED);
		}
		else
			return;
	}

	status = ProMessageDisplay(MSGFILE, "IMI_IntegerPrompt|||%0d", &intValue);
	status = ProMessageIntegerRead(intRange, &intValue);
	if (status != PRO_TK_NO_ERROR && status != PRO_TK_GENERAL_ERROR)
	{
		return;
	}

	status = ProMessageDisplay(MSGFILE, "IMI_DoublePrompt|||%0f", &doubleValue);
	status = ProMessageDoubleRead(NULL, &doubleValue);
	if (status != PRO_TK_NO_ERROR && status != PRO_TK_GENERAL_ERROR)
	{
		return;
	}

	status = ProMessageDisplay(MSGFILE, "IMI_ResultPrompt", wcharString, &intValue, &doubleValue);
}

void ClearMessageText()
{
	ProMessageClear();
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId ShowMessageTextID, ClearMessageID, InputMessageID;

	status = ProMenubarMenuAdd("IMI_MessageTest", "IMI_MessageTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("IMI_MessageTest", "IMI_MessageTest", "IMI_MessageTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ShowMessage_Act", (uiCmdCmdActFn)ShowMessageText, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ShowMessageTextID);
	status = ProMenubarmenuPushbuttonAdd("IMI_MessageTest", "IMI_ShowMessage", "IMI_ShowMessage", "IMI_ShowMessageTips", NULL, PRO_B_TRUE, ShowMessageTextID, MSGFILE);

	status = ProCmdActionAdd("InputMessage_Act", (uiCmdCmdActFn)InputMessage, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &InputMessageID);
	status = ProMenubarmenuPushbuttonAdd("IMI_MessageTest", "IMI_InputMessage", "IMI_InputMessage", "IMI_InputMessageTips", NULL, PRO_B_TRUE, InputMessageID, MSGFILE);

	status = ProCmdActionAdd("ClearMessage_Act", (uiCmdCmdActFn)ClearMessageText, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ClearMessageID);
	status = ProMenubarmenuPushbuttonAdd("IMI_MessageTest", "IMI_ClearMessage", "IMI_ClearMessage", "IMI_ClearMessageTips", NULL, PRO_B_TRUE, ClearMessageID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
