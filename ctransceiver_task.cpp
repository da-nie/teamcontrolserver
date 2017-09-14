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
void CTransceiver_Task::SendTaskDataToClient(SClient &sClient,const CTask &cTask,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SServerAnswer::CTaskDataHeader sServerAnswer_cTaskDataHeader;
 sServerAnswer_cTaskDataHeader.FromUserGUIDSize=cTask.GetFromUserGUID().GetLength();
 sServerAnswer_cTaskDataHeader.ForUserGUIDSize=cTask.GetForUserGUID().GetLength();
 sServerAnswer_cTaskDataHeader.ProjectGUIDSize=cTask.GetProjectGUID().GetLength();
 sServerAnswer_cTaskDataHeader.TaskSize=cTask.GetTask().GetLength();
 sServerAnswer_cTaskDataHeader.TaskGUIDSize=cTask.GetTaskGUID().GetLength();
 sServerAnswer_cTaskDataHeader.AnswerSize=cTask.GetAnswer().GetLength();
 const CDate &cDate=cTask.GetDate();
 sServerAnswer_cTaskDataHeader.Year=cDate.GetYear();
 sServerAnswer_cTaskDataHeader.Month=cDate.GetMonth();
 sServerAnswer_cTaskDataHeader.Day=cDate.GetDay();
 sServerAnswer_cTaskDataHeader.State=cTask.GetState();
 sServerAnswer_cTaskDataHeader.Index=cTask.GetIndex();
 sServerAnswer_cTaskDataHeader.AnswerNotRead=cTask.GetAnswerNotRead();
 sServerAnswer_cTaskDataHeader.PlannedPosition=cTask.GetPlannedPosition();
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_cTaskDataHeader),sizeof(SServerAnswer::CTaskDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cTask.GetFromUserGUID(),cTask.GetFromUserGUID().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cTask.GetForUserGUID(),cTask.GetForUserGUID().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cTask.GetProjectGUID(),cTask.GetProjectGUID().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cTask.GetTask(),cTask.GetTask().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cTask.GetTaskGUID(),cTask.GetTaskGUID().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cTask.GetAnswer(),cTask.GetAnswer().GetLength(),cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//передать клиенту данные задания в виде полного пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver_Task::SendTaskDataToClientInPackage(SClient &sClient,const CTask &cTask,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{ 
 on_exit=false;

 if (sClient.UserGUID.GetLength()==0) return;//если клиент не авторизован, то ему ничего не отправляем   
 if (cTask.IsFromOrForUserGUID(sClient.UserGUID)==false) return;

 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=answer;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendTaskDataToClient(sClient,cTask,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//получение задания из массива принятых данных
//----------------------------------------------------------------------------------------------------
bool CTransceiver_Task::ReadCTaskInArray(char *ptr,size_t &offset,size_t max_length,CTask &cTask)
{
 if (offset+sizeof(SServerCommand::CTaskDataHeader)>max_length) return(false);
 SServerCommand::CTaskDataHeader *sServerCommand_cTaskDataHeader_Ptr=reinterpret_cast<SServerCommand::CTaskDataHeader*>(ptr+offset);
 offset+=sizeof(SServerCommand::CTaskDataHeader);
 long length=offset;
 length+=sServerCommand_cTaskDataHeader_Ptr->ForUserGUIDSize;
 length+=sServerCommand_cTaskDataHeader_Ptr->FromUserGUIDSize;
 length+=sServerCommand_cTaskDataHeader_Ptr->ProjectGUIDSize;
 length+=sServerCommand_cTaskDataHeader_Ptr->TaskGUIDSize;
 length+=sServerCommand_cTaskDataHeader_Ptr->TaskSize;
 length+=sServerCommand_cTaskDataHeader_Ptr->AnswerSize;
 if (length>max_length) return(false);

 cTask.SetDate(CDate(sServerCommand_cTaskDataHeader_Ptr->Year,sServerCommand_cTaskDataHeader_Ptr->Month,sServerCommand_cTaskDataHeader_Ptr->Day));
 cTask.SetAnswerNotRead(sServerCommand_cTaskDataHeader_Ptr->AnswerNotRead);
 cTask.SetPlannedPosition(sServerCommand_cTaskDataHeader_Ptr->PlannedPosition);
 cTask.SetState(sServerCommand_cTaskDataHeader_Ptr->State);
 cTask.SetIndex(sServerCommand_cTaskDataHeader_Ptr->Index);

 CSafeString str;
 SetString(str,ptr+offset,sServerCommand_cTaskDataHeader_Ptr->FromUserGUIDSize);
 cTask.SetFromUserGUID(str);
 offset+=sServerCommand_cTaskDataHeader_Ptr->FromUserGUIDSize;

 SetString(str,ptr+offset,sServerCommand_cTaskDataHeader_Ptr->ForUserGUIDSize);
 cTask.SetForUserGUID(str);
 offset+=sServerCommand_cTaskDataHeader_Ptr->ForUserGUIDSize;

 SetString(str,ptr+offset,sServerCommand_cTaskDataHeader_Ptr->ProjectGUIDSize);
 cTask.SetProjectGUID(str);
 offset+=sServerCommand_cTaskDataHeader_Ptr->ProjectGUIDSize;

 SetString(str,ptr+offset,sServerCommand_cTaskDataHeader_Ptr->TaskSize);
 cTask.SetTask(str);
 offset+=sServerCommand_cTaskDataHeader_Ptr->TaskSize;

 SetString(str,ptr+offset,sServerCommand_cTaskDataHeader_Ptr->TaskGUIDSize);
 cTask.SetTaskGUID(str);
 offset+=sServerCommand_cTaskDataHeader_Ptr->TaskGUIDSize;

 SetString(str,ptr+offset,sServerCommand_cTaskDataHeader_Ptr->AnswerSize);
 cTask.SetAnswer(str);
 offset+=sServerCommand_cTaskDataHeader_Ptr->AnswerSize;
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

 list<CTask> list_CTask=cDocument_Main_Ptr->GetAllTaskForUserGUID(sClient.UserGUID);
 list<CTask>::iterator iterator=list_CTask.begin();
 list<CTask>::iterator iterator_end=list_CTask.end();  
 while(iterator!=iterator_end)
 {
  CTask &cTask=*iterator;  
  /*
  if (cTask.State==TASK_STATE_DONE || cTask.State==TASK_STATE_CANCELED || cTask.State==TASK_STATE_FINISHED)
  {
   if (cTask.FromUserGUID.Compare(sClient.UserGUID)!=0 && cTask.ForUserGUID.Compare(sClient.UserGUID)==0)//для данного клиента это задание уже завершено и не должно передаваться
   {
    iterator++;
	continue;
   }
  }
  */
  SendTaskDataToClient(sClient,cTask,cEvent_Exit,on_exit);
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
 CTask cTask;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadCTaskInArray(ptr,offset,sClient.vector_Data.size(),cTask);
 //указываем, от кого было задание
 cTask.SetFromUserGUID(sClient.UserGUID);
 //указываем, что задание не было прочитано
 cTask.SetState(TASK_STATE_NO_READ);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->AddTask(cTask)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
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
 CTask cTask;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadCTaskInArray(ptr,offset,sClient.vector_Data.size(),cTask);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->DeleteTask(cTask)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
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
 CTask cTask;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadCTaskInArray(ptr,offset,sClient.vector_Data.size(),cTask);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->ChangeTask(cTask)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}