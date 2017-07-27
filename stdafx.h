#ifndef STDAFX_H
#define STDAFX_H

//для работы с Direct Show
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

#define ID_TIMER_FRAMEWND_MAIN 1000

#define WM_SYSTEM_TRAY_ICON WM_USER+1

#endif