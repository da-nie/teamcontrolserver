#include "ctransceiver_user.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_User::CTransceiver_User(void)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver_User::~CTransceiver_User()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//передать клиенту данные пользователя
//----------------------------------------------------------------------------------------------------
void CTransceiver_User::SendUserDataToClient(const SClient &sClient,const SUser &sUser,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SServerAnswer::SUserDataHeader sServerAnswer_sUserDataHeader;
 sServerAnswer_sUserDataHeader.NameSize=sUser.Name.GetLength();
 sServerAnswer_sUserDataHeader.JobTitleSize=sUser.JobTitle.GetLength();
 sServerAnswer_sUserDataHeader.TelephoneSize=sUser.Telephone.GetLength();
 sServerAnswer_sUserDataHeader.DescriptionSize=sUser.Description.GetLength();
 sServerAnswer_sUserDataHeader.GUIDSize=sUser.UserGUID.GetLength();
 sServerAnswer_sUserDataHeader.Leader=sUser.Leader;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sUserDataHeader),sizeof(SServerAnswer::SUserDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sUser.Name,sUser.Name.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sUser.JobTitle,sUser.JobTitle.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sUser.Telephone,sUser.Telephone.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sUser.Description,sUser.Description.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sUser.UserGUID,sUser.UserGUID.GetLength(),cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//передать клиенту данные пользователя в виде полного пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver_User::SendUserDataToClientInPackage(const SClient &sClient,const SUser &sUser,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
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
 SendUserDataToClient(sClient,sUser,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//передача базы данных пользователей
//----------------------------------------------------------------------------------------------------
void CTransceiver_User::SendUserBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
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
 sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_USER_BOOK;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 list<SUser> list_SUser=cDocument_Main_Ptr->GetAllUser();
 list<SUser>::iterator iterator=list_SUser.begin();
 list<SUser>::iterator iterator_end=list_SUser.end();  
 while(iterator!=iterator_end)
 {
  SUser &sUser=*iterator;
  SendUserDataToClient(sClient,sUser,cEvent_Exit,on_exit);
  if (on_exit==true) return;
  iterator++;
 }
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
