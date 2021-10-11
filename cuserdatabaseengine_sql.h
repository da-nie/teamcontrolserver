#ifndef CUSERDATABASE_ENGINE_SQL_H
#define CUSERDATABASE_ENGINE_SQL_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с базой данных пользователей с использованием SQL

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "ciuserdatabaseengine.h"
#include "cdatabaseengine_sql.h"
#include "craiicrecordset.h"

#include <memory>


//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базой данных пользователей с использованием SQL
//====================================================================================================

class CUserDatabaseEngine_SQL:public CIUserDatabaseEngine
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_UserList>,CUser> *cDatabaseEngine_SQL_Ptr;//класс для работы с базой данных
  CString UserListTableName;//имя таблицы базы данных списка пользователей
  CString UserListBaseInitString;//строка инициализации базы данных пользователей
  CDatabase cDatabase_UserList;//база данных списка пользователей

  std::shared_ptr<CRAIICDatabase> cRAIICDatabase_Ptr;//указатель на класс захвата базы данных
 public:
  //-Конструктор класса------------------------------------------------------
  CUserDatabaseEngine_SQL(const CString &user_list_table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CUserDatabaseEngine_SQL();
  //-Функции класса----------------------------------------------------------
  bool AddUser(CUser& cUser);//добавить пользователя
  bool ChangeUser(long index,const CUser& cUser);//задать пользователя
  bool GetUser(long index,CUser &cUser);//получить пользователя по индексу
  bool DeleteUser(long index);//удалить пользователя по индексу
  std::list<CUser> GetAllUser(void);//получить список всех пользователей
  bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser);//найти пользователя по логину и паролю
  bool FindUserByGUID(const CSafeString& guid,CUser& cUser);//найти пользователя по GUID
  bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser);//изменить пользователя по GUID
  bool DeleteUserByGUID(const CSafeString& guid);//удалить пользователя по GUID
  void ResetBase(void);//очистить базу
};

#endif