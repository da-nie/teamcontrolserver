#include "crecordset_tasklist.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CRecordset_TaskList::CRecordset_TaskList(CDatabase* pDatabase):CRecordset(pDatabase)
{
 ForUserGUID="";
 FromUserGUID="";
 ProjectGUID="";
 TaskGUID="";
 Task="";
 State=TASK_STATE_NO_READ;
 Year=0;
 Month=0;
 Day=0;
 m_nFields=10;
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
 RFX_Text(pFX,_T("[TaskGUID]"),TaskGUID);
 RFX_Text(pFX,_T("[ForUserGUID]"),ForUserGUID); 
 RFX_Text(pFX,_T("[FromUserGUID]"),FromUserGUID);
 RFX_Text(pFX,_T("[ProjectGUID]"),ProjectGUID);
 RFX_Text(pFX,_T("[Task]"),Task);
 RFX_Long(pFX,_T("[State]"),State);  
 RFX_Long(pFX,_T("[Year]"),Year);  
 RFX_Long(pFX,_T("[Month]"),Month);
 RFX_Long(pFX,_T("[Day]"),Day); 
}
//----------------------------------------------------------------------------------------------------
//������ ������
//----------------------------------------------------------------------------------------------------
void CRecordset_TaskList::SetRecord(const STask& sTask)
{
 ForUserGUID=sTask.ForUserGUID;
 FromUserGUID=sTask.FromUserGUID;
 ProjectGUID=sTask.ProjectGUID;
 TaskGUID=sTask.TaskGUID;
 State=sTask.State;
 Year=sTask.Year;
 Month=sTask.Month;
 Day=sTask.Day;
 Task=sTask.Task;
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
void CRecordset_TaskList::GetRecord(STask& sTask)
{
 sTask.ForUserGUID=ForUserGUID;
 sTask.FromUserGUID=FromUserGUID;
 sTask.ProjectGUID=ProjectGUID;
 sTask.TaskGUID=TaskGUID;
 sTask.Task=Task;
 sTask.State=State;
 sTask.Year=Year;
 sTask.Month=Month;
 sTask.Day=Day;
}


//====================================================================================================
//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset_TaskList
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�����������
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
//����������
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_TaskList::~CRAIICRecordset_TaskList()
{
 if (Ok==false) return;
 if (cRecordset_TaskList.IsOpen()==TRUE) cRecordset_TaskList.Close();
}
//----------------------------------------------------------------------------------------------------
//��������, ��������� �� ������ �������
//----------------------------------------------------------------------------------------------------
bool CRAIICRecordset_TaskList::IsOk(void)
{
 return(Ok);
}
//----------------------------------------------------------------------------------------------------
//��������� ������������� �������
//----------------------------------------------------------------------------------------------------
CRecordset_TaskList& CRAIICRecordset_TaskList::GetMainObject(void)
{
 return(cRecordset_TaskList);
}
