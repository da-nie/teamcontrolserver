#include "crecordset_projectlist.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CRecordset_ProjectList::CRecordset_ProjectList(CDatabase* pDatabase):CRecordset(pDatabase)
{
 ProjectName="";
 ProjectGUID="";
 Index=0;
 m_nFields=3;
 m_nDefaultType=snapshot;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CRecordset_ProjectList::~CRecordset_ProjectList()
{		
}
//====================================================================================================
//������� ������
//====================================================================================================
CString CRecordset_ProjectList::GetDefaultConnect(void)
{
 return(_T("ODBC;DRIVER=Microsoft Paradox Driver (*.db );DBQ=ProjBase"));
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
CString CRecordset_ProjectList::GetDefaultSQL(void)
{
 return(_T("[ProjBase]"));
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void CRecordset_ProjectList::DoFieldExchange(CFieldExchange* pFX)
{ 
 pFX->SetFieldType(CFieldExchange::outputColumn);  
 RFX_Long(pFX,_T("[Index]"),Index);
 RFX_Text(pFX,_T("[ProjectName]"),ProjectName); 
 RFX_Text(pFX,_T("[ProjectGUID]"),ProjectGUID);
}
//----------------------------------------------------------------------------------------------------
//������ ������
//----------------------------------------------------------------------------------------------------
void CRecordset_ProjectList::SetRecord(const SProject& sProject)
{
 ProjectName=sProject.ProjectName;
 ProjectGUID=sProject.ProjectGUID; 
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
void CRecordset_ProjectList::GetRecord(SProject& sProject)
{
 sProject.ProjectName=ProjectName;
 sProject.ProjectGUID=ProjectGUID;
}


//====================================================================================================
//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset_ProjectList
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_ProjectList::CRAIICRecordset_ProjectList(CDatabase *cDatabase_Ptr,const CString &table_name)
{
 Ok=false;
 if (cDatabase_Ptr==NULL) return;
 if (cDatabase_Ptr->IsOpen()==FALSE) return;
 cRecordset_ProjectList.m_pDatabase=cDatabase_Ptr;
 if (cRecordset_ProjectList.Open(CRecordset::snapshot,table_name,CRecordset::none)==FALSE) return;
 if (cRecordset_ProjectList.IsOpen()==FALSE) return;
 Ok=true;
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_ProjectList::~CRAIICRecordset_ProjectList()
{
 if (Ok==false) return;
 if (cRecordset_ProjectList.IsOpen()==TRUE) cRecordset_ProjectList.Close();
}
//----------------------------------------------------------------------------------------------------
//��������, ��������� �� ������ �������
//----------------------------------------------------------------------------------------------------
bool CRAIICRecordset_ProjectList::IsOk(void)
{
 return(Ok);
}
//----------------------------------------------------------------------------------------------------
//��������� ������������� �������
//----------------------------------------------------------------------------------------------------
CRecordset_ProjectList& CRAIICRecordset_ProjectList::GetMainObject(void)
{
 return(cRecordset_ProjectList);
}
