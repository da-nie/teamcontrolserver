#ifndef CIDATABASE_ENGINE_SOFTWARE_H
#define CIDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с базами данных с использованием программного механизма поиска записей

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "cidatabaseengine.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базами данных с использованием программного механизма поиска записей
//====================================================================================================

class CDatabaseEngine_Software:public CIDatabaseEngine
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CString UserListTableName;//имя таблицы базы данных списка пользователей
  CString TaskListTableName;//имя таблицы базы данных списка задач  
  CString ProjectListTableName;//имя таблицы базы данных списка проектов  

  CString UserListBaseInitString;//строка инициализации базы данных пользователей
  CString TaskListBaseInitString;//строка инициализации базы данных заданий
  CString ProjectListBaseInitString;//строка инициализации базы данных проектов

  CDatabase cDatabase_UserList;//база данных списка пользователей
  CDatabase cDatabase_TaskList;//база данных списка задач
  CDatabase cDatabase_ProjectList;//база данных списка проектов
 public:
  //-Конструктор класса------------------------------------------------------
  CDatabaseEngine_Software(const CString &user_list_table_name,const CString &task_list_table_name,const CString &project_list_table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CDatabaseEngine_Software();
  //-Функции класса----------------------------------------------------------
  bool AddUser(SUser& sUser);//добавить пользователя
  bool ChangeUser(long index,const SUser& sUser);//задать пользователя
  bool GetUser(long index,SUser &sUser);//получить пользователя по индексу
  bool DeleteUser(long index);//удалить пользователя по индексу
  list<SUser> GetAllUser(void);//получить список всех пользователей
  bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser);//найти пользователя по логину и паролю
  bool FindUserByGUID(const CString& guid,SUser& sUser);//найти пользователя по GUID
  bool FindTaskByGUID(const CString &guid,STask &sTask);//найти задание по GUID
  bool FindProjectByGUID(const CString &guid,SProject &sProject);//найти проект по GUID
  list<STask> GetAllTaskForUserGUID(const CString &guid);//получить все задания для и от пользователя с заданным GUID
  list<STask> GetAllTask(void);//получить все задания
  list<SProject> GetAllProject(void);//получить все проекты
  bool AddTask(STask &sTask);//добавить задание
  bool DeleteTask(const STask &sTask);//удалить задание
  bool ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added);//изменить задание
  bool AddProject(SProject &sProject);//добавить проект
  bool DeleteProject(const SProject &sProject);//удалить проект
  bool ChangeProject(const SProject &sProject);//изменить проект
};

#endif