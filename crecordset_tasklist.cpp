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
 m_nFields=11;
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
}


//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) для работы с CRecordset_TaskList
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_TaskList::CRAIICRecordset_TaskList(CDatabase *cDatabase_Ptr,const CString &table_name)
{
 Ok=false;
 if (cDatabase_Ptr==NULL) return;
 if (cDatabase_Ptr->IsOpen()==FALSE) return;
 cRecordset_TaskList.m_pDatabase=cDatabase_Ptr;
 if (cRecordset_TaskList.Open(CRecordset::snapshot,table_name,CRecordset::none)==FALSE) return;
 if (cRecordset_TaskList.IsOpen()==FALSE) return;
 Ok=true;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_TaskList::~CRAIICRecordset_TaskList()
{
 if (Ok==false) return;
 if (cRecordset_TaskList.IsOpen()==TRUE) cRecordset_TaskList.Close();
}
//----------------------------------------------------------------------------------------------------
//проверка, произошёл ли захват объекта
//----------------------------------------------------------------------------------------------------
bool CRAIICRecordset_TaskList::IsOk(void)
{
 return(Ok);
}
//----------------------------------------------------------------------------------------------------
//получение оригинального объекта
//----------------------------------------------------------------------------------------------------
CRecordset_TaskList& CRAIICRecordset_TaskList::GetMainObject(void)
{
 return(cRecordset_TaskList);
}
