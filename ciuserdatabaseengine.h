#ifndef CIUSERDATABASE_ENGINE_H
#define CIUSERDATABASE_ENGINE_H

//====================================================================================================
//��������
//====================================================================================================

//����������� ����� ������ � ����� ������ �������������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include <CString>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "crecordset_userlist.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����������� ����� ������ � ����� ������ �������������
//====================================================================================================

class CIUserDatabaseEngine
{
 public:
  //-���������� ������-------------------------------------------------------
  virtual ~CIUserDatabaseEngine() {};
  //-������� ������----------------------------------------------------------
  virtual bool AddUser(SUser& sUser)=0;//�������� ������������
  virtual bool ChangeUser(long index,const SUser& sUser)=0;//������ ������������
  virtual bool GetUser(long index,SUser &sUser)=0;//�������� ������������ �� �������
  virtual bool DeleteUser(long index)=0;//������� ������������ �� �������
  virtual list<SUser> GetAllUser(void)=0;//�������� ������ ���� �������������
  virtual bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)=0;//����� ������������ �� ������ � ������
  virtual bool FindUserByGUID(const CString& guid,SUser& sUser)=0;//����� ������������ �� GUID
};

#endif