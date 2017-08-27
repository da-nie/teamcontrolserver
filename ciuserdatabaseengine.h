#ifndef CIUSERDATABASE_ENGINE_H
#define CIUSERDATABASE_ENGINE_H

//====================================================================================================
//описание
//====================================================================================================

//Абстрактный класс работы с базой данных пользователей

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "crecordset_userlist.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//абстрактный класс работы с базой данных пользователей
//====================================================================================================

class CIUserDatabaseEngine
{
 public:
  //-Деструктор класса-------------------------------------------------------
  virtual ~CIUserDatabaseEngine() {};
  //-Функции класса----------------------------------------------------------
  virtual bool AddUser(CUser& cUser)=0;//добавить пользователя
  virtual bool ChangeUser(long index,const CUser& cUser)=0;//задать пользователя
  virtual bool GetUser(long index,CUser &cUser)=0;//получить пользователя по индексу
  virtual bool DeleteUser(long index)=0;//удалить пользователя по индексу
  virtual list<CUser> GetAllUser(void)=0;//получить список всех пользователей
  virtual bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser)=0;//найти пользователя по логину и паролю
  virtual bool FindUserByGUID(const CSafeString& guid,CUser& cUser)=0;//найти пользователя по GUID
  virtual bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)=0;//изменить пользователя по GUID
  virtual bool DeleteUserByGUID(const CSafeString& guid)=0;//удалить пользователя по GUID
  virtual void ResetBase(void)=0;//очистить базу
};

#endif