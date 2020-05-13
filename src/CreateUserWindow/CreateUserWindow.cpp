/*-----------------------------------��ģ����QTģ�飬ʵ���û���������------------------------------------------------------------------------*/
/*-----------------------------------���ߣ���ʿ��--------------------------------------------------------------------------------------------*/
#include "ui_CreateUserWindow.h"
#include "CreateUserWindow.h"

#include "head.h"
#include "Admin.h"
#include "User.h"

//#include <mysql/m>

CreateUserWindow::CreateUserWindow(QWidget *parent)
	: QWidget(parent), ui(new Ui::CreateUserWindow()), a_(){
	ui->setupUi(this);
    ui->PasswordInput->setEchoMode(QLineEdit::Password);

	connect(ui->UserInput,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(CreateInputUser(QString))
	);
	connect(ui->PasswordInput,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(CreateInputPassword(QString))
	);
}

CreateUserWindow::~CreateUserWindow(){
	delete ui;
}

void CreateUserWindow::CreateClickedCreate(){
    /* admin operations */
    try {
        a_.create_user(temp_user_, temp_password_);
    }
    catch (mysql::mysqlexecException& e) {
        char temp[20]; sprintf(temp, "Create User Failed with erron %d", e.index());
		QMessageBox::about(this, tr("Failed"), tr(temp));

		return;
	}
	QMessageBox::about(this, tr("Succeed"), tr("Create User Succeed"));

	this->hide();
	emit Create2Log();
}
void CreateUserWindow::CreateClickedBack(){
	this->hide();
	emit Create2Log();
}

void  CreateUserWindow::LogClickedCreate(){
	this->show();

    temp_user_.clear();
    temp_password_.clear();
}

void CreateUserWindow::CreateInputUser(const QString& x){
    temp_user_ = x.toStdString();
}

void CreateUserWindow::CreateInputPassword(const QString& x){
    temp_password_ = x.toStdString();
}
