/********************************************
*  This file is intended by
********************************************/
#include "logWindow.h"

namespace Uni{
    std::string username;
    std::string userpassword;
    mysql::user* user;
}

logWindow::logWindow(QWidget *parent)
	: QWidget(parent), ui(new Ui::logWindow()) {
	ui->setupUi(this);
	ui->password->setEchoMode(QLineEdit::Password);

	connect(ui->user,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(LogInputUser(QString))
	    );
	connect(ui->password,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(LogInputPassword(QString))
	);
}

void logWindow::LogClickedLog(){
    /* mysql connection */
    try {
        Uni::username = user_;
        Uni::userpassword = password_;
        Uni::user = new mysql::user(user_, password_);
    }
    catch (mysql::mysqlpoolException&) {
        QMessageBox::about(this, tr("Failed"), tr("Can't connect to email database "));
		return;
    }

    /* prog_dlg */
	auto *prog_dlg = new QProgressDialog();
    prog_dlg->setWindowTitle("Please wait...");
    prog_dlg->setFixedWidth(300);
    prog_dlg->setRange(0, 50000);
    prog_dlg->show();
    QApplication::processEvents();
    prog_dlg->cancel();

    this->hide();
	emit Log2Main();
}


void logWindow::MainClickedLogout(){
	user_.clear();
	password_.clear();
	this->show();
}

void logWindow::LogClickedCreate(){
	emit Log2Create();
}

void logWindow::CreateClickedBack(){
    this->show();
}

void logWindow::LogInputUser(const QString& x){
    user_ = x.toStdString();
}

void logWindow::LogInputPassword(const QString& x){
    password_ = x.toStdString();
}
