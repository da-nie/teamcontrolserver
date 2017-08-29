#ifndef CDATABASE_ENGINE_SQL_H
#define CDATABASE_ENGINE_SQL_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с базой данных с использованием SQL
//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiicdatabase.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базой заданий с использованием SQL
//====================================================================================================
template<class CRAIICRecordset,class CUnit>
class CDatabaseEngine_SQL
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CString TableName;//имя таблицы базы данных
  CString BaseInitString;//строка инициализации базы данных
  CDatabase cDatabase;//база данных
 public:
  //-Конструктор класса------------------------------------------------------
  CDatabaseEngine_SQL(const CString &table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CDatabaseEngine_SQL();
  //-Функции класса----------------------------------------------------------
  bool Add(CUnit &cUnit);//добавить элемент
  list<CUnit> GetAll(void);//получить все элементы
  bool Delete(const CString &sql_condition);//удалить элемент по условию sql_condition
  bool FindOne(const CString &sql_condition,CUnit &cUnit);//найти по условию sql_condition один элемент
  list<CUnit> FindAll(const CString &sql_condition);//получить по условию sql_condition список элементов
  bool Change(const CString &sql_condition,const CUnit &cUnit);//изменить элемент по условию sql_condition
  void ResetBase(void);//очистить базу
};


#include "cdatabaseengine_sql.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
template<class CRAIICRecordset,class CUnit>
CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::CDatabaseEngine_SQL(const CString &table_name) 
{
 //настраиваем подключение к базе данных
 TableName=table_name;
 BaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TableName;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
template<class CRAIICRecordset,class CUnit>
CDatabaseEngine_SQL<CRAIICRecordset,CUnit>::~CDatabaseEngine_SQL() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//добавить элемент
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
//получить все элементы
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
//удалить элемент по условию sql_condition
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
//найти по условию sql_condition один элемент
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
//получить по условию sql_condition список элементов
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
   list_CUnit_Local.push_back(cUnit);//это нам или наше задание
   cRAIICRecordset.GetMainObject().MoveNext();
  }
 }
 return(list_CUnit_Local);
}
//----------------------------------------------------------------------------------------------------
//изменить элемент по условию sql_condition
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
//очистить базу
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