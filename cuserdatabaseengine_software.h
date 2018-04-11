#ifndef CUSERDATABASE_ENGINE_SOFTWARE_H
#define CUSERDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ����� ������ ������������� � �������������� ������������ ��������� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "ciuserdatabaseengine.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������ ������������� � �������������� ������������ ��������� ������ �������
//====================================================================================================

class CUserDatabaseEngine_Software:public CIUserDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString UserListTableName;//��� ������� ���� ������ ������ �������������
  CString UserListBaseInitString;//������ ������������� ���� ������ �������������
  CDatabase cDatabase_UserList;//���� ������ ������ �������������
 public:
  //-����������� ������------------------------------------------------------
  CUserDatabaseEngine_Software(const CString &user_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CUserDatabaseEngine_Software();
  //-������� ������----------------------------------------------------------
  bool AddUser(CUser& cUser) override;//�������� ������������
  bool ChangeUser(long index,const CUser& cUser) override;//������ ������������
  bool GetUser(long index,CUser &cUser) override;//�������� ������������ �� �������
  bool DeleteUser(long index) override;//������� ������������ �� �������
  list<CUser> GetAllUser(void) override;//�������� ������ ���� �������������
  bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser) override;//����� ������������ �� ������ � ������
  bool FindUserByGUID(const CSafeString& guid,CUser& cUser) override;//����� ������������ �� GUID
  bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser) override;//�������� ������������ �� GUID
  bool DeleteUserByGUID(const CSafeString& guid) override;//������� ������������ �� GUID
  void ResetBase(void) override;//�������� ����
};

#endif