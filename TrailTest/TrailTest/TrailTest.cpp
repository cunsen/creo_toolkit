// TrailTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "TrailTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CTrailTestApp

BEGIN_MESSAGE_MAP(CTrailTestApp, CWinApp)
END_MESSAGE_MAP()

// CTrailTestApp ����

CTrailTestApp::CTrailTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTrailTestApp ����

CTrailTestApp theApp;

// CTrailTestApp ��ʼ��

BOOL CTrailTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static int _count = 1;

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void AddTrail()
{
	ProError status;
	CString c;
	c.Format("����˲��Բ˵�%d��\n", _count);
	wchar_t *str = c.AllocSysString();
	status = ProTrailfileCommentWrite(str);
	_count++;
	SysFreeString(str);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId AddTrailID;

	status = ProMenubarMenuAdd("TrailTest", "TrailTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("TrailTest", "TrailTest", "TrailTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("AddTrailMenu_Act", (uiCmdCmdActFn)AddTrail, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AddTrailID);
	status = ProMenubarmenuPushbuttonAdd("TrailTest", "AddTrail", "AddTrail", "AddTrailTips", NULL, PRO_B_TRUE, AddTrailID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}