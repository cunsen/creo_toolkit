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

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId MessageClearID;

	status = ProMenubarMenuAdd("CoordTrf", "CoordTrf", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CoordTrf", "CoordTrf", "CoordTrf", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ComponentToAsm_Act", (uiCmdCmdActFn)CoordComponentToAsm, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &MessageClearID);
	status = ProMenubarmenuPushbuttonAdd("CoordTrf", "ComponentToAsm", "ComponentToAsm", "ComponentToAsmtips", NULL, PRO_B_TRUE, MessageClearID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}

