// DrawingAndView.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DrawingAndView.h"

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

// CDrawingAndViewApp

BEGIN_MESSAGE_MAP(CDrawingAndViewApp, CWinApp)
END_MESSAGE_MAP()

// CDrawingAndViewApp ����

CDrawingAndViewApp::CDrawingAndViewApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CDrawingAndViewApp ����

CDrawingAndViewApp theApp;

// CDrawingAndViewApp ��ʼ��

BOOL CDrawingAndViewApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CurrentMdlType()
{
	ProMdl mdl;
	ProMdlType mdlType;
	ProError status;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	status = ProMdlTypeGet(mdl, &mdlType);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	else
		return mdlType;
}

static uiCmdAccessState AccessPRTorASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_ASSEMBLY || CurrentMdlType() == PRO_PART)
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

ProError _setDisplayStyle(ProDrawing drawing, ProView view, ProDisplayStyle style)
{
	ProError status;
	ProDrawingViewDisplay displayStatus;
	status = ProDrawingViewDisplayGet(drawing, view, &displayStatus);
	displayStatus.style = style;
	status = ProDrawingViewDisplaySet(drawing, view, &displayStatus);
	return status;
}

ProError _createDrawing(CString TemplateName)
{
	ProMdl solidmdl;
	ProPath currentpath;
	ProError status;
	ProMdlType mdlType;
	ProMdldata data;
	ProModel model;
	ProDrawing created_drawing = NULL;
	ProDwgcreateOptions options = PRODWGCREATE_DISPLAY_DRAWING;
	ProDwgcreateErrs errors;
	int new_win_id;

	status = ProMdlCurrentGet(&solidmdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(solidmdl, &mdlType);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdlType == PRO_MDL_PART || mdlType == PRO_MDL_ASSEMBLY)
	{
		status = ProMdlDataGet(solidmdl, &data);
		status = ProDirectoryCurrentGet(currentpath);
		CString Path = CString(data.device) + _T(":") + CString(data.path);
		wchar_t *path = Path.AllocSysString();
		status = ProDirectoryChange(path);
		if (path != NULL)
			SysFreeString(path);

		for (int i = 0; i < 10; i++)
		{
			model.type[i] = data.type[i];
		}
		for (int i = 0; i < 80; i++)
		{
			model.name[i] = data.name[i];
		}

		wchar_t *wtemplatename = NULL;
		wtemplatename = TemplateName.AllocSysString();

		status = ProDrawingFromTmpltCreate(model.name, wtemplatename, &model, options, &created_drawing, &errors);

		if (wtemplatename != NULL)
			SysFreeString(wtemplatename);

		if (status == PRO_TK_NO_ERROR)
		{
			status = ProObjectwindowCreate(model.name, PRO_DRAWING, &new_win_id);
		}
		else if (status == PRO_TK_E_FOUND)
		{
			status = ProMdlLoad(model.name, PRO_MDL_DRAWING, PRO_B_FALSE, (ProMdl *)&created_drawing);
			status = ProObjectwindowCreate(model.name, PRO_DRAWING, &new_win_id);
		}
		else
		{
			status = ProDirectoryCurrentGet(currentpath);
			return PRO_TK_NO_ERROR;
		}
	}
	else
	{
		return PRO_TK_E_NOT_FOUND;
	}
	status = ProMdlDisplay(created_drawing);
	status = ProWindowActivate(new_win_id);
	status = ProDirectoryChange(currentpath);
	return PRO_TK_NO_ERROR;
}

void CreateDrw()
{
	if (_createDrawing(_T("D:\\mydoc\\creo_toolkit\\DrawingAndView\\drwtpl.drw")) == PRO_TK_NO_ERROR)
	{
		//do some thing
	}
}

ProError _createView()
{
	ProError status;
	ProMdl mdl;
	ProPoint3d position;
	ProMatrix matrix;
	ProView positive_view, top_view, left_view;
	ProDrawing drawing;
	ProSolid solid;
	ProMdlType mdlType;
	int sheet;

	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(mdl, &mdlType);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdlType == PRO_DRAWING)
	{
		drawing = (ProDrawing)mdl;
		status = ProDrawingCurrentsolidGet(drawing, &solid);
		status = ProDrawingCurrentSheetGet(drawing, &sheet);

		position[0] = 200;
		position[1] = 600;
		position[2] = 0;

		//////////////����ڷŷ���,FRONT�����ñ���0.015
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrix[i][j] = i == j ? 1 : 0;
			}
		}

		status = ProDrawingGeneralviewCreate(drawing, solid, sheet, PRO_B_FALSE, position, 1, matrix, &positive_view);
		status = _setDisplayStyle(drawing, positive_view, PRO_DISPSTYLE_HIDDEN_LINE);
		status = ProDrawingViewScaleSet(drawing, positive_view, 0.15);

		position[0] += 500;
		status = ProDrawingProjectedviewCreate(drawing, positive_view, PRO_B_FALSE, position, &top_view);
		status = _setDisplayStyle(drawing, top_view, PRO_DISPSTYLE_HIDDEN_LINE);

		position[0] -= 500;
		position[1] -= 400;
		status = ProDrawingProjectedviewCreate(drawing, positive_view, PRO_B_FALSE, position, &left_view);
		status = _setDisplayStyle(drawing, left_view, PRO_DISPSTYLE_HIDDEN_LINE);
		status = ProDwgSheetRegenerate(drawing, sheet);
		return PRO_TK_NO_ERROR;
	}
	else
		return PRO_TK_BAD_CONTEXT;
}

void CreateView()
{
	if (_createView() == PRO_TK_NO_ERROR)
	{
		//do some thing
	}
}

ProError _createAuxiliayView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProMdl mdl;
	ProView auxiliaryView;
	ProDrawing drawing;
	ProMdlType mdlType;
	ProSelection *sel;
	int n_sel;
	int sheet;
	ProPoint3d point;
	//����ʵ��ȷ��λ��
	point[0] = 300;
	point[1] = 0;
	point[2] = 0;

	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(mdl, &mdlType);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdlType == PRO_DRAWING)
	{
		drawing = (ProDrawing)mdl;
		status = ProDrawingCurrentSheetGet(drawing, &sheet);

		drawing = (ProDrawing)mdl;
		AfxMessageBox(_T("��ѡ��һ���������ɸ�����ͼ��"));
		status = ProSelect((char *)"edge", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProDrawingViewAuxiliaryCreate(drawing, *sel, point, &auxiliaryView);
			status = _setDisplayStyle(drawing, auxiliaryView, PRO_DISPSTYLE_HIDDEN_LINE);
			status = ProDwgSheetRegenerate(drawing, sheet);
			return PRO_TK_NO_ERROR;
		}
		else
			return status;
	}
	else
		return PRO_TK_BAD_CONTEXT;
}

void CreateAuxiliaryView()
{
	if (_createAuxiliayView() == PRO_TK_NO_ERROR)
	{
		//do some thing
	}
}

ProError _create2DSectionView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProMdl mdl;
	ProView parentView, _2DSectionView;
	ProDrawing drawing;
	ProSolid solid;
	ProMdlType mdlType;
	ProSelection *sel;
	int n_sel;
	int sheet;
	ProPoint3d refPoint;

	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(mdl, &mdlType);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdlType == PRO_DRAWING)
	{
		drawing = (ProDrawing)mdl;
		status = ProDrawingCurrentSheetGet(drawing, &sheet);
		status = ProDrawingCurrentsolidGet(drawing, &solid);

		AfxMessageBox(_T("������Ϊʾ�������Դ���д����Ĭ�ϴ�������ͼ�������Ϊ\"TESTSEC\"��������ȷ����ǰģ���Ѵ�����һ����Ϊ\"TESTSEC\"�Ľ��档\n��������ѡ��һ����ͼ����������ͼ��"));
		status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProSelectionPoint3dGet(sel[0], refPoint);
			status = ProSelectionViewGet(sel[0], &parentView);
			refPoint[1] -= 500;
			status = ProDrawingProjectedviewCreate(drawing, parentView, PRO_B_FALSE, refPoint, &_2DSectionView);
			status = ProDrawingView2DSectionSet(drawing, _2DSectionView, L"TESTSEC", PRO_VIEW_SECTION_AREA_FULL, NULL, NULL, parentView);
			status = _setDisplayStyle(drawing, _2DSectionView, PRO_DISPSTYLE_HIDDEN_LINE);
			status = ProDwgSheetRegenerate(drawing, sheet);
			return PRO_TK_NO_ERROR;
		}
		else
			return status;
	}
	else
		return PRO_TK_BAD_CONTEXT;
}

void Create2DSectionView()
{
	if (_create2DSectionView() == PRO_TK_NO_ERROR)
	{
		//do some thing
	}
}

ProError _createDetailedView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProMdl mdl;
	ProView parentView, detailedView;
	ProDrawing drawing;
	ProMdlType mdlType;
	ProSelection *sel;
	ProSolid solid;
	int n_sel;
	int sheet;
	ProPoint3d refPoint, locationPoint;
	//����ʵ��ȷ��λ��
	locationPoint[0] = 0;
	locationPoint[1] = 300;
	locationPoint[2] = 0;

	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(mdl, &mdlType);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdlType == PRO_DRAWING)
	{
		drawing = (ProDrawing)mdl;
		status = ProDrawingCurrentSheetGet(drawing, &sheet);
		status = ProDrawingCurrentsolidGet(drawing, &solid);

		AfxMessageBox(_T("��ѡ��һ������������ϸ��ͼ��"));
		status = ProSelect((char *)"edge", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProSelectionPoint3dGet(sel[0], refPoint);
			status = ProSelectionViewGet(sel[0], &parentView);

			ProSelection *sel1;
			int n_sel1;

			status = ProSelect((char *)"draft_ent", 1, NULL, NULL, NULL, NULL, &sel1, &n_sel1);

			ProGeomitem geomItem;
			ProCurve p_curve;
			status = ProSelectionModelitemGet(sel[0], &geomItem);
			status = ProGeomitemToCurve(&geomItem, &p_curve);
			ProGeomitemdata *gitem_data = NULL;
			status = ProCurveDataGet(p_curve, &gitem_data);

			//ProCurvedata* curve_data = 	(ProCurvedata*)(gitem_data[0].data);

			//status = ProDrawingViewDetailCreate(drawing,parentView,sel[0],curve_data,locationPoint,&detailedView);

			status = ProGeomitemdataFree(&gitem_data);
		}
		else
			return status;
	}
	else
		return PRO_TK_BAD_CONTEXT;
}

void CreateDetailedView()
{
	if (_createDetailedView() == PRO_TK_NO_ERROR)
	{
		//do some thing
	}
}
extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId CreateDrwID, CreateViewID, CreateAuxiliaryViewID, CreateDetailedViewID, Create2DSectionViewID;

	status = ProMenubarMenuAdd("DrawingAndView", "DrawingAndView", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("DrawingAndView", "DrawingAndView", "DrawingAndView", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("CreateDrw_Act", (uiCmdCmdActFn)CreateDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &CreateDrwID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateDrw", "CreateDrw", "CreateDrwtips", NULL, PRO_B_TRUE, CreateDrwID, MSGFILE);

	status = ProCmdActionAdd("CreateView_Act", (uiCmdCmdActFn)CreateView, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &CreateViewID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateView", "CreateView", "CreateViewtips", NULL, PRO_B_TRUE, CreateViewID, MSGFILE);

	status = ProCmdActionAdd("CreateAuxiliaryView_Act", (uiCmdCmdActFn)CreateAuxiliaryView, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &CreateAuxiliaryViewID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateAuxiliaryView", "CreateAuxiliaryView", "CreateAuxiliaryViewtips", NULL, PRO_B_TRUE, CreateAuxiliaryViewID, MSGFILE);

	status = ProCmdActionAdd("CreateDetailedView_Act", (uiCmdCmdActFn)CreateDetailedView, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &CreateDetailedViewID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateDetailedView", "CreateDetailedView", "CreateDetailedViewtips", NULL, PRO_B_TRUE, CreateDetailedViewID, MSGFILE);

	status = ProCmdActionAdd("Create2DSectionView_Act", (uiCmdCmdActFn)Create2DSectionView, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &Create2DSectionViewID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "Create2DSectionView", "Create2DSectionView", "Create2DSectionViewtips", NULL, PRO_B_TRUE, Create2DSectionViewID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}