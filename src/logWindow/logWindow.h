/*-----------------------------------��ģ����QTģ�飬ʵ���û���½����------------------------------------------------------------------------*/
/*-----------------------------------���ߣ���ʿ��--------------------------------------------------------------------------------------------*/
#pragma once

#include "ui_logWindow.h"
#include <QWidget>
#include <QProgressDialog>
#include <QMessageBox>
#include <qthread.h>
#include <qdebug.h>

#include <mysql/mysql.h>

#include "head.h"
#include "User.h"
#include "SqlConnectionPool.h"
#include "logWindow.h"

namespace Ui { class logWindow; }

class logWindow : public QWidget{
	Q_OBJECT
public:
    explicit logWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::logWindow *ui;
	std::string user_;
	std::string password_;

private slots:
    /* from Log */
	void LogClickedLog();
    void LogClickedCreate();
    void LogInputUser(const QString& x);
    void LogInputPassword(const QString& x);

    /* from Main */
	void MainClickedLogout();

    /* from Create */
    void CreateClickedBack();

signals:
    void Log2Main();
	void Log2Create();

};


