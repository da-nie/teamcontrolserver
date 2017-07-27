#ifndef CRECORDSET_PROJECT_LIST_H
#define CRECORDSET_PROJECT_LIST_H

//====================================================================================================
//описание
//====================================================================================================

//Класс записи информации о проектах

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <string>

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//параметры проекта
struct SProject
{
 CString ProjectName;//название проекта
 CString ProjectGUID;//уникальный идентификатор
};


//====================================================================================================
//класс записи данных о проектах
//====================================================================================================
class CRecordset_ProjectList:public CRecordset
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CString ProjectName;//имя
  CString ProjectGUID;//идентификатор
 public:
  //-Конструктор класса------------------------------------------------------
  CRecordset_ProjectList(CDatabase* pDatabase=NULL);
  //-Деструктор класса-------------------------------------------------------
  ~CRecordset_ProjectList();
  //-Функции класса----------------------------------------------------------
  void SetRecord(const SProject& sProject);//задать запись
  void GetRecord(SProject& sProject);//получить запись
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) для работы с CRecordset_ProjectList
//====================================================================================================

class CRAIICRecordset_ProjectList
{
 private:
  CRAIICRecordset_ProjectList (const CRAIICRecordset_ProjectList& cRAIICRecordset_ProjectList);
  CRAIICRecordset_ProjectList& operator=(const CRAIICRecordset_ProjectList& cRAIICRecordset_ProjectList);
  CRecordset_ProjectList cRecordset_ProjectList;//экземпляр объекта
  bool Ok;//результат захвата объекта
 public:
  CRAIICRecordset_ProjectList(CDatabase *cDatabase_Ptr,const CString &table_name);//конструктор
  ~CRAIICRecordset_ProjectList();//деструктор
  bool IsOk(void);//проверка, произошёл ли захват объекта
  CRecordset_ProjectList& GetMainObject(void);//получение оригинального объекта
};

#endif