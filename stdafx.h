#ifndef STDAFX_H
#define STDAFX_H

//��� ������ � Direct Show
#define _WIN32_WINNT 0x0500

#define VC_EXTRALEAN

#include <afxwin.h>
#include <afxdisp.h>
#include <afxext.h>
#include <afxcview.h>
#include <afxcmn.h>
#include <afxmt.h>
#include <afxdb.h>
#include <math.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "resource.h"
#include "csafestring.h"

//������������� ������� �������� ����
#define ID_TIMER_FRAMEWND_MAIN 1000
//������ ������� �������� ����
#define FRAME_WND_TIMER_PERIOD 100
//������������ �������� �������� ������� �������� �����
#define PING_COUNTER_MAX_VALUE (60000/FRAME_WND_TIMER_PERIOD)
//������������ �������� �������� ������� ���������� ����������� ��� ������
#define BACKUP_COUNTER_MAX_VALUE (4*60*60000/FRAME_WND_TIMER_PERIOD)

#define WM_SYSTEM_TRAY_ICON WM_USER+1

#define CLIENT_PROGRAMM_FILE_NAME "TeamControlClient.exe"
#define LOADER_FILE_NAME "TeamControlLoader.exe"

#endif