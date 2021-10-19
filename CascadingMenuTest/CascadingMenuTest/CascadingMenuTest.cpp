// CascadingMenuTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CascadingMenuTest.h"

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

// CCascadingMenuTestApp

BEGIN_MESSAGE_MAP(CCascadingMenuTestApp, CWinApp)
END_MESSAGE_MAP()


// CCascadingMenuTestApp ����

CCascadingMenuTestApp::CCascadingMenuTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCascadingMenuTestApp ����

CCascadingMenuTestApp theApp;


// CCascadingMenuTestApp ��ʼ��

BOOL CCascadingMenuTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


int _dialogStyle = 1;
wchar_t *_dialogcontent = L"��������1";

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

ProError _showDialog(int style, wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProError status;
	if (style == 1)
	{
		status = ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
		buttons[0] = PRO_UI_MESSAGE_YES;
	}
	else
	{
		ProArrayAlloc(2, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
		buttons[0] = PRO_UI_MESSAGE_YES;
		buttons[1] = PRO_UI_MESSAGE_NO;
	}

	status = ProUIMessageDialogDisplay(PROUIMESSAGE_QUESTION, L"��ʾ", Message, buttons, buttons[0], &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

int SetDialogContent(ProAppData app_data, int dialogStyle)
{
	_dialogcontent = (wchar_t *)app_data;
	return 0;
}

int ShowContent(ProAppData app_data, int app_int)
{
	ProError status;
	int ContentMenuID;
	int action;
	wchar_t *Content1 = L"��������1";
	wchar_t *Content2 = L"��������2";
	status = ProMenuFileRegister("Content", "Content.mnu", &ContentMenuID);
	status = ProMenubuttonActionSet("Content", "Custom String 1", (ProMenubuttonAction)SetDialogContent, ProAppData(Content1), 0);
	status = ProMenubuttonActionSet("Content", "Custom String 2", (ProMenubuttonAction)SetDialogContent, ProAppData(Content2), 0);
	status = ProMenubuttonActionSet("Content", "Done/Return", (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	status = ProMenubuttonActionSet("Content", "Content", (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	//status = ProMenuPush();
	status = ProMenuCreate(PROMENUTYPE_MAIN, "Content", &ContentMenuID);
	status = ProMenubuttonHighlight("Content", "Custom String 1");
	status = ProMenuProcess("", &action);
	return 0;
}

int SetDialogStyle(ProAppData app_data, int dialogStyle)
{
	_dialogStyle = dialogStyle;
	return 0;
}

int DeleteMenuadnPop(ProAppData app_data, int dialogStyle)
{
	ProError status;
	status = ProMenuDelete();
	status = ProMenuPop();
	return 0;
}

int ShowDialogStyle(ProAppData app_data, int app_int)
{
	ProError status;
	int DialogStyleMenuID;
	int action;
	status = ProMenuFileRegister("Dialog Style", "DialogStyle.mnu", &DialogStyleMenuID);
	status = ProMenubuttonActionSet("Dialog Style", "Single YES Button", (ProMenubuttonAction)SetDialogStyle, NULL, SINGLEYESDIALOG);
	status = ProMenubuttonActionSet("Dialog Style", "YES/NO Button", (ProMenubuttonAction)SetDialogStyle, NULL, YESNODIALOG);
	status = ProMenubuttonActionSet("Dialog Style", "Done/Return", (ProMenubuttonAction)DeleteMenuadnPop, NULL, 0);
	status = ProMenubuttonActionSet("Dialog Style", "Dialog Style", (ProMenubuttonAction)DeleteMenuadnPop, NULL, 0);
	status = ProMenuPush();
	status = ProMenuCreate(PROMENUTYPE_MAIN, "Dialog Style", &DialogStyleMenuID);
	status = ProMenubuttonHighlight("Dialog Style", "Single YES Button");
	status = ProMenuProcess("", &action);
	return 0;
}

int QuitAction(ProAppData app_data, int app_int)
{
	ProError status;
	status = ProMenuDeleteWithStatus(app_int);
	return 0;
}

void ShowCascadingMenu()
{
	ProError status;
	int TestMenuId;
	int action;
	status = ProMenuFileRegister("Show Custom Dialog", "ShowCustomDialog.mnu", &TestMenuId);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Show Custom Dialog", (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Dialog Style", (ProMenubuttonAction)ShowDialogStyle, NULL, 0);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Content", (ProMenubuttonAction)ShowContent, NULL, 1);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Show Dialog", (ProMenubuttonAction)QuitAction, NULL, POPUPMENU_DONE);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Done/Return", (ProMenubuttonAction)QuitAction, NULL, POPUPMENU_QUIT);
	status = ProMenuCreate(PROMENUTYPE_MAIN, "Show Custom Dialog", &TestMenuId);
	status = ProMenuProcess("", &action);
	if (action == POPUPMENU_DONE)
	{
		_showDialog(_dialogStyle, _dialogcontent);
	}
	else
	{
		_showDialog(SINGLEYESDIALOG, L"ȡ���˹���");
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId showPopupMenuID;

	status = ProMenubarMenuAdd("CascadingMenuTest", "CascadingMenuTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CascadingMenuTest", "CascadingMenuTest", "CascadingMenuTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ShowCascadingMenu_Act", (uiCmdCmdActFn)ShowCascadingMenu, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &showPopupMenuID);
	status = ProMenubarmenuPushbuttonAdd("CascadingMenuTest", "ShowCascadingMenu", "ShowCascadingMenu", "ShowCascadingMenuTips", NULL, PRO_B_TRUE, showPopupMenuID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
