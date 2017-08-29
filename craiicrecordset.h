#ifndef CRAIIRECORDSET_H
#define CRAIIRECORDSET_H

//====================================================================================================
//описание
//====================================================================================================

// ласс RAII (Resource Acquisition is Initialization) дл€ работы с CRecordset

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"

//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) дл€ работы с CRecordset_UserList
//====================================================================================================

template <class CRecordset>
class CRAIICRecordset
{
 private:
  CRAIICRecordset (const CRAIICRecordset& cRAIICRecordset);
  CRAIICRecordset& operator=(const CRAIICRecordset& cRAIICRecordset);
  CRecordset cRecordset;//экземпл€р объекта
  bool Ok;//результат захвата объекта
 public:
  CRAIICRecordset(CDatabase *cDatabase_Ptr,const CString &table_name);//конструктор
  ~CRAIICRecordset();//деструктор
  bool IsOk(void);//проверка, произошЄл ли захват объекта
  CRecordset& GetMainObject(void);//получение оригинального объекта
};

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
CRAIICRecordset<CRecordset>::CRAIICRecordset(CDatabase *cDatabase_Ptr,const CString &table_name)
{
 Ok=false;
 if (cDatabase_Ptr==NULL) return;
 if (cDatabase_Ptr->IsOpen()==FALSE) return;
 cRecordset.m_pDatabase=cDatabase_Ptr;
 if (cRecordset.Open(CRecordset::snapshot,table_name,CRecordset::none)==FALSE) return;
 if (cRecordset.IsOpen()==FALSE) return;
 Ok=true;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
CRAIICRecordset<CRecordset>::~CRAIICRecordset()
{
 if (Ok==false) return;
 if (cRecordset.IsOpen()==TRUE) cRecordset.Close();
}
//----------------------------------------------------------------------------------------------------
//проверка, произошЄл ли захват объекта
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
bool CRAIICRecordset<CRecordset>::IsOk(void)
{
 return(Ok);
}
//----------------------------------------------------------------------------------------------------
//получение оригинального объекта
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
CRecordset& CRAIICRecordset<CRecordset>::GetMainObject(void)
{
 return(cRecordset);
}


#endif