/*-----------------------------------???????QT???????û???½???????????--------------------------------------------------------------*/
/*-----------------------------------??????????--------------------------------------------------------------------------------------------*/
#include "ui_MainWindow.h"
#include <QMainWindow>
#include "qfile.h"
#include "qtranslator.h"
#include <QMovie>
#include <qdebug.h>
#include <qthread.h>
#include <QStandardItemModel>

#include "mysql++/mysql++.h"

#include "User.h"

#pragma once
namespace Ui
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = NULL);
	~MainWindow();

private:
	Ui::MainWindow *ui;
    mysql::user *myuser_;
    std::vector<mysql::Mail> emails_;
    QStandardItemModel* item_model_;
    std::string user_;
    std::string password_;
    int index_;

private slots:
    /* from Main */
	void MainClickedSend();
    void MainClickedLogout();
    void MainClickedInbox();
    void MainClickedRoot(){};
    void MainClickedIndex(QModelIndex index);

    /* from Send */
    void SendClickedBack();

    /* from Log */
    void LogClickedLog();

    /* from Check */
    void CheckClickedDelete();

signals:
	void Main2Send();
	void Main2Log();
    void Main2Check();
	void Main2Receive();
};


