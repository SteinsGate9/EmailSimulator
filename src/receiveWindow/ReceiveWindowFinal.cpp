#include "qmessagebox.h"

#include "ReceiveWindowFinal.h"

ReceiveWindowFinal::ReceiveWindowFinal(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::ReceiveWindowFinalClass();
	ui->setupUi(this);
	this->setWindowTitle(QString::fromStdString("Inbox"));

	mailChosen.clear();
	/*read in the emails myuser recieved into List*/
	List.clear();
	connect(ui->search,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(getSigFromSearch(QString))
	);

}
void ReceiveWindowFinal::getSigFromMainClickedReceive(){}
//{
//	mysqlpp::StoreQueryResult res = myuser_->userCheckEmail();
//	mysqlpp::StoreQueryResult::const_iterator it;
//	Mail *m;
//	List.clear();
//	if (res)
//	{
//		for (it = res.begin(); it != res.end(); ++it)
//		{
//			mysqlpp::Row row = *it;
//			//TODO
////			string sender = row[3];
////			string time = row[5];
////			string title = row[1];
////			string emailID = row[0];
////			m = new Mail(sender, title, time, emailID);
//			List.push_back(*m);
//		}
//	}
//	initWidget(List);
//	this->show();
//}
///*delivering the chosing mails' orders to ReceiveWindow*/
void ReceiveWindowFinal::ReceiveOrder(int order){}
//{
//	if(order>=0)
//		this->mailChosen.push_back(order);
//	else {
//		for (size_t i = 0; i < mailChosen.size(); i++)
//		{
//			if (mailChosen[i] == -order)
//				mailChosen.erase(mailChosen.begin() + i);
//		}
//	}
//}
//
///*deal with the delete operation from the CheckWindow*/
void ReceiveWindowFinal::DeleteOneMail(int order){}
//{
//	/*delete from db*/
//	myuser_->userDeleteEmailGet((List[order]).get_emailid());
//
//	/*delete from mailChosen*/
//	for (size_t i = 0; i < mailChosen.size(); i++)
//	{
//		if (mailChosen[i] == order)
//			mailChosen.erase(mailChosen.begin() + i);
//	}
//
//	/*delete from ReceiveWindow screen*/
//	delete showList[order];
//
//	/*QLabel *debug = new QLabel();
//	debug->setText(QString::number(order));
//	debug->show();*/
//}
//
///*display all the mails received*/
//void ReceiveWindowFinal::initWidget(std::vector<Mail> List)
//{
//	OneMailLine * oneline;
//
//	/*setting up an scrollArea*/
//	ui->scrollArea->setWidgetResizable(false);
//	ui->scrollAreaWidgetContents->setGeometry(10, 90, 570, 40 * List.size());
//	/*Adding a Vertical Layout to it*/
//	QVBoxLayout * vb = new QVBoxLayout(ui->scrollAreaWidgetContents);
//	vb->setSpacing(4);
//
//	/*setting up displays for each mail*/
//	for (size_t i = 0; i < List.size(); i++)
//	{
//		oneline = new OneMailLine(this->ui->scrollAreaWidgetContents,i);
//		oneline->SetLabels(List[i]);
//		//QMessageBox::about(this, tr("��ʾ��Ϣ"), tr(List[i].getemailId().c_str()));
//		//oneline->setGeometry(10, 40 * i, 500, 40);
//		vb->addWidget(oneline);
//		this->showList.push_back(oneline);
//		connect(oneline,
//			SIGNAL(sendOrder(int)),
//			this,
//			SLOT(ReceiveOrder(int)));
//	}
//	ui->scrollAreaWidgetContents->setLayout(vb);
//}
//
///*Check the chosen mails*/
void ReceiveWindowFinal::SigFromCheckbtn(){}
//{
//	//QLabel * debug;
//	/*debug->setText(QString::number(this->mailChosen.size()));
//	debug->show();*/
//
//	CheckWindow * mailcheck;
//	//mailcheck->SetContent("Amy", "Hello", "2016-9-1", "It's nice to meet you.\nGood night.\n");
//
//	/*setting up display window for the chosen mails*/
//	for (size_t i = 0; i < mailChosen.size(); i++)
//	{
//		mailcheck = new CheckWindow(nullptr,mailChosen[i]);
//		//List[mailChosen[i]].getemailId();
//		//QMessageBox::about(this, tr("��ʾ��Ϣ"), tr(List[mailChosen[i]].getemailId().c_str()));
//		mysqlpp::StoreQueryResult res = myuser_->userCheckEmailReceived(List[mailChosen[i]].get_emailid());
//		mysqlpp::StoreQueryResult::const_iterator it;
//		it = res.begin();
//		mysqlpp::Row row = *it;
//		// TODO
//		string content = "1";
////		string content = row[0];
//
//		mailcheck->SetContent(List[i].get_sender(), List[i].get_title(), List[i].get_time(), content);
//		//mailcheck->SetContent(to_string(mailChosen[i]), "Hello", "2018-6-14", "Good Afternoon!\n");
//		connect(mailcheck,
//			SIGNAL(sendOrderToDelete(int)),
//			this,
//			SLOT(DeleteOneMail(int)));
//		mailcheck->show();
//	}
//}
//
void ReceiveWindowFinal::SigFromBackbtn(){}
//{
//	this->close();/*only to quit*/
//	//delete(ui->Back_btn);
//	//delete ui;
//}
//
///*uncompleted, used to show which mails are chosen*/
void ReceiveWindowFinal::SigFromFowardbtn(){}
//{
//	string mails;
//	for (size_t i = 0; i < mailChosen.size(); i++)
//	{
//		mails += to_string(mailChosen[i]);
//		mails += " ";
//	}
//
//	QLabel * debug = new QLabel();
//	debug->setText(QString::fromStdString(mails));
//	debug->show();
//}
//
///*delete the chosen mails from ReceiveWindow*/
void ReceiveWindowFinal::SigFromDeletebtn(){}
//{
//
//	for (size_t i = 0; i < mailChosen.size(); i++)
//	{
//		/*delete from db*/
//		myuser_->userDeleteEmailGet(List[i].get_emailid());
//
//		/*delete from the gui window*/
//		delete showList[mailChosen[i]];
//	}
//	//ui->scrollAreaWidgetContents->setGeometry(10, 90, 570, 40 * showList.size());
//	mailChosen.clear();
//}
//
void ReceiveWindowFinal::SigFromSearchbtn(){}
//{
//	mysqlpp::StoreQueryResult res = myuser_->userCheckEmailRetrieved(search_);
//
//	mysqlpp::StoreQueryResult::const_iterator it;
//	Mail *m;
//	vector<Mail> List;
//	if (res)
//	{
//		for (it = res.begin(); it != res.end(); ++it)
//		{
//			mysqlpp::Row row = *it;
//			//TODO
////			string sender = row[3];
////			string time = row[5];
////			string title = row[1];
////			string emailID = row[0];
////			m = new Mail(sender, title, time, emailID);
//			List.push_back(*m);
//		}
//	}
//	//QMessageBox::about(this, tr("��ʾ��Ϣ"), tr(List[0].getemailId().c_str()));
//	initWidget(List);
//	this->show();
//}
//
void ReceiveWindowFinal::getSigFromSearch(QString x){}
//{
//	this->search_ = x.toStdString();
//}
//
//
///////////////////////////////////////
////
////Reply one mail from check window
////
/////////////////////////////////////////
void ReceiveWindowFinal::ReplyOneMail(int order){}
//{
//	sendWindow * replywin = new sendWindow();
//
//	replywin->replyWindowSet(List[order].get_sender());
//	replywin->setUser(*myuser_);
//	replywin->show();
//}
//
///////////////////////////////////////
////
////Forward one mail from check window
////
/////////////////////////////////////////
void ReceiveWindowFinal::ForwardOneMail(int order){}
//{
//	sendWindow * forwardWindow = new sendWindow();
//
//	forwardWindow->setUser(*myuser_);
//
//	mysqlpp::StoreQueryResult res = myuser_->userCheckEmailReceived(List[order].get_emailid());
//	mysqlpp::StoreQueryResult::const_iterator it;
//
//	it = res.begin();
//	mysqlpp::Row row = *it;
//	// TODO
//	string content = "1";
////	string content = row[0];
//	forwardWindow->forwardWindowSet(content);
//
//	forwardWindow->show();
//}