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
 //�����������
 SERVER_COMMAND_AUTORIZATION=1,
 //��������� ���� �������������
 SERVER_COMMAND_GET_USER_BOOK=2,
 //��������� ���� �������
 SERVER_COMMAND_GET_TASK_BOOK=3,
 //��������� ���� ��������
 SERVER_COMMAND_GET_PROJECT_BOOK=4,
 //����� �������
 SERVER_COMMAND_ADDED_TASK=5,
 //������� �������
 SERVER_COMMAND_DELETED_TASK=6,
 //������� ��������
 SERVER_COMMAND_CHANGED_TASK=7,
 //����� ������
 SERVER_COMMAND_ADDED_PROJECT=8,
 //������ ������
 SERVER_COMMAND_DELETED_PROJECT=9,
 //������ �������
 SERVER_COMMAND_CHANGED_PROJECT=10
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

 //��������� ���� �������������
 SERVER_ANSWER_USER_BOOK=3,
 //��������� �������� ������������
 SERVER_ANSWER_DELETED_USER=4,
 //��������� ����������� ������������
 SERVER_ANSWER_ADDED_USER=5,
 //��������� ��������� ������������
 SERVER_ANSWER_CHANGED_USER=6,

 //��������� ���� ������� ������������
 SERVER_ANSWER_TASK_BOOK=7,
 //��������� �������� �������
 SERVER_ANSWER_DELETED_TASK=8,
 //��������� ����������� �������
 SERVER_ANSWER_ADDED_TASK=9,
 //��������� ��������� �������
 SERVER_ANSWER_CHANGED_TASK=10,

 //��������� ���� ��������
 SERVER_ANSWER_PROJECT_BOOK=11,
 //��������� �������� ������
 SERVER_ANSWER_DELETED_PROJECT=12,
 //��������� ����������� ������
 SERVER_ANSWER_ADDED_PROJECT=13,
 //��������� ��������� ������
 SERVER_ANSWER_CHANGED_PROJECT=14

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
 //�����������
 struct SAutorizationDataHeader
 { 
  unsigned long LoginSize;//������ ������
  unsigned long PasswordSize;//������ ������
 };
 //����������� ��������� ������ ��� �������� �������
 struct STaskDataHeader
 {
  long Index;//������ �������
  unsigned long FromUserGUIDSize;//�� ������������
  unsigned long ForUserGUIDSize;//��� ������������
  unsigned long ProjectGUIDSize;//�� �������
  unsigned long TaskSize;//�������
  unsigned long TaskGUIDSize;//������������� �������
  long State;//���������
  long Year;//���
  long Month;//�����
  long Day;//�����   
 };
 //����������� ��������� ������ ��� �������� �������
 struct SProjectDataHeader
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
 //����������� ��������� ������ ��� �����������
 struct SAutorizationDataHeader
 {
  unsigned long UserGUIDSize;//������ GUID
 };
 //����������� ��������� ������ ��� �������� ���������� ������������
 struct SUserDataHeader
 {
  unsigned long NameSize;//������ �����
  unsigned long JobTitleSize;//������ ���������
  unsigned long TelephoneSize;//������ ���������� ������
  unsigned long DescriptionSize;//������ ��������
  unsigned long GUIDSize;//������ GUID
  bool Leader;//�������� �� �����������
 };
 //����������� ��������� ������ ��� �������� ���������� �������
 struct STaskDataHeader
 {
  long Index;//������ �������
  unsigned long FromUserGUIDSize;//�� ������������
  unsigned long ForUserGUIDSize;//��� ������������
  unsigned long ProjectGUIDSize;//�� �������
  unsigned long TaskSize;//�������
  unsigned long TaskGUIDSize;//������������� �������
  long State;//���������
  long Year;//���
  long Month;//�����
  long Day;//����� 
 };
 //����������� ��������� ������ ��� �������� ���������� �������
 struct SProjectDataHeader
 {
  unsigned long ProjectNameSize;//������
  unsigned long ProjectGUIDSize;//������������� �������
 };
};
#pragma pack()

#endif