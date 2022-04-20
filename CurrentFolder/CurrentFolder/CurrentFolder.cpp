// CurrentFolder.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CurrentFolder.h"

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

// CCurrentFolderApp

BEGIN_MESSAGE_MAP(CCurrentFolderApp, CWinApp)
END_MESSAGE_MAP()

// CCurrentFolderApp ����

CCurrentFolderApp::CCurrentFolderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CCurrentFolderApp ����

CCurrentFolderApp theApp;

// CCurrentFolderApp ��ʼ��

BOOL CCurrentFolderApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void OpenFolder()
{
	ProError status;
	ProMdl mdl;
	ProPath path;
	ProMdldata mdldata;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDirectoryCurrentGet(path);
		ShellExecute(NULL, NULL, _T("explorer"), CString(path), NULL, SW_SHOW);
		return;
	}
	else
	{
		status = ProMdlDataGet(mdl, &mdldata);
		ShellExecute(NULL, NULL, _T("explorer"), CString(mdldata.device) + _T(":") + CString(mdldata.path), NULL, SW_SHOW);
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId CurrentFolderID;

	status = ProMenubarMenuAdd("CurrentFolder", "CurrentFolder", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CurrentFolder", "CurrentFolder", "CurrentFolder", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("CurrentFolder_Act", (uiCmdCmdActFn)OpenFolder, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &CurrentFolderID);
	status = ProMenubarmenuPushbuttonAdd("CurrentFolder", "CurrentFoldermenu", "CurrentFoldermenu", "CurrentFoldermenutips", NULL, PRO_B_TRUE, CurrentFolderID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}