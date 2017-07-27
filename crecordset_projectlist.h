#ifndef CRECORDSET_PROJECT_LIST_H
#define CRECORDSET_PROJECT_LIST_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ ���������� � ��������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <string>

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//��������� �������
struct SProject
{
 CString ProjectName;//�������� �������
 CString ProjectGUID;//���������� �������������
};


//====================================================================================================
//����� ������ ������ � ��������
//====================================================================================================
class CRecordset_ProjectList:public CRecordset
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString ProjectName;//���
  CString ProjectGUID;//�������������
 public:
  //-����������� ������------------------------------------------------------
  CRecordset_ProjectList(CDatabase* pDatabase=NULL);
  //-���������� ������-------------------------------------------------------
  ~CRecordset_ProjectList();
  //-������� ������----------------------------------------------------------
  void SetRecord(const SProject& sProject);//������ ������
  void GetRecord(SProject& sProject);//�������� ������
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset_ProjectList
//====================================================================================================

class CRAIICRecordset_ProjectList
{
 private:
  CRAIICRecordset_ProjectList (const CRAIICRecordset_ProjectList& cRAIICRecordset_ProjectList);
  CRAIICRecordset_ProjectList& operator=(const CRAIICRecordset_ProjectList& cRAIICRecordset_ProjectList);
  CRecordset_ProjectList cRecordset_ProjectList;//��������� �������
  bool Ok;//��������� ������� �������
 public:
  CRAIICRecordset_ProjectList(CDatabase *cDatabase_Ptr,const CString &table_name);//�����������
  ~CRAIICRecordset_ProjectList();//����������
  bool IsOk(void);//��������, ��������� �� ������ �������
  CRecordset_ProjectList& GetMainObject(void);//��������� ������������� �������
};

#endif