#ifndef CUSERDATABASE_ENGINE_SQL_H
#define CUSERDATABASE_ENGINE_SQL_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ����� ������ ������������� � �������������� SQL

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "ciuserdatabaseengine.h"
#include "cdatabaseengine_sql.h"
#include "craiicrecordset.h"

#include <memory>


//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������ ������������� � �������������� SQL
//====================================================================================================

class CUserDatabaseEngine_SQL:public CIUserDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_UserList>,CUser> *cDatabaseEngine_SQL_Ptr;//����� ��� ������ � ����� ������
  CString UserListTableName;//��� ������� ���� ������ ������ �������������
  CString UserListBaseInitString;//������ ������������� ���� ������ �������������
  CDatabase cDatabase_UserList;//���� ������ ������ �������������

  std::shared_ptr<CRAIICDatabase> cRAIICDatabase_Ptr;//��������� �� ����� ������� ���� ������
 public:
  //-����������� ������------------------------------------------------------
  CUserDatabaseEngine_SQL(const CString &user_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CUserDatabaseEngine_SQL();
  //-������� ������----------------------------------------------------------
  bool AddUser(CUser& cUser);//�������� ������������
  bool ChangeUser(long index,const CUser& cUser);//������ ������������
  bool GetUser(long index,CUser &cUser);//�������� ������������ �� �������
  bool DeleteUser(long index);//������� ������������ �� �������
  std::list<CUser> GetAllUser(void);//�������� ������ ���� �������������
  bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser);//����� ������������ �� ������ � ������
  bool FindUserByGUID(const CSafeString& guid,CUser& cUser);//����� ������������ �� GUID
  bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser);//�������� ������������ �� GUID
  bool DeleteUserByGUID(const CSafeString& guid);//������� ������������ �� GUID
  void ResetBase(void);//�������� ����
};

#endif