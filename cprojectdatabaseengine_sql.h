#ifndef CPROJECTDATABASE_ENGINE_SQL_H
#define CPROJECTDATABASE_ENGINE_SQL_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с базой данных проектов с использованием SQL

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "ciprojectdatabaseengine.h"
#include "cdatabaseengine_sql.h"
#include "craiicrecordset.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базой данных проектов с использованием SQL
//====================================================================================================

class CProjectDatabaseEngine_SQL:public CIProjectDatabaseEngine
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_ProjectList>,CProject> *cDatabaseEngine_SQL_Ptr;//класс для работы с базой данных

  CString ProjectListTableName;//имя таблицы базы данных списка проектов  
  CString ProjectListBaseInitString;//строка инициализации базы данных проектов
  CDatabase cDatabase_ProjectList;//база данных списка проектов
 public:
  //-Конструктор класса------------------------------------------------------
  CProjectDatabaseEngine_SQL(const CString &project_list_table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CProjectDatabaseEngine_SQL();
  //-Функции класса----------------------------------------------------------
  bool FindProjectByGUID(const CSafeString &guid,CProject &cProject);//найти проект по GUID
  list<CProject> GetAllProject(void);//получить все проекты
  bool AddProject(CProject &cProject);//добавить проект
  bool DeleteProject(const CProject &cProject);//удалить проект
  bool ChangeProject(const CProject &cProject);//изменить проект
  void ResetBase(void);//очистить базу
};

#endif