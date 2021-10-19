// NavigatorpaneTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "NavigatorpaneTest.h"

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

// CNavigatorpaneTestApp

BEGIN_MESSAGE_MAP(CNavigatorpaneTestApp, CWinApp)
END_MESSAGE_MAP()


// CNavigatorpaneTestApp ����

CNavigatorpaneTestApp::CNavigatorpaneTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNavigatorpaneTestApp ����

CNavigatorpaneTestApp theApp;


// CNavigatorpaneTestApp ��ʼ��

BOOL CNavigatorpaneTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void AddUrlPane()
{
	ProError status;
	status = ProNavigatorpaneBrowserAdd("Weblink Apps", NULL, L"www.hudi.site");
	status = ProNavigatorpaneBrowserURLSet("Weblink Apps", L"weblink.hudi.site");
	status = ProNavigatorpaneBrowsericonSet("Weblink Apps", "D:\\mydoc\\creo_toolkit\\NavigatorpaneTest\\text\\favicon.PNG");
}

void AddDialog()
{
	ProError status;
	status = ProNavigatorpanePHolderAdd("MyApps1", "NavigatorpaneTest.res", "D:\\mydoc\\creo_toolkit\\NavigatorpaneTest\\text\\favicon.PNG");

}
extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId AddUrlID,AddDialogID;

	status = ProMenubarMenuAdd("NavigatorpaneTest", "NavigatorpaneTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("NavigatorpaneTest", "NavigatorpaneTest", "NavigatorpaneTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("AddUrl_Act", (uiCmdCmdActFn)AddUrlPane, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AddUrlID);
	status = ProMenubarmenuPushbuttonAdd("NavigatorpaneTest", "AddUrl", "AddUrl", "AddUrlTips", NULL, PRO_B_TRUE, AddUrlID, MSGFILE);

	status = ProCmdActionAdd("AddDialog_Act", (uiCmdCmdActFn)AddDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AddDialogID);
	status = ProMenubarmenuPushbuttonAdd("NavigatorpaneTest", "AddDialog", "AddDialog", "AddDialogTips", NULL, PRO_B_TRUE, AddDialogID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
