#include "./includes/AutoWorkDirSetting.h"

int AutoWorkDirSettingFn(uiCmdCmdId command, uiCmdValue *p_value, void *p_push_command_data)
{
	ProError status;
	ProPath exePath, cfgPath;
	status = ProToolkitApplTextPathGet(exePath);
	status = ProWstringCopy(exePath, cfgPath, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(L"\\CreoTool.ini", cfgPath, PRO_VALUE_UNUSED);
	
	if (command == check_but[1].command)
	{
		if (check_but[1].state == PRO_B_FALSE)
		{
			check_but[1].state = PRO_B_TRUE;
			status = ProNotificationSet(PRO_WINDOW_CHANGE_POST, ProUserWindowChangePost); //�л����ڵ��µĵ�ǰģ�ͱ仯
			status = ProNotificationSet(PRO_MDL_SAVE_POST, ProUserMdlSavePost);			  //���Ϊ���µ�·���仯
			status = ProNotificationSet(PRO_MDL_RETRIEVE_POST, ProUserMdlRetrievePost);	  //����ģ�͵��µ�·���仯
			WriteOrUpdateConfig(cfgPath, L"AutoChangeWorkDir", L"True");
		}
		else
		{
			check_but[1].state = PRO_B_FALSE;
			status = ProNotificationUnset(PRO_WINDOW_CHANGE_POST);
			status = ProNotificationUnset(PRO_MDL_SAVE_POST);
			status = ProNotificationUnset(PRO_MDL_RETRIEVE_POST);
			WriteOrUpdateConfig(cfgPath, L"AutoChangeWorkDir", L"False");
		}
	}
	return 0;
}

int AutoWorkDirSettingValueFn(uiCmdCmdId command, uiCmdValue *p_value)
{
	ProError status;
	ProBoolean value;
	if (check_but[1].command == command)
	{
		status = ProMenubarmenuChkbuttonValueGet(p_value, &value);
		if (value == check_but[1].state)
			return 0;
		status = ProMenubarmenuChkbuttonValueSet(p_value, check_but[1].state);
		return 0;
	}
	return 0;
}