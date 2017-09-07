#include "ctransceiver_ping.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_Ping::CTransceiver_Ping(void)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver_Ping::~CTransceiver_Ping()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//передать клиенту сообщение для проверки связи
//----------------------------------------------------------------------------------------------------
void CTransceiver_Ping::SendPingDataToClientInPackage(const SClient &sClient,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=answer;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SPingDataHeader sServerAnswer_sPingDataHeader;
 sServerAnswer_sPingDataHeader.PingData[0]='T';
 sServerAnswer_sPingDataHeader.PingData[1]='E';
 sServerAnswer_sPingDataHeader.PingData[2]='S';
 sServerAnswer_sPingDataHeader.PingData[3]='T';
 sServerAnswer_sPingDataHeader.PingData[4]=' ';
 sServerAnswer_sPingDataHeader.PingData[5]='T';
 sServerAnswer_sPingDataHeader.PingData[6]='E';
 sServerAnswer_sPingDataHeader.PingData[7]='S';
 sServerAnswer_sPingDataHeader.PingData[8]='T';
 sServerAnswer_sPingDataHeader.PingData[9]=0;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sPingDataHeader),sizeof(SServerAnswer::SPingDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
