#ifndef CRAII_CDATABASE_H
#define CRAII_CDATABASE_H

//====================================================================================================
//��������
//====================================================================================================

//����� RAII ��� ������ � ����� ������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "stdafx.h"

using namespace std;

//====================================================================================================
//����� RAII ���� ������
//====================================================================================================
class CRAIICDatabase
{
 private:
  CDatabase *cDatabase_Ptr;//��������� �� ����� ���� ������
 public:
  CRAIICDatabase(CDatabase *cDatabase_Set_Ptr,const char *init)//�����������
  {
   cDatabase_Ptr=cDatabase_Set_Ptr;
   cDatabase_Set_Ptr->Open(NULL,FALSE,FALSE,init,TRUE);
  }
  ~CRAIICDatabase()//����������
  {
   if (cDatabase_Ptr->IsOpen()==TRUE) cDatabase_Ptr->Close();
  }
  bool IsOpen(void)//������ �� ����
  {
   if (cDatabase_Ptr->IsOpen()==TRUE) return(true);
   return(false);
  }
};

#endif