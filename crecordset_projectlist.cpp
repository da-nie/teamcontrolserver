#include "crecordset_projectlist.h"

//====================================================================================================
//конструктор класса
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
//деструктор класса
//====================================================================================================
CRecordset_ProjectList::~CRecordset_ProjectList()
{		
}
//====================================================================================================
//функции класса
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
 RFX_Text(pFX,_T("[ProjectGUID]"),ProjectGUID);
 RFX_Text(pFX,_T("[ProjectName]"),ProjectName); 
}
//----------------------------------------------------------------------------------------------------
//задать запись
//----------------------------------------------------------------------------------------------------
void CRecordset_ProjectList::SetRecord(const CProject& cProject)
{
 ProjectName=cProject.GetProjectName();
 ProjectGUID=cProject.GetProjectGUID(); 
}
//----------------------------------------------------------------------------------------------------
//получить запись
//----------------------------------------------------------------------------------------------------
void CRecordset_ProjectList::GetRecord(CProject& cProject)
{
 cProject.SetProjectName(ProjectName);
 cProject.SetProjectGUID(ProjectGUID);
}
