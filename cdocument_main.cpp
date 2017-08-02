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
   sProtectedVariables.cIUserDatabaseEngine_Ptr=new CUserDatabaseEngine_Software("UserBase");
   sProtectedVariables.cIProjectDatabaseEngine_Ptr=new CProjectDatabaseEngine_Software("ProjBase");
   sProtectedVariables.cITaskDatabaseEngine_Ptr=new CTaskDatabaseEngine_Software("TaskBase");
   cThreadServer.SetDocument(this);
  }
 }
 cThreadServer.Start();
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDocument_Main::~CDocument_Main() 
{
 cThreadServer.Stop();
 SaveState();
 delete(sProtectedVariables.cIUserDatabaseEngine_Ptr);
 delete(sProtectedVariables.cIProjectDatabaseEngine_Ptr);
 delete(sProtectedVariables.cITaskDatabaseEngine_Ptr);
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
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->AddUser(sUser)==false) return(false);
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
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->ChangeUser(index,sUser)==false) return(false);
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
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->GetUser(index,sUser)==false) return(false);
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
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->DeleteUser(index)==false) return;
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
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->GetAllUser());
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
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->FindUserByLoginAndPassword(login,password,sUser));
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
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->FindUserByGUID(guid,sUser));
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
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->FindTaskByGUID(guid,sTask));
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
   return(sProtectedVariables.cIProjectDatabaseEngine_Ptr->FindProjectByGUID(guid,sProject));
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
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTaskForUserGUID(guid));
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
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask());
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
   return(sProtectedVariables.cIProjectDatabaseEngine_Ptr->GetAllProject());
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
 sTask.State=TASK_STATE_NO_READ;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->AddTask(sTask)==false) return(false);
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
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->DeleteTask(sTask)==false) return(false);
  }
 }
 STask sTask_Deleted=sTask;
 cThreadServer.OnTaskDeleted(sTask_Deleted);//указываем потоку, что произошло удаление задания
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить задание
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeTask(const STask &sTask)
{
 STask sTask_Deleted;
 STask sTask_Added;
 bool for_user_change=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->ChangeTask(sTask,for_user_change,sTask_Deleted,sTask_Added)==false) return(false);
  }
 }
 if (for_user_change==true)//измение с заменой адресата
 {
  cThreadServer.OnTaskDeleted(sTask_Deleted);//указываем потоку, что произошло удаление задания
  cThreadServer.OnTaskAdded(sTask_Added);//указываем потоку, что произошло добавление задания
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
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->AddProject(sProject)==false) return(false);
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
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->DeleteProject(sProject)==false) return(false);
  }
 }
 SProject sProject_Deleted=sProject;
 cThreadServer.OnProjectDeleted(sProject_Deleted);//указываем потоку, что произошло удаление проекта
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить проект
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeProject(const SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->ChangeProject(sProject)==false) return(false);
  }
 }
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
