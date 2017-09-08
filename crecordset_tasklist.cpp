#include "crecordset_tasklist.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CRecordset_TaskList::CRecordset_TaskList(CDatabase* pDatabase):CRecordset(pDatabase)
{
 Index=0;
 ForUserGUID="";
 FromUserGUID="";
 ProjectGUID="";
 TaskGUID="";
 Task="";
 State=TASK_STATE_NO_READ;
 Year=0;
 Month=0;
 Day=0;
 Answer="";
 Propertys=0;
 m_nFields=12;
 m_nDefaultType=snapshot;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CRecordset_TaskList::~CRecordset_TaskList()
{
}
//====================================================================================================
//������� ������
//====================================================================================================
CString CRecordset_TaskList::GetDefaultConnect(void)
{	
 return(_T("ODBC;DRIVER=Microsoft Paradox Driver (*.db );DBQ=TaskBase"));
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
CString CRecordset_TaskList::GetDefaultSQL(void)
{
 return(_T("[TaskBase]"));
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void CRecordset_TaskList::DoFieldExchange(CFieldExchange* pFX)
{ 
 pFX->SetFieldType(CFieldExchange::outputColumn);
 RFX_Long(pFX,_T("[Index]"),Index);
 RFX_Text(pFX,_T("[TaskGUID]"),TaskGUID);
 RFX_Text(pFX,_T("[FromUserGUID]"),FromUserGUID);
 RFX_Text(pFX,_T("[ForUserGUID]"),ForUserGUID); 
 RFX_Text(pFX,_T("[ProjectGUID]"),ProjectGUID);
 RFX_Text(pFX,_T("[Task]"),Task);
 RFX_Long(pFX,_T("[State]"),State);  
 RFX_Long(pFX,_T("[Year]"),Year);  
 RFX_Long(pFX,_T("[Month]"),Month);
 RFX_Long(pFX,_T("[Day]"),Day);  
 RFX_Text(pFX,_T("[Answer]"),Answer);
 RFX_Long(pFX,_T("[Propertys]"),Propertys);
}
//----------------------------------------------------------------------------------------------------
//������ ������
//----------------------------------------------------------------------------------------------------
void CRecordset_TaskList::SetRecord(const CTask& cTask)
{
 ForUserGUID=cTask.GetForUserGUID();
 FromUserGUID=cTask.GetFromUserGUID();
 ProjectGUID=cTask.GetProjectGUID();
 TaskGUID=cTask.GetTaskGUID();
 State=cTask.GetState();
 const CDate &cDate=cTask.GetDate();
 Year=cDate.GetYear();
 Month=cDate.GetMonth();
 Day=cDate.GetDay();
 Task=cTask.GetTask();
 Answer=cTask.GetAnswer();
 long p=0;
 if (cTask.IsAnswerNotRead()==true) p|=TASK_RECORD_PROPERTYS_MASK_ANSWER_NOT_READ;
 Propertys=p;
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
void CRecordset_TaskList::GetRecord(CTask& cTask)
{
 cTask.SetForUserGUID(ForUserGUID);
 cTask.SetFromUserGUID(FromUserGUID);
 cTask.SetProjectGUID(ProjectGUID);
 cTask.SetTaskGUID(TaskGUID);
 cTask.SetTask(Task);
 cTask.SetState(State);
 CDate cDate(Year,Month,Day);
 cTask.SetDate(cDate);
 cTask.SetIndex(Index);
 cTask.SetAnswer(Answer);

 if (Propertys&TASK_RECORD_PROPERTYS_MASK_ANSWER_NOT_READ) cTask.SetAnswerNotRead(true);
                                                      else cTask.SetAnswerNotRead(false);
}
