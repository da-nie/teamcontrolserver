#ifndef CDATABASE_ENGINE_SQL_H
#define CDATABASE_ENGINE_SQL_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ����� ������ � �������������� SQL
//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiicdatabase.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������� � �������������� SQL
//====================================================================================================
template<class CRAIICRecordset,class CUnit>
class CDatabaseEngine_SQL
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString TableName;//��� ������� ���� ������
  CString BaseInitString;//������ ������������� ���� ������
  CDatabase cDatabase;//���� ������
 public:
  //-����������� ������------------------------------------------------------
  CDatabaseEngine_SQL(const CString &table_name);
  //-���������� ������-------------------------------------------------------
  ~CDatabaseEngine_SQL();
  //-������� ������----------------------------------------------------------
  bool Add(CUnit &cUnit);//�������� �������
  list<CUnit> GetAll(void);//�������� ��� ��������
  bool Delete(const CString &sql_condition);//������� ������� �� ������� sql_condition
  bool FindOne(const CString &sql_condition,CUnit &cUnit);//����� �� ������� sql_condition ���� �������
  list<CUnit> FindAll(const CString &sql_condition);//�������� �� ������� sql_condition ������ ���������
  bool Change(const CString &sql_condition,const CUnit &cUnit);//�������� ������� �� ������� sql_condition
  void ResetBase(void);//�������� ����
};


#include "cdatabaseengine_sql.h"

//====================================================================================================
//����������� ������
//====================================================================================================
template<class CRAIICRecordset,class CUnit>
CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::CDatabaseEngine_SQL(const CString &table_name) 
{
 //����������� ����������� � ���� ������
 TableName=table_name;
 BaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TableName;
}
//====================================================================================================
//���������� ������
//====================================================================================================
template<class CRAIICRecordset,class CUnit>
CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::~CDatabaseEngine_SQL() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
bool CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::Add(CUnit &cUnit)
{
 {
  CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
  {
   if (cRAIICDatabase.IsOpen()==false) return(false);
   {
    CRAIICRecordset cRAIICRecordset(&cDatabase,TableName);
    if (cRAIICRecordset.IsOk()==false) return(false);
    if (cRAIICRecordset.GetMainObject().CanAppend()==TRUE)
    {
     if (cRAIICRecordset.GetMainObject().GetRecordCount()!=0) cRAIICRecordset.GetMainObject().MoveLast();
     cRAIICRecordset.GetMainObject().AddNew();
     cRAIICRecordset.GetMainObject().SetRecord(cUnit);
     cRAIICRecordset.GetMainObject().Update();
    }
   }
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� ��������
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
list<CUnit> CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::GetAll(void)
{
 list<CUnit> list_CUnit_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_CUnit_Local);
  {
   CRAIICRecordset cRAIICRecordset(&cDatabase,TableName);
   if (cRAIICRecordset.IsOk()==false) return(list_CUnit_Local);
   if (cRAIICRecordset.GetMainObject().GetRecordCount()==0) return(list_CUnit_Local);
   cRAIICRecordset.GetMainObject().MoveFirst();
   while(cRAIICRecordset.GetMainObject().IsEOF()==FALSE)
   {
    CUnit cUnit;
    cRAIICRecordset.GetMainObject().GetRecord(cUnit);
    cRAIICRecordset.GetMainObject().MoveNext();
    list_CUnit_Local.push_back(cUnit);
   }
  }
 } 
 return(list_CUnit_Local);
}
//----------------------------------------------------------------------------------------------------
//������� ������� �� ������� sql_condition
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
bool CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::Delete(const CString &sql_condition)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TableName;
  sql_request+=" WHERE ("+sql_condition+")";
  CRAIICRecordset cRAIICRecordset(&cDatabase,sql_request);
  if (cRAIICRecordset.IsOk()==false) return(false);
  if (cRAIICRecordset.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset.GetMainObject().MoveFirst();
  if (cRAIICRecordset.GetMainObject().IsEOF()==TRUE) return(false);
  CUnit cUnit_Local;
  cRAIICRecordset.GetMainObject().GetRecord(cUnit_Local);
  cRAIICRecordset.GetMainObject().Delete();
  return(true);
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//����� �� ������� sql_condition ���� �������
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
bool CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::FindOne(const CString &sql_condition,CUnit &cUnit)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TableName;
  sql_request+=" WHERE ("+sql_condition+")";
  CRAIICRecordset cRAIICRecordset(&cDatabase,sql_request);
  if (cRAIICRecordset.IsOk()==false) return(false);
  if (cRAIICRecordset.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset.GetMainObject().MoveFirst();
  if (cRAIICRecordset.GetMainObject().IsEOF()==TRUE) return(false);
  cRAIICRecordset.GetMainObject().GetRecord(cUnit);
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� �� ������� sql_condition ������ ���������
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
list<CUnit> CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::FindAll(const CString &sql_condition)
{ 
 list<CUnit> list_CUnit_Local;  
 CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_CUnit_Local);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TableName;
  sql_request+=" WHERE ("+sql_condition+")";
  CRAIICRecordset cRAIICRecordset(&cDatabase,sql_request);
  if (cRAIICRecordset.IsOk()==false) return(list_CUnit_Local);
  if (cRAIICRecordset.GetMainObject().GetRecordCount()==0) return(list_CUnit_Local);
  cRAIICRecordset.GetMainObject().MoveFirst();
  while(cRAIICRecordset.GetMainObject().IsEOF()==FALSE)
  {
   CUnit cUnit;
   cRAIICRecordset.GetMainObject().GetRecord(cUnit);
   list_CUnit_Local.push_back(cUnit);//��� ��� ��� ���� �������
   cRAIICRecordset.GetMainObject().MoveNext();
  }
 }
 return(list_CUnit_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �� ������� sql_condition
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
bool CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::Change(const CString &sql_condition,const CUnit &cUnit)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TableName;
  sql_request+=" WHERE ("+sql_condition+")";
  CRAIICRecordset cRAIICRecordset(&cDatabase,sql_request);
  if (cRAIICRecordset.IsOk()==false) return(false);
  if (cRAIICRecordset.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset.GetMainObject().MoveFirst();
  if (cRAIICRecordset.GetMainObject().IsEOF()==TRUE) return(false);
  CUnit cUnit_Local;
  cRAIICRecordset.GetMainObject().GetRecord(cUnit_Local);
  cRAIICRecordset.GetMainObject().Edit();
  cRAIICRecordset.GetMainObject().SetRecord(cUnit);   
  cRAIICRecordset.GetMainObject().Update();
  return(true);
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
template<class CRAIICRecordset,class CUnit>
void CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::ResetBase(void)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase,BaseInitString);
 {
  CString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=TableName;
  cDatabase.ExecuteSQL(sql_request);
 }
}

#endif