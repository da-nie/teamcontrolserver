#include "crecordset_tasklist.h"

//====================================================================================================
//конструктор класса
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
 AnswerReference="";
 TaskReference="";
 Propertys=0;
 m_nFields=14;
 m_nDefaultType=snapshot;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CRecordset_TaskList::~CRecordset_TaskList()
{
}
//====================================================================================================
//функции класса
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
 RFX_Text(pFX,_T("[AnswerReference]"),AnswerReference);
 RFX_Text(pFX,_T("[TaskReference]"),TaskReference);
}
//----------------------------------------------------------------------------------------------------
//задать запись
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
 if (cTask.IsPlannedPosition()==true) p|=TASK_RECORD_PROPERTYS_MASK_PLANNED_POSITION;
 if (cTask.IsAnswerReferenceExist()==true) p|=TASK_RECORD_PROPERTYS_MASK_ANSWER_REFERENCE;
 if (cTask.IsTaskReferenceExist()==true) p|=TASK_RECORD_PROPERTYS_MASK_TASK_REFERENCE;
 if (cTask.IsCommon()==true) p|=TASK_RECORD_PROPERTYS_MASK_COMMON;
 Propertys=p;
 AnswerReference=cTask.GetAnswerReference();
 TaskReference=cTask.GetTaskReference();
}
//----------------------------------------------------------------------------------------------------
//получить запись
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
 if (Propertys&TASK_RECORD_PROPERTYS_MASK_PLANNED_POSITION) cTask.SetPlannedPosition(true);
                                                       else cTask.SetPlannedPosition(false);
 if (Propertys&TASK_RECORD_PROPERTYS_MASK_TASK_REFERENCE) cTask.SetTaskReferenceExist(true);
                                                     else cTask.SetTaskReferenceExist(false);
 if (Propertys&TASK_RECORD_PROPERTYS_MASK_ANSWER_REFERENCE) cTask.SetAnswerReferenceExist(true);
                                                       else cTask.SetAnswerReferenceExist(false);
 if (Propertys&TASK_RECORD_PROPERTYS_MASK_COMMON) cTask.SetCommon(true);
                                             else cTask.SetCommon(false);
 cTask.SetAnswerReference(AnswerReference);
 cTask.SetTaskReference(TaskReference);
}
