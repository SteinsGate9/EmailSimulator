/*-----------------------------------��ģ����QTģ�飬ʵ���û���������------------------------------------------------------------------------*/
/*-----------------------------------���ߣ���ʿ��--------------------------------------------------------------------------------------------*/
#pragma once

#include <QWidget>
#include <QMessageBox>
#include <iostream>

#include "Admin.h"

namespace Ui { class CreateUserWindow; };

class CreateUserWindow : public QWidget
{
	Q_OBJECT

public:
	CreateUserWindow(QWidget *parent = Q_NULLPTR);
	~CreateUserWindow();

private:
	Ui::CreateUserWindow *ui;
	std::string temp_user_;
	std::string temp_password_;
	mysql::Admin a_;


private slots:
    /* from create */
	void CreateClickedCreate();
    void CreateClickedBack();
	void CreateInputUser(const QString&);
	void CreateInputPassword(const QString&);

    /* from log */
	void LogClickedCreate();

signals:
	void Create2Log();
};
