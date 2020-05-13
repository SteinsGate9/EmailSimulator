/*-----------------------------------��ģ������ģ�飬ʵ������ģ��Ĵ���----------------------------------------------------------------------*/
/*-----------------------------------���ߣ���ʿ��--------------------------------------------------------------------------------------------*/
#pragma once
#if defined(MYSQLPP_MYSQL_HEADERS_BURIED)  
#   include <mysql/mysql_version.h>  
#else  
#   include "mysql/mysql_version.h"
#endif  

#pragma comment(lib,"mysqlpp.lib")

#include <iostream>  
#include <string>  
#include <cstdlib>  
#include <fstream>

/*usages*/

#include "User.h"
#include "Admin.h"
#include "Mail.h"

/*windows*/
#include "../mailLineWindow/OneMailLine.h"
#include "../receiveWindow/ReceiveWindowFinal.h"
#include "../CheckWindow/CheckWindow.h"
#include "../mainWindow/MainWindow.h"
#include "../sendWindow/sendWindow.h"
#include "../CreateUserWindow/CreateUserWindow.h"
#include "../logWindow/logWindow.h"

/*qt*/
#include <QApplication>
#include"qfile.h"
#include"qtranslator.h"

namespace Uni {
	extern sendWindow *e;
	extern MainWindow *w;
	extern CreateUserWindow *c;
	extern ReceiveWindowFinal *r;
}