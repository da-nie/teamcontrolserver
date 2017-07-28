#include "cdocument_main.h"
#include "cthreadserver.h"

extern CThreadServer cThreadServer;//серверный поток

//порт сервера по-умолчанию
#define THREAD_SERVER_PORT_DEFAULT 9090

//====================================================================================================
//конструктор класса
//====================================================================================================
CDocument_Main::CDocument_Main(void) 
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   //считаем настройки сервера
   FILE *file=fopen("server_settings.bin","rb");
   if (file!=NULL)
   {
    fread(&sProtectedVariables.sServerSettings,sizeof(SServerSettings),1,file);
    fclose(file);
   }
   else
   {
    sProtectedVariables.sServerSettings.Port=THREAD_SERVER_PORT_DEFAULT;
   }

   //настраиваем подключение к базам данных
   sProtectedVariables.UserListTableName="UserBase";
   sProtectedVariables.UserListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ=UserBase";

   sProtectedVariables.TaskListTableName="TaskBase";
   sProtectedVariables.TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ=TaskBase";

   sProtectedVariables.ProjectListTableName="ProjBase";
   sProtectedVariables.ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ=ProjBase";

   cThreadServer.SetDocument(this);
  }
 }
/*
 //сохранение базы в файл
 FILE *file=fopen("base.csv","wb");
 list<STask> list_STask=GetAllTask();
 list<SUser> list_SUser=GetAllUser();

 list<STask>::iterator iterator=list_STask.begin();
 list<STask>::iterator iterator_end=list_STask.end();  
 while(iterator!=iterator_end)
 {
  STask &sTask=*iterator;  
  SUser sUser_From;
  SUser sUser_For; 
  
  list<SUser>::iterator iterator_user=list_SUser.begin();
  list<SUser>::iterator iterator_user_end=list_SUser.end();  
  while(iterator_user!=iterator_user_end)
  {
   SUser &sUser=*iterator_user;
   if (sUser.UserGUID.Compare(sTask.ForUserGUID)==0) sUser_For=sUser;
   if (sUser.UserGUID.Compare(sTask.FromUserGUID)==0) sUser_From=sUser;
   iterator_user++;
  }
  fprintf(file,"От: %s;Для:%s;Задание:%s\r\n",sUser_From.Name,sUser_For.Name,sTask.Task);
  iterator++;
 }
 fclose(file);
*/

 cThreadServer.Start();
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDocument_Main::~CDocument_Main() 
{
 cThreadServer.Stop();
 SaveState();
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//сохранение состояния
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SaveState(void)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   //записываем настройки сервера
   FILE *file=fopen("server_settings.bin","wb");
   if (file!=NULL)
   {
    fwrite(&sProtectedVariables.sServerSettings,sizeof(SServerSettings),1,file);
    fclose(file);
   }
  }
 }
}

//----------------------------------------------------------------------------------------------------
//добавить пользователя
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddUser(SUser& sUser)
{
 if (CreateGUID(sUser.UserGUID)==false)
 {
  MessageBox(NULL,"Не могу создать GUID (глобально-уникальный идентификатор) нового пользователя!","Ошибка",MB_OK);
  return(false);
 }
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return(false);
     if (cRAIICRecordset_UserList.GetMainObject().CanAppend()==TRUE)
	 {
      if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_UserList.GetMainObject().MoveLast();
      cRAIICRecordset_UserList.GetMainObject().AddNew();
      cRAIICRecordset_UserList.GetMainObject().SetRecord(sUser); 
      cRAIICRecordset_UserList.GetMainObject().Update();
	 }
	}
   }
  }
 }
 SUser sUser_Added=sUser;
 cThreadServer.OnUserAdded(sUser_Added);//указываем потоку, что произошло добавление пользователя
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить данные пользователя
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeUser(long index,const SUser& sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return(false);
     if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
     cRAIICRecordset_UserList.GetMainObject().Edit();
     cRAIICRecordset_UserList.GetMainObject().SetRecord(sUser);   
     cRAIICRecordset_UserList.GetMainObject().Update();
	}
   }
  }
 }
 SUser sUser_Changed=sUser;
 cThreadServer.OnUserChanged(sUser_Changed);//указываем потоку, что произошло изменение данных пользователя
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить пользователя по индексу
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::GetUser(long index,SUser &sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return(false);
     if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
     cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
	}
   }
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить пользователя по индексу
//----------------------------------------------------------------------------------------------------
void CDocument_Main::DeleteUser(long index)
{
 SUser sUser_Deleted; 
 if (GetUser(index,sUser_Deleted)==false) return;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return;
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return;
     if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return;  
     cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
     cRAIICRecordset_UserList.GetMainObject().Delete();
	}
   }
  }
 }
 cThreadServer.OnUserDeleted(sUser_Deleted);//указываем потоку, что произошло удаление пользователя
}
//----------------------------------------------------------------------------------------------------
//получить список всех пользователей
//----------------------------------------------------------------------------------------------------
list<SUser> CDocument_Main::GetAllUser(void)
{ 
 list<SUser> list_SUser_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return(list_SUser_Local);
     if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(list_SUser_Local);
     cRAIICRecordset_UserList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
	 {
      SUser sUser;
      cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
      list_SUser_Local.push_back(sUser);
      cRAIICRecordset_UserList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 return(list_SUser_Local);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return(false);
     if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_UserList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
	 {
      cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
  	  if (sUser.Login.Compare(login)==0 && sUser.Password.Compare(password)==0) return(true);    
      cRAIICRecordset_UserList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByGUID(const CString& guid,SUser& sUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_UserList cRAIICRecordset_UserList(&sProtectedVariables.cDatabase_UserList,sProtectedVariables.UserListTableName);
     if (cRAIICRecordset_UserList.IsOk()==false) return(false);
     if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_UserList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
	 {
      cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
  	  if (sUser.UserGUID.Compare(guid)==0) return(true);    
      cRAIICRecordset_UserList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//найти задание по GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindTaskByGUID(const CString &guid,STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
	 {
      cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
	  if (sTask.TaskGUID.Compare(guid)==0) return(true);    
      cRAIICRecordset_TaskList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//найти проект по GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindProjectByGUID(const CString &guid,SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListTableName);
     if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
     if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
	 {
      cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
  	  if (sProject.ProjectGUID.Compare(guid)==0) return(true);    
      cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 return(false);
}

//----------------------------------------------------------------------------------------------------
//получить настройки сервера
//----------------------------------------------------------------------------------------------------
void CDocument_Main::GetServerSettings(SServerSettings &sServerSettings)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sServerSettings=sProtectedVariables.sServerSettings;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//установить настройки сервера (сервер будет перезапущен)
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SetServerSettings(const SServerSettings &sServerSettings)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.sServerSettings=sServerSettings;
  }
 }
 //перезапускаем сервер с новыми настройками
 cThreadServer.Start();
}
//----------------------------------------------------------------------------------------------------
//получить все задания для и от пользователя с заданным GUID
//----------------------------------------------------------------------------------------------------
list<STask> CDocument_Main::GetAllTaskForUserGUID(const CString &guid)
{ 
 list<STask> list_STask_Local;  
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(list_STask_Local);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(list_STask_Local);
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_STask_Local);
     cRAIICRecordset_TaskList.GetMainObject().MoveFirst();   
     while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
	 {
      STask sTask;
      cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
      cRAIICRecordset_TaskList.GetMainObject().MoveNext();
      if (sTask.ForUserGUID.Compare(guid)==0 || sTask.FromUserGUID.Compare(guid)==0) list_STask_Local.push_back(sTask);//это нам или наше задание
	 }
	}
   }
  }
 }
 return(list_STask_Local);
}
//----------------------------------------------------------------------------------------------------
//получить все задания
//----------------------------------------------------------------------------------------------------
list<STask> CDocument_Main::GetAllTask(void)
{
 list<STask> list_STask_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(list_STask_Local);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(list_STask_Local);
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_STask_Local);
     cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
	 {
      STask sTask;
      cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
      cRAIICRecordset_TaskList.GetMainObject().MoveNext();
      list_STask_Local.push_back(sTask);
	 }
	}
   } 
  }
 }
 return(list_STask_Local);
}

//----------------------------------------------------------------------------------------------------
//получить все проекты
//----------------------------------------------------------------------------------------------------
list<SProject> CDocument_Main::GetAllProject(void)
{
 list<SProject> list_SProject_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(list_SProject_Local);
	{
     CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListTableName);
     if (cRAIICRecordset_ProjectList.IsOk()==false) return(list_SProject_Local);
     if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(list_SProject_Local);
     cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
	 {
      SProject sProject;
      cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
      cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
      list_SProject_Local.push_back(sProject);
	 }
	}
   } 
  }
 }
 return(list_SProject_Local);
}
//----------------------------------------------------------------------------------------------------
//добавить задание
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddTask(STask &sTask)
{
 if (CreateGUID(sTask.TaskGUID)==false) return(false);
 {
  sTask.State=TASK_STATE_NO_READ;
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
     if (cRAIICRecordset_TaskList.GetMainObject().CanAppend()==TRUE)
	 {
      if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_TaskList.GetMainObject().MoveLast();
      cRAIICRecordset_TaskList.GetMainObject().AddNew();
      cRAIICRecordset_TaskList.GetMainObject().SetRecord(sTask);
      cRAIICRecordset_TaskList.GetMainObject().Update();
	 }
	}
   }
  }
 }
 //считаем задание заново, так как там есть автоинкрементное поле
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
	 {
      STask sTask_Read;
      cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Read);
      cRAIICRecordset_TaskList.GetMainObject().MoveNext();
      if (sTask_Read.TaskGUID.Compare(sTask.TaskGUID)==0)
	  {
       sTask=sTask_Read;
       break;
	  }
	 }
	}
   } 
  }
 }
 STask sTask_Added=sTask;
 cThreadServer.OnTaskAdded(sTask_Added);//указываем потоку, что произошло добавление задания
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить задание
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteTask(const STask &sTask)
{
 bool is_finded=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_TaskList.GetMainObject().MoveFirst();   
     while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
	 {
      STask sTask_Local;
      cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Local);
	  if (sTask_Local.TaskGUID.Compare(sTask.TaskGUID)==0)
	  {
       cRAIICRecordset_TaskList.GetMainObject().Delete();
	   is_finded=true;
	   break;
	  }
      cRAIICRecordset_TaskList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 if (is_finded==false) return(false);
 STask sTask_Deleted=sTask;
 cThreadServer.OnTaskDeleted(sTask_Deleted);//указываем потоку, что произошло удаление задания
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить задание
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeTask(const STask &sTask)
{
 STask sTask_Delete;
 STask sTask_Add;
 bool for_user_change=false;
 bool is_finded=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&sProtectedVariables.cDatabase_TaskList,sProtectedVariables.TaskListTableName);
     if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
	 {
      STask sTask_Local;
      cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Local);
	  if (sTask_Local.TaskGUID.Compare(sTask.TaskGUID)==0)
	  {
       //если у задания поменялся адресат, то отправляем старому адресату сообщение об удалении задания,
       //а новому о новом задании
       if (sTask_Local.ForUserGUID.Compare(sTask.ForUserGUID)!=0)
	   {
        sTask_Delete=sTask_Local;
	    sTask_Add=sTask;
	    for_user_change=true;
	   }
       cRAIICRecordset_TaskList.GetMainObject().Edit();
       cRAIICRecordset_TaskList.GetMainObject().SetRecord(sTask);   
       cRAIICRecordset_TaskList.GetMainObject().Update();
	   is_finded=true;
	   break;
	  }
      cRAIICRecordset_TaskList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 if (is_finded==false) return(false);
 if (for_user_change==true)//измение с заменой адресата
 {
  cThreadServer.OnTaskDeleted(sTask_Delete);//указываем потоку, что произошло удаление задания
  cThreadServer.OnTaskAdded(sTask_Add);//указываем потоку, что произошло добавление задания
  return(true);
 } 
 //изменение без замены адресата
 STask sTask_Changed=sTask;
 cThreadServer.OnTaskChanged(sTask_Changed);//указываем потоку, что произошло изменение данных задания
 return(true);
}


//----------------------------------------------------------------------------------------------------
//добавить проект
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddProject(SProject &sProject)
{
 if (CreateGUID(sProject.ProjectGUID)==false) return(false);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListTableName);
     if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
     if (cRAIICRecordset_ProjectList.GetMainObject().CanAppend()==TRUE)
	 {
      if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_ProjectList.GetMainObject().MoveLast();
      cRAIICRecordset_ProjectList.GetMainObject().AddNew();
      cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);
      cRAIICRecordset_ProjectList.GetMainObject().Update();
	  //считаем проект заново, так как там есть автоинкрементное поле
	  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
	 }
	}
   }
  }
 }
 SProject sProject_Added=sProject;
 cThreadServer.OnProjectAdded(sProject_Added);//указываем потоку, что произошло добавление проекта
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить проект
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteProject(const SProject &sProject)
{
 bool is_finded=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListTableName);
     if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
     if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();   
     while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
	 {
      SProject sProject_Local;
      cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
	  if (sProject_Local.ProjectGUID.Compare(sProject.ProjectGUID)==0)
	  {
       cRAIICRecordset_ProjectList.GetMainObject().Delete();
	   is_finded=true;
	   break;
	  }
      cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 if (is_finded==false) return(false);
 SProject sProject_Deleted=sProject;
 cThreadServer.OnProjectDeleted(sProject_Deleted);//указываем потоку, что произошло удаление проекта
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить проект
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeProject(const SProject &sProject)
{
 bool is_finded=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   CRAIICDatabase cRAIICDatabase(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListBaseInitString);
   {
    if (cRAIICDatabase.IsOpen()==false) return(false);
	{
     CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&sProtectedVariables.cDatabase_ProjectList,sProtectedVariables.ProjectListTableName);
     if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
     if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
     cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
     while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
	 {
      SProject sProject_Local;
      cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
  	  if (sProject_Local.ProjectGUID.Compare(sProject.ProjectGUID)==0)
	  {
       cRAIICRecordset_ProjectList.GetMainObject().Edit();
       cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);   
       cRAIICRecordset_ProjectList.GetMainObject().Update();
	   is_finded=true;
	   break;
	  }
      cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
	 }
	}
   } 
  }
 }
 if (is_finded==false) return(false);
 SProject sProject_Changed=sProject;
 cThreadServer.OnProjectChanged(sProject_Changed);//указываем потоку, что произошло изменение данных проекта
 return(true);
}

//----------------------------------------------------------------------------------------------------
//создать GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::CreateGUID(CString &cString_GUID)
{
 cString_GUID="";
 //вычисляем GUID
 GUID guid;
 HRESULT hr=CoCreateGuid(&guid);
 if (FAILED(hr)) return(false);
 LPOLESTR szGUID=new WCHAR[39];
 StringFromGUID2(guid,szGUID,39);
 long length=WideCharToMultiByte(CP_UTF8,0,szGUID,-1,0,0,0,0);
 char *guid_name=new char[length+1];
 WideCharToMultiByte(CP_ACP,0,szGUID,-1,guid_name,length,0,0);
 cString_GUID=guid_name;
 delete[](guid_name);
 delete[](szGUID);
 return(true);
}

//====================================================================================================
//прочее
//====================================================================================================
IMPLEMENT_DYNCREATE(CDocument_Main,CDocument)
