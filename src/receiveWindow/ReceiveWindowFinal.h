#pragma once
#include "ui_ReceiveWindowFinal.h"
#include <QtWidgets/QMainWindow>
#include <qscrollarea.h>

#include <vector>

#include "OneMailLine.h"
#include "CheckWindow.h"
#include "sendWindow.h"
#include "Mail.h"
#include "User.h"



class ReceiveWindowFinal : public QMainWindow
{
	Q_OBJECT

public:
    explicit ReceiveWindowFinal(QWidget *parent = Q_NULLPTR);
	~ReceiveWindowFinal() { delete ui; delete myuser_; }
	void initWidget(std::vector<mysql::Mail> List);

private:
    Ui::ReceiveWindowFinalClass *ui;

private:
    mysql::user *myuser_;
    std::string user_;
    std::string password_;

	std::string search_;
	std::vector<int> mailChosen;/*recording mails chosen*/
	vector<mysql::Mail> List;/*recording the mails received*/
	vector<OneMailLine*> showList;/*recording the displayment of each mail*/
	vector<OneMailLine*> showSearch;

private slots:
	void SigFromBackbtn();
	void SigFromCheckbtn();
	void SigFromFowardbtn();
	void SigFromDeletebtn();
	void SigFromSearchbtn();
	void ReceiveOrder(int order);/*deal with the choose operation*/
	void DeleteOneMail(int order);/*deal with deleting on CheckWindow*/

	//New
	void ReplyOneMail(int order);/*deal with replying from CheckWindow*/
	//New
	void ForwardOneMail(int order);/*deal with forwarding from CheckWindow*/

	void getSigFromMainClickedReceive();
	void getSigFromSearch(QString);
};
