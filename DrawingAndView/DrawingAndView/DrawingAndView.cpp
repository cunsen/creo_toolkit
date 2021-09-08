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

ProError _createDrawing(CString TemplateName)
{
	ProMdl solidmdl;
	ProPath currentpath;
	ProError status;
	ProMdlType mdltype;
	ProMdldata data;
	ProModel model;
	ProDrawing created_drawing = NULL;
	ProDwgcreateOptions options = PRODWGCREATE_DISPLAY_DRAWING;
	ProDwgcreateErrs errors;
	int new_win_id;

	status = ProMdlCurrentGet(&solidmdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(solidmdl, &mdltype);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdltype == PRO_MDL_PART || mdltype == PRO_MDL_ASSEMBLY)
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

void Getsize(ProDrawing drawing, int sheet)
{
	ProError status;
	ProPlotPaperSize size_type;
	double width, height;
	status = ProDrawingFormatSizeGet(drawing, sheet, &size_type, &width, &height);
}
ProError _createView()
{
	ProError status;
	ProMdl mdl;
	ProPoint3d position;
	//ProPoint3d outline[2];
	//ProVector to_vec;
	ProMatrix matrix;
	ProView positive_view, top_view, left_view;
	ProDrawing drawing;
	ProSolid solid;
	ProMdlType mdltype;
	//ProPlotPaperSize p_size;
	//double p_width, p_height;
	int sheet;

	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(mdl, &mdltype);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdltype == PRO_DRAWING)
	{
		drawing = (ProDrawing)mdl;
		status = ProDrawingCurrentsolidGet(drawing, &solid);
		status = ProDrawingCurrentSheetGet(drawing, &sheet);

		//Getsize(drawing,sheet);
		//��ͼ��С
		// status = ProDrawingFormatSizeGet(drawing,sheet,&p_size,&p_width,&p_height);
		//////////////����ڷ�λ��,������ϵΪ��׼
		position[0] = 200;
		position[1] = 600;
		position[2] = 0;

		//////////////����ڷŷ���,FRONT
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrix[i][j] = 0;
			}
		}
		matrix[0][0] = 1;
		matrix[1][1] = 1;
		matrix[2][2] = 1;
		matrix[3][3] = 1;

		status = ProDrawingGeneralviewCreate(drawing, solid, sheet, PRO_B_FALSE, position, 1, matrix, &positive_view);
		//��ͼ��С
		//status = ProDrawingViewOutlineGet(drawing,positive_view,outline);
		//�ƶ���ͼ
		// �ƶ���ͼ��Ӧ������
		// to_vec[0] = position[0] - 100;
		// to_vec[1] = position[1] - 10;
		// to_vec[2] = position[2] - 0;
		//status = ProDrawingViewMove(drawing,positive_view,to_vec);

		position[0] += 500;
		status = ProDrawingProjectedviewCreate(drawing, positive_view, PRO_B_FALSE, position, &top_view);

		position[0] -= 500;
		position[1] -= 400;
		status = ProDrawingProjectedviewCreate(drawing, positive_view, PRO_B_FALSE, position, &left_view);

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
	ProView auxiliaryview;
	ProDrawing drawing;
	ProMdlType mdltype;
	ProSelection *sel;
	int n_sel;
	int sheet;
	ProPoint3d point;
	//����ʵ��ȷ��λ��
	point[0] = 200;
	point[1] = 700;
	point[2] = 0;

	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProMdlTypeGet(mdl, &mdltype);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if (mdltype == PRO_DRAWING)
	{
		drawing = (ProDrawing)mdl;
		status = ProDrawingCurrentSheetGet(drawing, &sheet);

		drawing = (ProDrawing)mdl;
		status = ProSelect((char *)"edge", 1, NULL, NULL, NULL, NULL, &sel, &n_sel);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProDrawingViewAuxiliaryCreate(drawing, *sel, point, &auxiliaryview);
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

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId CreateDrwID, CreateViewID, CreateAuxiliaryViewID;

	status = ProMenubarMenuAdd("DrawingAndView", "DrawingAndView", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("DrawingAndView", "DrawingAndView", "DrawingAndView", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("CreateDrw_Act", (uiCmdCmdActFn)CreateDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &CreateDrwID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateDrw", "CreateDrw", "CreateDrwtips", NULL, PRO_B_TRUE, CreateDrwID, MSGFILE);

	status = ProCmdActionAdd("CreateView_Act", (uiCmdCmdActFn)CreateView, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &CreateViewID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateView", "CreateView", "CreateViewtips", NULL, PRO_B_TRUE, CreateViewID, MSGFILE);

	status = ProCmdActionAdd("CreateAuxiliaryView_Act", (uiCmdCmdActFn)CreateAuxiliaryView, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &CreateAuxiliaryViewID);
	status = ProMenubarmenuPushbuttonAdd("DrawingAndView", "CreateAuxiliaryView", "CreateAuxiliaryView", "CreateAuxiliaryViewtips", NULL, PRO_B_TRUE, CreateAuxiliaryViewID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}