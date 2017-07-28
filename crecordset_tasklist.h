#ifndef CRECORDSET_TASKLIST_H
#define CRECORDSET_TASKLIST_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ ���������� � �������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <string>
#include "protocol.h"
using namespace std;

//====================================================================================================
//���������
//====================================================================================================

struct STask
{
 long Index;//������ �������
 CString FromUserGUID;//�� ������������
 CString ForUserGUID;//��� ������������
 CString ProjectGUID;//�� �������
 CString Task;//�������
 long State;//���������
 long Year;//���
 long Month;//�����
 long Day;//�����
 CString TaskGUID;//������������� �������
};

//====================================================================================================
//����� ������ ���������� � �������
//====================================================================================================
class CRecordset_TaskList:public CRecordset
{
 protected:
  //-���������� ������-------------------------------------------------------
  long Index;//������ �������
  CString FromUserGUID;//�� ����
  CString ForUserGUID;//��� ����
  CString ProjectGUID;//�� �������
  CString Task;//�������
  long State;//��������� �������
  long Year;//���
  long Month;//�����
  long Day;//�����
  CString TaskGUID;//������������� �������  
 public:
  //-����������� ������------------------------------------------------------
  CRecordset_TaskList(CDatabase* pDatabase=NULL);
  //-���������� ������-------------------------------------------------------
  ~CRecordset_TaskList();
  //-������� ������----------------------------------------------------------
  void SetRecord(const STask& sTask);//������ ������
  void GetRecord(STask& sTask);//�������� ������
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//����� RAII (Resource Acquisition is Initialization) ��� ������ � CRecordset_TaskList
//====================================================================================================

class CRAIICRecordset_TaskList
{
 private:
  CRAIICRecordset_TaskList (const CRAIICRecordset_TaskList& cRAIICRecordset_TaskList);
  CRAIICRecordset_TaskList& operator=(const CRAIICRecordset_TaskList& cRAIICRecordset_TaskList);
  CRecordset_TaskList cRecordset_TaskList;//��������� �������
  bool Ok;//��������� ������� �������
 public:
  CRAIICRecordset_TaskList(CDatabase *cDatabase_Ptr,const CString &table_name);//�����������
  ~CRAIICRecordset_TaskList();//����������
  bool IsOk(void);//��������, ��������� �� ������ �������
  CRecordset_TaskList& GetMainObject(void);//��������� ������������� �������
};

#endif