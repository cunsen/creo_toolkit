// zdpt.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "zdpt.h"

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

// CzdptApp

BEGIN_MESSAGE_MAP(CzdptApp, CWinApp)
END_MESSAGE_MAP()

// CzdptApp ����

CzdptApp::CzdptApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CzdptApp ����

CzdptApp theApp;

// CzdptApp ��ʼ��

BOOL CzdptApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

typedef enum _hint
{
	Fun = 0,
	About = 1,
} HINT;
HINT hint;

ProError ShowDialog(wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"��ʾ", Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void zdpt()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProPath *file_list, *dir_list;
	int n_files;
	CString filename;
	CString macro;

	CString Cfilter = _T("*.drw");
	wchar_t *filter = NULL;
	filter = Cfilter.AllocSysString();
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
	status = ProFilesList(NULL, filter, PRO_FILE_LIST_LATEST, &file_list, &dir_list);

	if (filter != NULL)
		SysFreeString(filter);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 1)
		{
			ProPath savepath;
			status = ProFileSave(NULL, filter, NULL, NULL, NULL, NULL, savepath);
			if (status != PRO_TK_NO_ERROR)
				return;
			status = ProMdlfileCopy(PRO_MDL_DRAWING, file_list[0], savepath);
			if (status != PRO_TK_NO_ERROR)
			{
				AfxMessageBox(_T("�޷������򸲸��ļ�!"));
				status = ProArrayFree((ProArray *)&file_list);
				status = ProArrayFree((ProArray *)&dir_list);
				return;
			}
			filename = CString(savepath);
			filename.Replace(_T("\\"), _T("\\\\"));
			macro = _T("~ Command `ProCmdModelOpen` ;~ Trail `UI Desktop` `UI Desktop` `DLG_PREVIEW_POST` `file_open`;~ Update `file_open` `Inputname` `" + filename + "`;~ Trail `UI Desktop` `UI Desktop` `PREVIEW_POPUP_TIMER` `file_open:Ph_list.Filelist:<NULL>`;~ Command `ProFileSelPushOpen@context_dlg_open_cmd`;");
		}
		else
		{
			AfxMessageBox(_T("��ǰĿ¼�����Ļ�ͼ�ļ�����С��1,������ͼ!"));
		}
		for (int i = 1; i < n_files; i++)
		{
			CString tmp = CString(file_list[i]);
			tmp.Replace(_T("\\"), _T("\\\\"));
			if (tmp == filename)
				continue;
			macro += _T("~ Command `ProCmdDwgImpAppend` ;~ Trail `UI Desktop` `UI Desktop` `DLG_PREVIEW_POST` `file_open`;~ Update `file_open` `Inputname` `") + tmp + _T("`;~ Command `ProFileSelPushOpen@context_dlg_open_cmd`;");
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
	macro += _T("~ Command `About_Act`;");
	hint = Fun;
	wchar_t *p = macro.AllocSysString();
	ProMacroLoad(p);
	SysFreeString(p);
}

void about()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hint == Fun)
	{
		ShowDialog(L"�Զ���ͼ������");
	}
	else
	{
		ShowDialog(L"�����Զ���ͼ��\n�����ҵĲ��ͻ�ȡ������Ϣ��\nhttp://www.hudi.site");
	}
	hint = About;
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId zdptID, AboutId;

	status = ProMenubarMenuAdd("zdpt", "zdpt", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("zdpt", "zdpt", "zdpt", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("zdpt_Act", (uiCmdCmdActFn)zdpt, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &zdptID);
	status = ProMenubarmenuPushbuttonAdd("zdpt", "zdptmenu", "zdptmenu", "zdptmenutips", NULL, PRO_B_TRUE, zdptID, MSGFILE);

	status = ProCmdActionAdd("About_Act", (uiCmdCmdActFn)about, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AboutId);
	status = ProMenubarmenuPushbuttonAdd("zdpt", "About", "About", "Abouttips", NULL, PRO_B_TRUE, AboutId, MSGFILE);

	hint = About;

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
