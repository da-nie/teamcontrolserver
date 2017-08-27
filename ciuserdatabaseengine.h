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
  virtual bool AddUser(CUser& cUser)=0;//�������� ������������
  virtual bool ChangeUser(long index,const CUser& cUser)=0;//������ ������������
  virtual bool GetUser(long index,CUser &cUser)=0;//�������� ������������ �� �������
  virtual bool DeleteUser(long index)=0;//������� ������������ �� �������
  virtual list<CUser> GetAllUser(void)=0;//�������� ������ ���� �������������
  virtual bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser)=0;//����� ������������ �� ������ � ������
  virtual bool FindUserByGUID(const CSafeString& guid,CUser& cUser)=0;//����� ������������ �� GUID
  virtual bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)=0;//�������� ������������ �� GUID
  virtual bool DeleteUserByGUID(const CSafeString& guid)=0;//������� ������������ �� GUID
  virtual void ResetBase(void)=0;//�������� ����
};

#endif