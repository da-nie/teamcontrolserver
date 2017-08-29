#ifndef CTASKDATABASE_ENGINE_SQL_H
#define CTASKDATABASE_ENGINE_SQL_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с баой данных заданий с использованием SQL
//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "citaskdatabaseengine.h"
#include "cdatabaseengine_sql.h"
#include "craiicrecordset.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базой данных заданий с использованием SQL
//====================================================================================================

class CTaskDatabaseEngine_SQL:public CITaskDatabaseEngine
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_TaskList>,CTask> *cDatabaseEngine_SQL_Ptr;//класс для работы с базой данных
  CString TaskListTableName;//имя таблицы базы данных списка задач  
  CString TaskListBaseInitString;//строка инициализации базы данных заданий
  CDatabase cDatabase_TaskList;//база данных списка задач
 public:
  //-Конструктор класса------------------------------------------------------
  CTaskDatabaseEngine_SQL(const CString &task_list_table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CTaskDatabaseEngine_SQL();
  //-Функции класса----------------------------------------------------------
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask);//найти задание по GUID
  list<CTask> GetAllTaskForUserGUID(const CSafeString &guid);//получить все задания для и от пользователя с заданным GUID
  list<CTask> GetAllTask(void);//получить все задания
  bool AddTask(CTask &cTask);//добавить задание
  bool DeleteTask(const CTask &cTask);//удалить задание
  bool ChangeTask(const CTask &cTask,bool &for_user_change,CTask &cTask_Deleted,CTask &cTask_Added);//изменить задание
  void ResetBase(void);//очистить базу
};

#endif