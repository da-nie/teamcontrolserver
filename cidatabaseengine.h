#ifndef CIDATABASE_ENGINE_H
#define CIDATABASE_ENGINE_H

//====================================================================================================
//описание
//====================================================================================================

//Абстрактный класс работы с базами данных

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include <CString>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "crecordset_userlist.h"
#include "crecordset_tasklist.h"
#include "crecordset_projectlist.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//абстрактный класс работы с базами данных
//====================================================================================================

class CIDatabaseEngine
{
 public:
  //-Деструктор класса-------------------------------------------------------
  virtual ~CIDatabaseEngine() {};
  //-Функции класса----------------------------------------------------------
  virtual bool AddUser(SUser& sUser)=0;//добавить пользователя
  virtual bool ChangeUser(long index,const SUser& sUser)=0;//задать пользователя
  virtual bool GetUser(long index,SUser &sUser)=0;//получить пользователя по индексу
  virtual bool DeleteUser(long index)=0;//удалить пользователя по индексу
  virtual list<SUser> GetAllUser(void)=0;//получить список всех пользователей
  virtual bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)=0;//найти пользователя по логину и паролю
  virtual bool FindUserByGUID(const CString& guid,SUser& sUser)=0;//найти пользователя по GUID
  virtual bool FindTaskByGUID(const CString &guid,STask &sTask)=0;//найти задание по GUID
  virtual bool FindProjectByGUID(const CString &guid,SProject &sProject)=0;//найти проект по GUID
  virtual list<STask> GetAllTaskForUserGUID(const CString &guid)=0;//получить все задания для и от пользователя с заданным GUID
  virtual list<STask> GetAllTask(void)=0;//получить все задания
  virtual list<SProject> GetAllProject(void)=0;//получить все проекты
  virtual bool AddTask(STask &sTask)=0;//добавить задание
  virtual bool DeleteTask(const STask &sTask)=0;//удалить задание
  virtual bool ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added)=0;//изменить задание
  virtual bool AddProject(SProject &sProject)=0;//добавить проект
  virtual bool DeleteProject(const SProject &sProject)=0;//удалить проект
  virtual bool ChangeProject(const SProject &sProject)=0;//изменить проект
};

#endif