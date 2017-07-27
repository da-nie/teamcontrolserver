#include "ctransceiver_autorization.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_Autorization::CTransceiver_Autorization(void)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver_Autorization::~CTransceiver_Autorization()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//обработка команды авторизации
//----------------------------------------------------------------------------------------------------
void CTransceiver_Autorization::ExecuteAutorization(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 long length=sizeof(SServerCommand::SHeader);
 length+=sizeof(SServerCommand::SAutorizationDataHeader); 
 if (sClient.vector_Data.size()<length)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 ptr+=sizeof(SServerCommand::SHeader);
 SServerCommand::SAutorizationDataHeader *sServerCommand_sAutorizationDataHeader_Ptr=reinterpret_cast<SServerCommand::SAutorizationDataHeader*>(ptr);
 ptr+=sizeof(SServerCommand::SAutorizationDataHeader);

 //выделяем логин и пароль
 sClient.Login="";
 sClient.Password=""; 
 length+=sServerCommand_sAutorizationDataHeader_Ptr->PasswordSize;
 length+=sServerCommand_sAutorizationDataHeader_Ptr->LoginSize;
 if (length>sClient.vector_Data.size())
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 SetString(sClient.Login,ptr,sServerCommand_sAutorizationDataHeader_Ptr->LoginSize);
 ptr+=sServerCommand_sAutorizationDataHeader_Ptr->LoginSize;
 SetString(sClient.Password,ptr,sServerCommand_sAutorizationDataHeader_Ptr->PasswordSize);
 //авторизуем пользователя
 SUser sUser;
 if (cDocument_Main_Ptr->FindUserByLoginAndPassword(sClient.Login,sClient.Password,sUser)==true)
 {
  sClient.UserGUID=sUser.UserGUID;  
  SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
  if (on_exit==true) return;
  SServerAnswer::SHeader sServerAnswer_sHeader;
  sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_AUTORIZATION_OK;
  sServerAnswer_sHeader.CommandID=command;
  SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
  if (on_exit==true) return;
  SServerAnswer::SAutorizationDataHeader sServerAnswer_sAutorizationDataHeader;
  sServerAnswer_sAutorizationDataHeader.UserGUIDSize=sUser.UserGUID.GetLength();
  SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sAutorizationDataHeader),sizeof(SServerAnswer::SAutorizationDataHeader),cEvent_Exit,on_exit);
  if (on_exit==true) return;
  SendPart(sClient.Socket,sUser.UserGUID,sUser.UserGUID.GetLength(),cEvent_Exit,on_exit);
  if (on_exit==true) return;
  SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
  return;
 } 
 //отвечаем
 SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
}
