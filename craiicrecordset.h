#ifndef CRAIIRECORDSET_H
#define CRAIIRECORDSET_H

//====================================================================================================
//��������
//====================================================================================================

//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"

//====================================================================================================
//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset_UserList
//====================================================================================================

template <class CRecordset>
class CRAIICRecordset
{
 private:
  CRAIICRecordset (const CRAIICRecordset& cRAIICRecordset);
  CRAIICRecordset& operator=(const CRAIICRecordset& cRAIICRecordset);
  CRecordset cRecordset;//��������� �������
  bool Ok;//��������� ������� �������
 public:
  CRAIICRecordset(CDatabase *cDatabase_Ptr,const CString &table_name);//�����������
  ~CRAIICRecordset();//����������
  bool IsOk(void);//��������, ��������� �� ������ �������
  CRecordset& GetMainObject(void);//��������� ������������� �������
};

//----------------------------------------------------------------------------------------------------
//�����������
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
//����������
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
CRAIICRecordset<CRecordset>::~CRAIICRecordset()
{
 if (Ok==false) return;
 if (cRecordset.IsOpen()==TRUE) cRecordset.Close();
}
//----------------------------------------------------------------------------------------------------
//��������, ��������� �� ������ �������
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
bool CRAIICRecordset<CRecordset>::IsOk(void)
{
 return(Ok);
}
//----------------------------------------------------------------------------------------------------
//��������� ������������� �������
//----------------------------------------------------------------------------------------------------
template <class CRecordset>
CRecordset& CRAIICRecordset<CRecordset>::GetMainObject(void)
{
 return(cRecordset);
}


#endif