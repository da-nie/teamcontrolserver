#ifndef CRECORDSET_USERLIST_H
#define CRECORDSET_USERLIST_H

//====================================================================================================
//описание
//====================================================================================================

//Класс записи информации о пользователях

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include "cuser.h"
#include <string>

using namespace std;

//====================================================================================================
//класс записи данных пользователя
//====================================================================================================
class CRecordset_UserList:public CRecordset
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CString Name;//имя
  CString JobTitle;//должность
  CString Login;//логин
  CString Password;//пароль
  CString Description;//описание
  CString Telephone;//телефон
  CString UserGUID;//идентификатор
  int Leader;//является ли лидером
 public:
  //-Конструктор класса------------------------------------------------------
  CRecordset_UserList(CDatabase* pDatabase=NULL);
  //-Деструктор класса-------------------------------------------------------
  ~CRecordset_UserList();
  //-Функции класса----------------------------------------------------------
  void SetRecord(const CUser& cUser);//задать запись
  void GetRecord(CUser& cUser);//получить запись
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) для работы с CRecordset_UserList
//====================================================================================================

class CRAIICRecordset_UserList
{
 private:
  CRAIICRecordset_UserList (const CRAIICRecordset_UserList& cRAIICRecordset_UserList);
  CRAIICRecordset_UserList& operator=(const CRAIICRecordset_UserList& cRAIICRecordset_UserList);
  CRecordset_UserList cRecordset_UserList;//экземпляр объекта
  bool Ok;//результат захвата объекта
 public:
  CRAIICRecordset_UserList(CDatabase *cDatabase_Ptr,const CString &table_name);//конструктор
  ~CRAIICRecordset_UserList();//деструктор
  bool IsOk(void);//проверка, произошёл ли захват объекта
  CRecordset_UserList& GetMainObject(void);//получение оригинального объекта
};

#endif