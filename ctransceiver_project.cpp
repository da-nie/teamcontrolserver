#include "ctransceiver_project.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_Project::CTransceiver_Project(void)
{
 Version=1;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver_Project::~CTransceiver_Project()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//передать клиенту данные проекта
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::SendProjectDataToClient(SClient &sClient,const CProject &cProject,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SServerAnswer::CProjectDataHeader sServerAnswer_cProjectDataHeader;

 sServerAnswer_cProjectDataHeader.Signature[0]='P';
 sServerAnswer_cProjectDataHeader.Signature[1]='L';
 sServerAnswer_cProjectDataHeader.Signature[2]='V';
 sServerAnswer_cProjectDataHeader.Version=Version;

 sServerAnswer_cProjectDataHeader.ProjectNameSize=cProject.GetProjectName().GetLength();
 sServerAnswer_cProjectDataHeader.ProjectGUIDSize=cProject.GetProjectGUID().GetLength();
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_cProjectDataHeader),sizeof(SServerAnswer::CProjectDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cProject.GetProjectName(),cProject.GetProjectName().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cProject.GetProjectGUID(),cProject.GetProjectGUID().GetLength(),cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//передать клиенту данные проекта в виде полного пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::SendProjectDataToClientInPackage(SClient &sClient,const CProject &cProject,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{ 
 on_exit=false;

 if (sClient.UserGUID.GetLength()==0) return;//если клиент не авторизован, то ему ничего не отправляем  

 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=answer;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendProjectDataToClient(sClient,cProject,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//получение проекта из массива принятых данных
//----------------------------------------------------------------------------------------------------
bool CTransceiver_Project::ReadCProjectInArray(char *ptr,size_t &offset,size_t max_length,CProject &cProject)
{
 if (offset+sizeof(SServerCommand::CProjectDataHeader)>max_length) return(false);
 SServerCommand::CProjectDataHeader *sServerCommand_cProjectDataHeader_Ptr=reinterpret_cast<SServerCommand::CProjectDataHeader*>(ptr+offset);
 offset+=sizeof(SServerCommand::CProjectDataHeader);

 if (sServerCommand_cProjectDataHeader_Ptr->Signature[0]!='P' || sServerCommand_cProjectDataHeader_Ptr->Signature[1]!='L' || sServerCommand_cProjectDataHeader_Ptr->Signature[2]!='V') return(false);
 if (sServerCommand_cProjectDataHeader_Ptr->Version!=Version) return(false);

 long length=offset;
 length+=sServerCommand_cProjectDataHeader_Ptr->ProjectNameSize;
 length+=sServerCommand_cProjectDataHeader_Ptr->ProjectGUIDSize;
 if (length>max_length) return(false);

 CSafeString str;
 SetString(str,ptr+offset,sServerCommand_cProjectDataHeader_Ptr->ProjectNameSize);
 cProject.SetProjectName(str);
 offset+=sServerCommand_cProjectDataHeader_Ptr->ProjectNameSize;

 SetString(str,ptr+offset,sServerCommand_cProjectDataHeader_Ptr->ProjectGUIDSize);
 cProject.SetProjectGUID(str);
 offset+=sServerCommand_cProjectDataHeader_Ptr->ProjectGUIDSize;
 return(true); 
}
//----------------------------------------------------------------------------------------------------
//передача базы данных проектов
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::SendProjectBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 list<CProject> list_CProject=cDocument_Main_Ptr->GetAllProject();
//отвечаем
 //передаём список базы данных
 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_PROJECT_BOOK;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 list<CProject>::iterator iterator=list_CProject.begin();
 list<CProject>::iterator iterator_end=list_CProject.end();  
 while(iterator!=iterator_end)
 {
  CProject &cProject=*iterator;  
  SendProjectDataToClient(sClient,cProject,cEvent_Exit,on_exit);
  if (on_exit==true) return;
  iterator++;
 }
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//добавление нового проекта
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::AddProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //выделяем параметры проекта
 CProject cProject;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadCProjectInArray(ptr,offset,sClient.vector_Data.size(),cProject);
 CUser cUser;
 if (cDocument_Main_Ptr->FindUserByGUID(sClient.UserGUID,cUser)==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 if (cUser.GetLeader()==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->AddProject(cProject)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//удаление проекта
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::DeleteProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //выделяем параметры проекта
 CProject cProject;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadCProjectInArray(ptr,offset,sClient.vector_Data.size(),cProject);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->DeleteProject(cProject)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//изменение проекта
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::ChangeProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//если документа нет или клиент не авторизован
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //выделяем параметры проекта
 CProject cProject;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadCProjectInArray(ptr,offset,sClient.vector_Data.size(),cProject);
 //выполняем действия с заданием
 if (cDocument_Main_Ptr->ChangeProject(cProject)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}
