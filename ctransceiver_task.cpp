#include "ctransceiver_task.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_Task::CTransceiver_Task(void)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver_Task::~CTransceiver_Task()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//передать клиенту данные задания
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::SendTaskDataToClient(SClient &sClient,const STask &sTask,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SServerAnswer::STaskDataHeader sServerAnswer_sTaskDataHeader;
 sServerAnswer_sTaskDataHeader.FromUserGUIDSize=sTask.FromUserGUID.GetLength();
 sServerAnswer_sTaskDataHeader.ForUserGUIDSize=sTask.ForUserGUID.GetLength();
 sServerAnswer_sTaskDataHeader.ProjectGUIDSize=sTask.ProjectGUID.GetLength();
 sServerAnswer_sTaskDataHeader.TaskSize=sTask.Task.GetLength();
 sServerAnswer_sTaskDataHeader.TaskGUIDSize=sTask.TaskGUID.GetLength();
 sServerAnswer_sTaskDataHeader.AnswerSize=sTask.Answer.GetLength();
 sServerAnswer_sTaskDataHeader.Year=sTask.Year;
 sServerAnswer_sTaskDataHeader.Month=sTask.Month;
 sServerAnswer_sTaskDataHeader.Day=sTask.Day;
 sServerAnswer_sTaskDataHeader.State=sTask.State;
 sServerAnswer_sTaskDataHeader.Index=sTask.Index;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sTaskDataHeader),sizeof(SServerAnswer::STaskDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sTask.FromUserGUID,sTask.FromUserGUID.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sTask.ForUserGUID,sTask.ForUserGUID.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sTask.ProjectGUID,sTask.ProjectGUID.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sTask.Task,sTask.Task.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sTask.TaskGUID,sTask.TaskGUID.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sTask.Answer,sTask.Answer.GetLength(),cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//передать клиенту данные задания в виде полного пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::SendTaskDataToClientInPackage(SClient &sClient,const STask &sTask,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{ 
 on_exit=false;

 if (sClient.UserGUID.GetLength()==0) return;//если клиент не авторизован, то ему ничего не отправляем  
 if (!(sClient.UserGUID.Compare(sTask.ForUserGUID)==0 || sClient.UserGUID.Compare(sTask.FromUserGUID)==0)) return;

 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=answer;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendTaskDataToClient(sClient,sTask,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//получение задания из массива принятых данных
//----------------------------------------------------------------------------------------------------
bool CTransceiver_Task::ReadSTaskInArray(char *ptr,size_t &offset,size_t max_length,STask &sTask)
{
 if (offset+sizeof(SServerCommand::STaskDataHeader)>max_length) return(false);
 SServerCommand::STaskDataHeader *sServerCommand_sTaskDataHeader_Ptr=reinterpret_cast<SServerCommand::STaskDataHeader*>(ptr+offset);
 offset+=sizeof(SServerCommand::STaskDataHeader);
 long length=offset;
 length+=sServerCommand_sTaskDataHeader_Ptr->ForUserGUIDSize;
 length+=sServerCommand_sTaskDataHeader_Ptr->FromUserGUIDSize;
 length+=sServerCommand_sTaskDataHeader_Ptr->ProjectGUIDSize;
 length+=sServerCommand_sTaskDataHeader_Ptr->TaskGUIDSize;
 length+=sServerCommand_sTaskDataHeader_Ptr->TaskSize;
 length+=sServerCommand_sTaskDataHeader_Ptr->AnswerSize;
 if (length>max_length) return(false);

 sTask.Year=sServerCommand_sTaskDataHeader_Ptr->Year;
 sTask.Month=sServerCommand_sTaskDataHeader_Ptr->Month;
 sTask.Day=sServerCommand_sTaskDataHeader_Ptr->Day;
 sTask.State=sServerCommand_sTaskDataHeader_Ptr->State;
 sTask.Index=sServerCommand_sTaskDataHeader_Ptr->Index;

 SetString(sTask.FromUserGUID,ptr+offset,sServerCommand_sTaskDataHeader_Ptr->FromUserGUIDSize);
 offset+=sServerCommand_sTaskDataHeader_Ptr->FromUserGUIDSize;

 SetString(sTask.ForUserGUID,ptr+offset,sServerCommand_sTaskDataHeader_Ptr->ForUserGUIDSize);
 offset+=sServerCommand_sTaskDataHeader_Ptr->ForUserGUIDSize;

 SetString(sTask.ProjectGUID,ptr+offset,sServerCommand_sTaskDataHeader_Ptr->ProjectGUIDSize);
 offset+=sServerCommand_sTaskDataHeader_Ptr->ProjectGUIDSize;

 SetString(sTask.Task,ptr+offset,sServerCommand_sTaskDataHeader_Ptr->TaskSize);
 offset+=sServerCommand_sTaskDataHeader_Ptr->TaskSize;

 SetString(sTask.TaskGUID,ptr+offset,sServerCommand_sTaskDataHeader_Ptr->TaskGUIDSize);
 offset+=sServerCommand_sTaskDataHeader_Ptr->TaskGUIDSize;

 SetString(sTask.Answer,ptr+offset,sServerCommand_sTaskDataHeader_Ptr->AnswerSize);
 offset+=sServerCommand_sTaskDataHeader_Ptr->AnswerSize;
 return(true); 
}
//----------------------------------------------------------------------------------------------------
//передача базы данных заданий
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::SendTaskBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 //отвечаем
 //передаём список базы данных
 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_TASK_BOOK;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 list<STask> list_STask=cDocument_Main_Ptr->GetAllTaskForUserGUID(sClient.UserGUID);
 list<STask>::iterator iterator=list_STask.begin();
 list<STask>::iterator iterator_end=list_STask.end();  
 while(iterator!=iterator_end)
 {
  STask &sTask=*iterator;  
  /*
  if (sTask.State==TASK_STATE_DONE || sTask.State==TASK_STATE_CANCELED || sTask.State==TASK_STATE_FINISHED)
  {
   if (sTask.FromUserGUID.Compare(sClient.UserGUID)!=0 && sTask.ForUserGUID.Compare(sClient.UserGUID)==0)//для данного клиента это задание уже завершено и не должно передаваться
   {
    iterator++;
	continue;
   }
  }
  */
  SendTaskDataToClient(sClient,sTask,cEvent_Exit,on_exit);
  if (on_exit==true) return;
  iterator++;
 }
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//добавление нового задания
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::AddTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //выделяем параметры задания
 STask sTask;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadSTaskInArray(ptr,offset,sClient.vector_Data.size(),sTask);
 //указываем, от кого было задание
 sTask.FromUserGUID=sClient.UserGUID;
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->AddTask(sTask)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//удаление задания
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::DeleteTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //выделяем параметры задания
 STask sTask;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadSTaskInArray(ptr,offset,sClient.vector_Data.size(),sTask);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->DeleteTask(sTask)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//изменение задания
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::ChangeTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //выделяем параметры задания
 STask sTask;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadSTaskInArray(ptr,offset,sClient.vector_Data.size(),sTask);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->ChangeTask(sTask)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}