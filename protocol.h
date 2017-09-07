#ifndef PROTOCOL_H
#define PROTOCOL_H

//====================================================================================================
//��������
//====================================================================================================

//��������� ��������� ������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"

//====================================================================================================
//����������������
//====================================================================================================

//������������ ������ ������ �������
#define MAX_PACKAGE_LENGTH 10000

//��������� �������������
//����� �������
#define PROTOCOL_END_PACKAGE   0x00
//������ �������
#define PROTOCOL_BEGIN_PACKAGE 0xFF
//��������
#define PROTOCOL_STUFFING      0xFE
//��� 0x00
#define PROTOCOL_STUFFING_00   0x01
//��� 0xFE
#define PROTOCOL_STUFFING_FE   0x02
//��� 0xFF
#define PROTOCOL_STUFFING_FF   0x03


//������� �������
enum SERVER_COMMAND
{
 //��� �������
 SERVER_COMMAND_NOTHING=0,
 //������ CRC ����� ���������� ���������
 SERVER_COMMAND_GET_CLIENT_PROGRAMM_CRC=1,
 //������ ���������� ��������� � ���������� ��� ����������
 SERVER_COMMAND_GET_CLIENT_PROGRAMM_AND_LOADER=2,
 //�����������
 SERVER_COMMAND_AUTORIZATION=3,
 //��������� ���� �������������
 SERVER_COMMAND_GET_USER_BOOK=4,
 //��������� ���� �������
 SERVER_COMMAND_GET_TASK_BOOK=5,
 //��������� ���� ��������
 SERVER_COMMAND_GET_PROJECT_BOOK=6,
 //����� �������
 SERVER_COMMAND_ADDED_TASK=7,
 //������� �������
 SERVER_COMMAND_DELETED_TASK=8,
 //������� ��������
 SERVER_COMMAND_CHANGED_TASK=9,
 //����� ������
 SERVER_COMMAND_ADDED_PROJECT=10,
 //������ ������
 SERVER_COMMAND_DELETED_PROJECT=11,
 //������ �������
 SERVER_COMMAND_CHANGED_PROJECT=12,

 //�������� �����
 SERVER_COMMAND_PING=13
};

//������ �������
enum SERVER_ANSWER
{
 //������� ��������� ������� � �� ������� ������
 SERVER_ANSWER_OK=0,
 //������� �� ��������� � �� ������� ������
 SERVER_ANSWER_ERROR=1,
 //������� ����������� ��������� ������� � �� ������� ������
 SERVER_ANSWER_AUTORIZATION_OK=2,
 //�������� CRC ����� ���������� ���������
 SERVER_ANSWER_CLIENT_PROGRAMM_CRC=3,
 //�������� ���������� ��������� ��� ����������
 SERVER_ANSWER_CLIENT_PROGRAMM_AND_LOADER=4,
 //��������� ���� �������������
 SERVER_ANSWER_USER_BOOK=5,
 //��������� �������� ������������
 SERVER_ANSWER_DELETED_USER=6,
 //��������� ����������� ������������
 SERVER_ANSWER_ADDED_USER=7,
 //��������� ��������� ������������
 SERVER_ANSWER_CHANGED_USER=8,

 //��������� ���� ������� ������������
 SERVER_ANSWER_TASK_BOOK=9,
 //��������� �������� �������
 SERVER_ANSWER_DELETED_TASK=10,
 //��������� ����������� �������
 SERVER_ANSWER_ADDED_TASK=11,
 //��������� ��������� �������
 SERVER_ANSWER_CHANGED_TASK=12,

 //��������� ���� ��������
 SERVER_ANSWER_PROJECT_BOOK=13,
 //��������� �������� ������
 SERVER_ANSWER_DELETED_PROJECT=14,
 //��������� ����������� ������
 SERVER_ANSWER_ADDED_PROJECT=15,
 //��������� ��������� ������
 SERVER_ANSWER_CHANGED_PROJECT=16,

 //�������� �����
 SERVER_ANSWER_PING=17

};

//������� �� ���������
#define TASK_STATE_NO_READ 0
//������� ��������� � ������� ����������
#define TASK_STATE_READED 1
//������� �����������
#define TASK_STATE_IS_RUNNING 2
//������� ���������
#define TASK_STATE_DONE 3
//������� ���������
#define TASK_STATE_CANCELED 4
//������� ���������
#define TASK_STATE_FINISHED 5

//====================================================================================================
//���������
//====================================================================================================

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma pack(1)
struct SServerCommand
{
 //���������
 struct SHeader
 {
  unsigned long CommandID;//������������� �������
 };
 //������� �������
 //����������� ��������� ������ ��� �������� �����
 struct SPingDataHeader
 {
  unsigned char PingData[10];//�������� ���������
 };
 //�����������
 struct SAutorizationDataHeader
 { 
  unsigned long LoginSize;//������ ������
  unsigned long PasswordSize;//������ ������
 };
 //����������� ��������� ������ ��� �������� �������
 struct CTaskDataHeader
 {
  long Index;//������ �������
  unsigned long FromUserGUIDSize;//�� ������������
  unsigned long ForUserGUIDSize;//��� ������������
  unsigned long ProjectGUIDSize;//�� �������
  unsigned long TaskSize;//�������
  unsigned long TaskGUIDSize;//������������� �������
  unsigned long AnswerSize;//�����
  long State;//���������
  long Year;//���
  long Month;//�����
  long Day;//�����   
 };
 //����������� ��������� ������ ��� �������� �������
 struct CProjectDataHeader
 {
  unsigned long ProjectNameSize;//������
  unsigned long ProjectGUIDSize;//������������� �������
 };
};
#pragma pack()


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� �������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma pack(1)
struct SServerAnswer
{
 //���������
 struct SHeader
 {
  unsigned long AnswerID;//������������� ������
  unsigned long CommandID;//������������� ������� �� ������� ������� �����
 };
 //�����
 //����������� ��������� ������ ��� �������� �����
 struct SPingDataHeader
 {
  unsigned char PingData[10];//�������� ���������
 };
 //����������� ��������� ������ ��� �����������
 struct SAutorizationDataHeader
 {
  unsigned long UserGUIDSize;//������ GUID
 };
 //����������� ��������� ������ ��� �������� CRC ����� ���������� ���������
 struct SClientProgrammCRCHeader
 {
  unsigned long CRC;//����������� �����
 };
 //����������� ��������� ������ ��� �������� ����� ���������� ��������� � ����������
 struct SClientProgrammAndLoaderDataHeader
 {
  unsigned long LoaderCRC;//����������� ����� ����������
  unsigned long ProgrammCRC;//����������� ����� ���������
  unsigned long LoaderSize;//������ ����������
  unsigned long ProgrammSize;//������ ���������
 };

 //����������� ��������� ������ ��� �������� ���������� ������������
 struct CUserDataHeader
 {
  unsigned long NameSize;//������ �����
  unsigned long JobTitleSize;//������ ���������
  unsigned long TelephoneSize;//������ ���������� ������
  unsigned long DescriptionSize;//������ ��������
  unsigned long GUIDSize;//������ GUID
  bool Leader;//�������� �� �����������
 };
 //����������� ��������� ������ ��� �������� ���������� �������
 struct CTaskDataHeader
 {
  long Index;//������ �������
  unsigned long FromUserGUIDSize;//�� ������������
  unsigned long ForUserGUIDSize;//��� ������������
  unsigned long ProjectGUIDSize;//�� �������
  unsigned long TaskSize;//�������
  unsigned long TaskGUIDSize;//������������� �������
  unsigned long AnswerSize;//�����
  long State;//���������
  long Year;//���
  long Month;//�����
  long Day;//����� 
 };
 //����������� ��������� ������ ��� �������� ���������� �������
 struct CProjectDataHeader
 {
  unsigned long ProjectNameSize;//������
  unsigned long ProjectGUIDSize;//������������� �������
 };
};
#pragma pack()

#endif