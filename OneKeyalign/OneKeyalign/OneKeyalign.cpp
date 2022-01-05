// OneKeyalign.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "OneKeyalign.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(COneKeyalignApp, CWinApp)
END_MESSAGE_MAP()

COneKeyalignApp::COneKeyalignApp()
{
}

COneKeyalignApp theApp;

BOOL COneKeyalignApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

typedef enum _alignmode
{
	Vertical = 0,
	Horizon = 1,
} ALIGNMENT;

int CurrentMdlType()
{
	ProMdl mdl;
	ProMdlType mdltype;
	ProError status;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	status = ProMdlTypeGet(mdl, &mdltype);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	else
		return mdltype;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_DRAWING)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void DrawRefresh()
{
	Sleep(100);
	ProMacroLoad(_T("~Command `ProCmdWinActivate`;"));
}

void Align(ALIGNMENT alignment)
{
	ProError status;
	ProSelection *SelBuffer = NULL;
	int size;
	ProModelitem Modelitem;
	ProVector Dimlocation;
	ProMdl mdl;
	ProVector Mousepos;
	ProMouseButton Mousebutton;
	ProDtlsyminstdata SymData;
	ProDtlattach SymAttachOld, SymAttachNew;
	ProDtlattachType SymType;
	ProView SymView;
	ProVector location;
	ProSelection attach_point;

	status = ProMdlCurrentGet(&mdl);
	status = ProSelbufferSelectionsGet(&SelBuffer);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet(SelBuffer, &size);
		if (status == PRO_TK_NO_ERROR && size > 0)
		{
			status = ProMessageDisplay(MSGFILE, "MyPosition");
			status = ProMousePickGet(PRO_LEFT_BUTTON, &Mousebutton, Mousepos);

			for (int i = 0; i < size; i++)
			{
				status = ProSelectionModelitemGet(SelBuffer[i], &Modelitem);
				if (status == PRO_TK_NO_ERROR)
				{
					if (Modelitem.type == PRO_DIMENSION || Modelitem.type == PRO_REF_DIMENSION)
					{
						status = ProDrawingDimensionPosGet((ProDrawing)mdl, &Modelitem, Dimlocation);
						if (alignment == Vertical)
							Dimlocation[1] = Mousepos[1];
						else
							Dimlocation[0] = Mousepos[0];
						status = ProDrawingDimensionMove((ProDrawing)mdl, &Modelitem, Dimlocation);
					}
					else if (Modelitem.type == PRO_SYMBOL_INSTANCE)
					{
						status = ProDtlsyminstDataGet(&Modelitem, PRODISPMODE_SYMBOLIC, &SymData);
						status = ProDtlsyminstdataAttachmentGet(SymData, &SymAttachOld);
						status = ProDtlattachGet(SymAttachOld, &SymType, &SymView, location, &attach_point);
						if (status == PRO_TK_NO_ERROR)
						{
							if (alignment == Vertical)
								location[1] = Mousepos[1];
							else
								location[0] = Mousepos[0];
							status = ProDtlattachAlloc(SymType, SymView, location, attach_point, &SymAttachNew);
							status = ProDtlsyminstdataAttachmentSet(SymData, SymAttachNew);
							status = ProDtlsyminstModify(&(ProDtlsyminst)Modelitem, SymData);
							status = ProDtlattachFree(SymAttachNew);
						}
					}
					else
						continue;
				}
			}
		}
		status = ProSelectionarrayFree(SelBuffer);
		DrawRefresh();
	}
}

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

void VerticalAlign()
{
	Align(Vertical);
}

void HorizonAlign()
{
	Align(Horizon);
}

void about()
{
	ShowDialog(_T("ʹ��˵�����ȿ�ѡ���źͳߴ磬�ٵ���˵���ť��������������ͼ��������С�\nCreo�Դ�����ꡢ����ֲڶ��Լ�ģ�ͻ�׼�޷����롣�����ҵĲ��ͻ�ø�����Ϣ��\nhttp://www.hudi.site"));
}

extern "C" int user_initialize()
{
	//MFC��ʼ��
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxInitRichEdit2();
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxEnableControlContainer();

	ProError status;
	uiCmdCmdId VerticalAlignId, HorizonAlignId, AboutId;

	status = ProMenubarMenuAdd("OneKey", "OneKey", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("OneKey", "OneKey", "OneKey", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("VerticalAlign_Act", (uiCmdCmdActFn)VerticalAlign, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &VerticalAlignId);
	status = ProMenubarmenuPushbuttonAdd("OneKey", "VerticalAlign", "VerticalAlign", "VerticalAligntips", NULL, PRO_B_TRUE, VerticalAlignId, MSGFILE);

	status = ProCmdActionAdd("HorizonAlign_Act", (uiCmdCmdActFn)HorizonAlign, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &HorizonAlignId);
	status = ProMenubarmenuPushbuttonAdd("OneKey", "HorizonAlign", "HorizonAlign", "HorizonAligntips", NULL, PRO_B_TRUE, HorizonAlignId, MSGFILE);

	status = ProCmdActionAdd("About_Act", (uiCmdCmdActFn)about, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AboutId);
	status = ProMenubarmenuPushbuttonAdd("OneKey", "About", "About", "Abouttips", NULL, PRO_B_TRUE, AboutId, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}