#include "ctransceiver_user.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_User::CTransceiver_User(void)
{
 Version=1;
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
void CTransceiver_User::SendUserDataToClient(const SClient &sClient,const CUser &cUser,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SServerAnswer::CUserDataHeader sServerAnswer_cUserDataHeader;
 sServerAnswer_cUserDataHeader.Signature[0]='U';
 sServerAnswer_cUserDataHeader.Signature[1]='L';
 sServerAnswer_cUserDataHeader.Signature[2]='V';
 sServerAnswer_cUserDataHeader.Version=Version;

 sServerAnswer_cUserDataHeader.NameSize=cUser.GetName().GetLength();
 sServerAnswer_cUserDataHeader.JobTitleSize=cUser.GetJobTitle().GetLength();
 sServerAnswer_cUserDataHeader.TelephoneSize=cUser.GetTelephone().GetLength();
 sServerAnswer_cUserDataHeader.DescriptionSize=cUser.GetDescription().GetLength();
 sServerAnswer_cUserDataHeader.GUIDSize=cUser.GetUserGUID().GetLength();
 sServerAnswer_cUserDataHeader.Leader=cUser.GetLeader();
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_cUserDataHeader),sizeof(SServerAnswer::CUserDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cUser.GetName(),cUser.GetName().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cUser.GetJobTitle(),cUser.GetJobTitle().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cUser.GetTelephone(),cUser.GetTelephone().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cUser.GetDescription(),cUser.GetDescription().GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,cUser.GetUserGUID(),cUser.GetUserGUID().GetLength(),cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//передать клиенту данные пользователя в виде полного пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver_User::SendUserDataToClientInPackage(const SClient &sClient,const CUser &cUser,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
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
 SendUserDataToClient(sClient,cUser,cEvent_Exit,on_exit);
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

 std::list<CUser> list_CUser=cDocument_Main_Ptr->GetAllUser();
 std::list<CUser>::iterator iterator=list_CUser.begin();
 std::list<CUser>::iterator iterator_end=list_CUser.end();  
 while(iterator!=iterator_end)
 {
  CUser &cUser=*iterator;
  SendUserDataToClient(sClient,cUser,cEvent_Exit,on_exit);
  if (on_exit==true) return;
  iterator++;
 }
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
