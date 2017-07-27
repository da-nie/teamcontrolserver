#include "ctransceiver_project.h"

//====================================================================================================
//����������������
//====================================================================================================

//====================================================================================================
//����������� ������
//====================================================================================================
CTransceiver_Project::CTransceiver_Project(void)
{
}
//====================================================================================================
//���������� ������
//====================================================================================================
CTransceiver_Project::~CTransceiver_Project()
{ 
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ������� ������ �������
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::SendProjectDataToClient(SClient &sClient,const SProject &sProject,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 SServerAnswer::SProjectDataHeader sServerAnswer_sProjectDataHeader;
 sServerAnswer_sProjectDataHeader.ProjectNameSize=sProject.ProjectName.GetLength();
 sServerAnswer_sProjectDataHeader.ProjectGUIDSize=sProject.ProjectGUID.GetLength();
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sProjectDataHeader),sizeof(SServerAnswer::SProjectDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sProject.ProjectName,sProject.ProjectName.GetLength(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendPart(sClient.Socket,sProject.ProjectGUID,sProject.ProjectGUID.GetLength(),cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//�������� ������� ������ ������� � ���� ������� ������
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::SendProjectDataToClientInPackage(SClient &sClient,const SProject &sProject,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{ 
 on_exit=false;

 if (sClient.UserGUID.GetLength()==0) return;//���� ������ �� �����������, �� ��� ������ �� ����������  

 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=answer;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendProjectDataToClient(sClient,sProject,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//��������� ������� �� ������� �������� ������
//----------------------------------------------------------------------------------------------------
bool CTransceiver_Project::ReadSProjectInArray(char *ptr,size_t &offset,size_t max_length,SProject &sProject)
{
 if (offset+sizeof(SServerCommand::SProjectDataHeader)>max_length) return(false);
 SServerCommand::SProjectDataHeader *sServerCommand_sProjectDataHeader_Ptr=reinterpret_cast<SServerCommand::SProjectDataHeader*>(ptr+offset);
 offset+=sizeof(SServerCommand::SProjectDataHeader);
 long length=offset;
 length+=sServerCommand_sProjectDataHeader_Ptr->ProjectNameSize;
 length+=sServerCommand_sProjectDataHeader_Ptr->ProjectGUIDSize;
 if (length>max_length) return(false);

 SetString(sProject.ProjectName,ptr+offset,sServerCommand_sProjectDataHeader_Ptr->ProjectNameSize);
 offset+=sServerCommand_sProjectDataHeader_Ptr->ProjectNameSize;

 SetString(sProject.ProjectGUID,ptr+offset,sServerCommand_sProjectDataHeader_Ptr->ProjectGUIDSize);
 offset+=sServerCommand_sProjectDataHeader_Ptr->ProjectGUIDSize;
 return(true); 
}
//----------------------------------------------------------------------------------------------------
//�������� ���� ������ ��������
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::SendProjectBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//���� ��������� ��� ��� ������ �� �����������
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 //��������
 //������� ������ ���� ������
 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_PROJECT_BOOK;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 list<SProject> list_SProject=cDocument_Main_Ptr->GetAllProject();
 list<SProject>::iterator iterator=list_SProject.begin();
 list<SProject>::iterator iterator_end=list_SProject.end();  
 while(iterator!=iterator_end)
 {
  SProject &sProject=*iterator;  
  SendProjectDataToClient(sClient,sProject,cEvent_Exit,on_exit);
  if (on_exit==true) return;
  iterator++;
 }
 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//���������� ������ �������
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::AddProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//���� ��������� ��� ��� ������ �� �����������
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //�������� ��������� �������
 SProject sProject;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadSProjectInArray(ptr,offset,sClient.vector_Data.size(),sProject);
 SUser sUser;
 if (cDocument_Main_Ptr->FindUserByGUID(sClient.UserGUID,sUser)==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 if (sUser.Leader==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 //��������� �������� � ��������
 if (cDocument_Main_Ptr->AddProject(sProject)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //��������
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::DeleteProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//���� ��������� ��� ��� ������ �� �����������
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //�������� ��������� �������
 SProject sProject;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadSProjectInArray(ptr,offset,sClient.vector_Data.size(),sProject);
 //��������� �������� � ��������
 if (cDocument_Main_Ptr->DeleteProject(sProject)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //��������
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
void CTransceiver_Project::ChangeProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 if (cDocument_Main_Ptr==NULL || sClient.UserGUID.GetLength()==0)//���� ��������� ��� ��� ������ �� �����������
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }
 char *ptr=reinterpret_cast<char*>(&sClient.vector_Data[0]);
 //�������� ��������� �������
 SProject sProject;
 size_t offset=sizeof(SServerCommand::SHeader);
 ReadSProjectInArray(ptr,offset,sClient.vector_Data.size(),sProject);
 //��������� �������� � ��������
 if (cDocument_Main_Ptr->ChangeProject(sProject)==false) SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
 //��������
 SendAnswer(sClient.Socket,SERVER_ANSWER_OK,command,NULL,0,cEvent_Exit,on_exit);
}