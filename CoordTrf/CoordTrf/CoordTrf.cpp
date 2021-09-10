// CoordTrf.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CoordTrf.h"

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

// CCoordTrfApp

BEGIN_MESSAGE_MAP(CCoordTrfApp, CWinApp)
END_MESSAGE_MAP()

// CCoordTrfApp ����

CCoordTrfApp::CCoordTrfApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CCoordTrfApp ����

CCoordTrfApp theApp;

// CCoordTrfApp ��ʼ��

BOOL CCoordTrfApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

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

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_DRAWING)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void CoordComponentToAsm()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProPoint3d pointCompCoord, pointAsmCoord;
	ProSelection *sel;
	int n_sel;
	ProMatrix transformation;
	ProAsmcomppath comppath;
	CString message;
	AfxMessageBox(_T("ѡ��һ����鿴���������װ�����е����꣬ע��ѡȡʱ������ͼ�ж�����Ԫ������ѡ����������������ݻ᲻��ȷ��"));
	status = ProSelect("point", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProSelectionPoint3dGet(sel[0], pointCompCoord);
		status = ProSelectionAsmcomppathGet(sel[0], &comppath);
		status = ProAsmcomppathTrfGet(&comppath, PRO_B_TRUE, transformation);
		status = ProPntTrfEval(pointCompCoord, transformation, pointAsmCoord);

		message.Format("�������Ĭ������ϵ�µ�����Ϊ(%f,%f,%f)��\n��װ����Ĭ������ϵ�µ�����Ϊ(%f,%f,%f)", pointCompCoord[0], pointCompCoord[1], pointCompCoord[2], pointAsmCoord[0], pointAsmCoord[1], pointAsmCoord[2]);
		AfxMessageBox(message);
	}
}

void CoordViewtoScreen()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProPoint3d pointOrig, pointDrawingCoord, pointViewCoord;
	ProView view;
	ProSelection *sel;
	int n_sel;
	ProMatrix transViewtoScreen;
	ProMdl mdl;
	pointOrig[0] = 0;
	pointOrig[1] = 0;
	pointOrig[2] = 0;

	CString message;
	pointViewCoord[0] = 0;
	pointViewCoord[1] = 0;
	pointViewCoord[2] = 0;
	AfxMessageBox(_T("ѡ��һ����ͼ�鿴��ͼԭ������ת������Ļ�ϵ����ꡣ"));
	status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProSelectionViewGet(sel[0], &view);
		status = ProMdlCurrentGet(&mdl);
		status = ProDrawingViewTransformGet(ProDrawing(mdl), view, PRO_B_TRUE, transViewtoScreen);
		status = ProPntTrfEval(pointViewCoord, transViewtoScreen, pointDrawingCoord);
		message.Format("��ͼԭ�������ڻ�ͼ������ϵ������Ϊ(%f,%f)", pointDrawingCoord[0], pointDrawingCoord[1]);
		AfxMessageBox(message);
		AfxMessageBox(_T("������������ƴ���Ļԭ�㵽ѡ����ֱ�ߣ�ˢ�´��ڡ����������ƶ���������ͼֱ�߻���ʧ��"));
		ProGraphicsPenPosition(pointOrig);
		ProGraphicsLineDraw(pointDrawingCoord);
	}
}

void CoordsolidtoScreen()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProPoint3d pointOrig, pointCompCoord, pointAsmCoord, pointScreenCoord;
	ProAsmcomppath comppath;
	ProView view;
	ProSelection *sel;
	int n_sel;
	ProMdl mdl;
	ProSolid solid;
	CString message;

	pointOrig[0] = 0;
	pointOrig[1] = 0;
	pointOrig[2] = 0;

	ProMatrix transComptoAsm, transAsmtoScreen;
	AfxMessageBox(_T("ѡ��һ����鿴���������װ���弰��ǰ��Ļ�µ��е�����"));
	status = ProSelect((char *)"point", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProSelectionPoint3dGet(sel[0], pointCompCoord);
		status = ProSelectionAsmcomppathGet(sel[0], &comppath);
		status = ProAsmcomppathTrfGet(&comppath, PRO_B_TRUE, transComptoAsm);
		status = ProPntTrfEval(pointCompCoord, transComptoAsm, pointAsmCoord);

		status = ProMdlCurrentGet(&mdl);
		status = ProDrawingCurrentsolidGet(ProDrawing(mdl), &solid);
		status = ProSelectionViewGet(sel[0], &view);
		status = ProViewMatrixGet(ProMdl(solid), view, transAsmtoScreen);
		status = ProPntTrfEval(pointAsmCoord, transAsmtoScreen, pointScreenCoord);

		message.Format("�������Ĭ������ϵ�µ�����Ϊ(%f,%f,%f)��\n��װ����Ĭ������ϵ�µ�����Ϊ(%f,%f,%f)��\n�ڵ�ǰ��Ļ�µ�����Ϊ(%f,%f)", pointCompCoord[0], pointCompCoord[1], pointCompCoord[2], pointAsmCoord[0], pointAsmCoord[1], pointAsmCoord[2], pointScreenCoord[0], pointScreenCoord[1]);
		AfxMessageBox(message);
		AfxMessageBox(_T("������������ƴ���Ļԭ�㵽ѡ����ֱ�ߣ�ˢ�´��ڡ����������ƶ���������ͼֱ�߻���ʧ��"));
		ProGraphicsPenPosition(pointOrig);
		ProGraphicsLineDraw(pointScreenCoord);
	}
}
extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId ComponentToAsmID, ViewtoScreenID, solidtoScreenID;

	status = ProMenubarMenuAdd("CoordTrf", "CoordTrf", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CoordTrf", "CoordTrf", "CoordTrf", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ComponentToAsm_Act", (uiCmdCmdActFn)CoordComponentToAsm, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &ComponentToAsmID);
	status = ProMenubarmenuPushbuttonAdd("CoordTrf", "ComponentToAsm", "ComponentToAsm", "ComponentToAsmtips", NULL, PRO_B_TRUE, ComponentToAsmID, MSGFILE);

	status = ProCmdActionAdd("ViewtoScreen_Act", (uiCmdCmdActFn)CoordViewtoScreen, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &ViewtoScreenID);
	status = ProMenubarmenuPushbuttonAdd("CoordTrf", "ViewtoScreen", "ViewtoScreen", "ViewtoScreentips", NULL, PRO_B_TRUE, ViewtoScreenID, MSGFILE);

	status = ProCmdActionAdd("solidtoScreen_Act", (uiCmdCmdActFn)CoordsolidtoScreen, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &solidtoScreenID);
	status = ProMenubarmenuPushbuttonAdd("CoordTrf", "solidtoScreen", "solidtoScreen", "solidtoScreentips", NULL, PRO_B_TRUE, solidtoScreenID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
