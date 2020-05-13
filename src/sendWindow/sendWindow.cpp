/********************************************
*
*
********************************************/
#include "sendWindow.h"


namespace Uni{
    extern string username;
    extern string userpassword;
    extern mysql::user* user;
    extern mysql::Mail mail;
}

sendWindow::sendWindow(QWidget *parent)
	: QWidget(parent), ui(new Ui::sendWindow()){
	ui->setupUi(this);

	connect(ui->sendto,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(SendInputSendto(QString))
	);
	connect(ui->title,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(SendInputTitle(QString))
	);
	connect(ui->context,
		SIGNAL(textChanged()),
		this,
		SLOT(SendInputContent())
	);


}

void sendWindow::SendInputSendto(const QString& x){
	this->sendto_ = x.toStdString();
}

void sendWindow::SendInputTitle(const QString& x){
	this->title_ = x.toStdString();
}

void sendWindow::SendInputContent(){
	this->context_ = ui->context->toPlainText().toStdString();
}

void sendWindow::SendClickedBack(){
    this->hide();
    emit Send2Main();
}

void sendWindow::SendClickedAppendix(){
    /* open file icon */
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Appendix"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("All files (*.*);;Images (*.jpg *.jpeg *.tif *.bmp *.png);;Binary File(*.bin *.txt *.hex);;\
                                               execulate (*.exe *.com *.pe)"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);

    /* if selected */
    if (fileDialog->exec() == QDialog::Accepted){
        file_names_ = fileDialog->selectedFiles();
    }

    /* update ui*/
    for (QList<QString>::iterator iter = file_names_.begin(); iter != file_names_.end(); ++iter){
        ui->box->addItem(*iter);
    }

}

void sendWindow::SendClickedSend(){
    if (sendto_.empty()){
        QMessageBox::about(this, tr("Failed"), tr("Need receiver"));
        return;
    }
    if (title_.empty()){
        QMessageBox::about(this, tr("Failed"), tr("Need title"));
        return;
    }
    if (context_.empty()){
        QMessageBox::about(this, tr("Failed"), tr("Need context"));
        return;
    }
    std::vector<string> concat;
    if (!file_names_.empty()){
      for (QList<QString>::iterator iter = file_names_.begin(); iter != file_names_.end(); ++iter){
          concat.emplace_back((*iter).toStdString());
      }
      try {
          Uni::user->user_send_email(sendto_, title_, context_, "caption", concat);
      }
      catch (mysql::mysqlexecException& e) {
          QMessageBox::about(this, tr("Failed"), tr("Failed sending email"));
          return;
      }
    }

    else {
        try {
            Uni::user->user_send_email(sendto_, title_, context_, "caption", concat);
        }
        catch (mysql::mysqlexecException& e) {
            QMessageBox::about(this, tr("Failed"), tr("Failed sending email"));
            return;
        }
    }
    QMessageBox::about(this, tr("Succeed"), tr("Email Sent"));
    ui->sendto->clear();
    ui->title->clear();
    ui->context->clear();
    ui->box->clear();
    file_names_.clear();
    return;

}

void sendWindow::SendClickedCancel(){
  ui->sendto->clear();
  ui->title->clear();
  ui->context->clear();
  ui->box->clear();
  file_names_.clear();
}



void sendWindow::MainClickedSend(){
  ui->sendto->clear();
  ui->title->clear();
  ui->context->clear();
  ui->box->clear();
  file_names_.clear();
  this->show();
}

void sendWindow::CheckClickedReply(){
    ui->sendto->setText(QString::fromStdString(Uni::mail.get_sender()));
    ui->context->setText(QString::fromStdString("Reply:"));

    this->show();
};


//void sendWindow::LogClickedLog(std::string a, std::string b){
//	user2 = a;
//	password = b;
//}


void sendWindow::split_(std::vector<std::string> &Result, std::string &Input, const char* Regex)
{
	int pos = 0;
	int npos = 0;
	int regexlen = strlen(Regex);
	while ((npos = Input.find(Regex, pos)) != -1){
		std::string tmp = Input.substr(pos, npos - pos);
		Result.push_back(tmp);
		pos = npos + regexlen;
	}
	Result.push_back(Input.substr(pos, Input.length() - pos));
}



void sendWindow::replyWindowSet(std::string Receiver){
	ui->sendto->setText(QString::fromStdString(Receiver));
	ui->title->setText(QString::fromStdString("Reply"));

	this->sendto_ = Receiver;
	this->title_ = "Reply";
}

void sendWindow::forwardWindowSet(std::string Content){
	ui->context->setText(QString::fromStdString(Content));

	this->context_ = Content;
}

void sendWindow::setUser(mysql::user u){
	this->myuser_ = &u;
}
////////////////////////////////////////////////////////////