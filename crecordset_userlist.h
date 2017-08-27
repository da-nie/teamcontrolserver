#ifndef CRECORDSET_USERLIST_H
#define CRECORDSET_USERLIST_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ ���������� � �������������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include "cuser.h"
#include <string>

using namespace std;

//====================================================================================================
//����� ������ ������ ������������
//====================================================================================================
class CRecordset_UserList:public CRecordset
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString Name;//���
  CString JobTitle;//���������
  CString Login;//�����
  CString Password;//������
  CString Description;//��������
  CString Telephone;//�������
  CString UserGUID;//�������������
  int Leader;//�������� �� �������
 public:
  //-����������� ������------------------------------------------------------
  CRecordset_UserList(CDatabase* pDatabase=NULL);
  //-���������� ������-------------------------------------------------------
  ~CRecordset_UserList();
  //-������� ������----------------------------------------------------------
  void SetRecord(const CUser& cUser);//������ ������
  void GetRecord(CUser& cUser);//�������� ������
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset_UserList
//====================================================================================================

class CRAIICRecordset_UserList
{
 private:
  CRAIICRecordset_UserList (const CRAIICRecordset_UserList& cRAIICRecordset_UserList);
  CRAIICRecordset_UserList& operator=(const CRAIICRecordset_UserList& cRAIICRecordset_UserList);
  CRecordset_UserList cRecordset_UserList;//��������� �������
  bool Ok;//��������� ������� �������
 public:
  CRAIICRecordset_UserList(CDatabase *cDatabase_Ptr,const CString &table_name);//�����������
  ~CRAIICRecordset_UserList();//����������
  bool IsOk(void);//��������, ��������� �� ������ �������
  CRecordset_UserList& GetMainObject(void);//��������� ������������� �������
};

#endif