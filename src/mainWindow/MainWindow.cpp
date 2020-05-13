/*-----------------------------------本模块是QT模块，实现用户登陆后的主页面显示--------------------------------------------------------------*/
/*-----------------------------------作者：黄士诚--------------------------------------------------------------------------------------------*/
#include "MainWindow.h"

namespace Uni{
    extern mysql::user* user;
    mysql::Mail mail;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    item_model_ = new QStandardItemModel(this);

}

void MainWindow::MainClickedSend(){
    emit Main2Send();
}

void MainWindow::MainClickedLogout(){
    this->hide();
    emit Main2Log();
}

void MainWindow::MainClickedInbox(){
    /* get from mysql */
    item_model_->clear();
    try{
        emails_ = Uni::user->user_check_inbox();
    }
    catch (mysql::mysqlexecException& e) {
        QMessageBox::about(this, tr("Failed"), tr("Failed check inbox"));
        return;
    }

    /* clicked */
    QStringList str_list_;
    for (auto& m: emails_) {
        auto qstr_ = QString::fromStdString((m.get_all()));
        str_list_.append(qstr_);
        auto *item = new QStandardItem(qstr_);
        item->setSizeHint(QSize(50,60));
        item_model_->appendRow(item);
    }
    ui->listView->setModel(item_model_);
}

void MainWindow::MainClickedIndex(QModelIndex index){
    index_ = index.data().toInt();
    Uni::mail = emails_[index_];

    emit Main2Check();
}

void MainWindow::SendClickedBack(){
	this->show();
}

void MainWindow::LogClickedLog(){
    /* click log */
	this->show();
}

void MainWindow::CheckClickedDelete() {
    /* get from mysql */
    item_model_->clear();
    try {
        Uni::user->user_delete_email(emails_[index_].get_id());
        emails_ = Uni::user->user_check_inbox();
    }
    catch (mysql::mysqlexecException& e) {
        QMessageBox::about(this, tr("Failed"), tr("Failed delete email"));
        return;
    }

    /* clicked */
    QStringList str_list_;
    for (auto& m: emails_) {
        auto qstr_ = QString::fromStdString((m.get_all()));
        str_list_.append(qstr_);
        auto *item = new QStandardItem(qstr_);
        item->setSizeHint(QSize(50,60));
        item_model_->appendRow(item);
    }
    ui->listView->setModel(item_model_);

    /*show */
    this->show();
}

MainWindow::~MainWindow(){
    delete ui;
	delete myuser_;
}
