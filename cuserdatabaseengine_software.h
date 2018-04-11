#ifndef CUSERDATABASE_ENGINE_SOFTWARE_H
#define CUSERDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с базой данных пользователей с использованием программного механизма поиска записей

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "ciuserdatabaseengine.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базой данных пользователей с использованием программного механизма поиска записей
//====================================================================================================

class CUserDatabaseEngine_Software:public CIUserDatabaseEngine
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CString UserListTableName;//имя таблицы базы данных списка пользователей
  CString UserListBaseInitString;//строка инициализации базы данных пользователей
  CDatabase cDatabase_UserList;//база данных списка пользователей
 public:
  //-Конструктор класса------------------------------------------------------
  CUserDatabaseEngine_Software(const CString &user_list_table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CUserDatabaseEngine_Software();
  //-Функции класса----------------------------------------------------------
  bool AddUser(CUser& cUser) override;//добавить пользователя
  bool ChangeUser(long index,const CUser& cUser) override;//задать пользователя
  bool GetUser(long index,CUser &cUser) override;//получить пользователя по индексу
  bool DeleteUser(long index) override;//удалить пользователя по индексу
  list<CUser> GetAllUser(void) override;//получить список всех пользователей
  bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser) override;//найти пользователя по логину и паролю
  bool FindUserByGUID(const CSafeString& guid,CUser& cUser) override;//найти пользователя по GUID
  bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser) override;//изменить пользователя по GUID
  bool DeleteUserByGUID(const CSafeString& guid) override;//удалить пользователя по GUID
  void ResetBase(void) override;//очистить базу
};

#endif