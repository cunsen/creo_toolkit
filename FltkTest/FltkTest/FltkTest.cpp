// FltkTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FltkTest.h"

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

// CFltkTestApp

BEGIN_MESSAGE_MAP(CFltkTestApp, CWinApp)
END_MESSAGE_MAP()


// CFltkTestApp ����

CFltkTestApp::CFltkTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFltkTestApp ����

CFltkTestApp theApp;


// CFltkTestApp ��ʼ��

BOOL CFltkTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}




static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
		return ACCESS_AVAILABLE;
}

void ShowDialog()
{
	Fl_Window  *window;
	Fl_Box  *box;
	//���ļ��ı�����GB2312,�����޷���ʾ����
	//Ӧ�ý�Fltk�Ĵ��뵥���������UTF-8���뱣�棬������֧��������
	window = new  Fl_Window(450, 330, "First App!");
	box = new Fl_Box(20, 40, 360, 200, "Hello World");
	box->box(FL_FLAT_BOX);
	box->labelsize(36);                   //���������С
	box->labelfont(FL_BOLD + FL_ITALIC);  //��������
	box->labeltype(FL_SHADOW_LABEL);      //����label������
	window->end();
	window->show();
}



extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId FltkTestID;

	status = ProMenubarMenuAdd("FltkTest", "FltkTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("FltkTest", "FltkTest", "FltkTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("FltkTest_Act", (uiCmdCmdActFn)ShowDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &FltkTestID);
	status = ProMenubarmenuPushbuttonAdd("FltkTest", "FltkTestmenu", "FltkTestmenu", "FltkTestmenutips", NULL, PRO_B_TRUE, FltkTestID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
