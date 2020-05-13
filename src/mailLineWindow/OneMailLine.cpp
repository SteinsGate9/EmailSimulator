#include "OneMailLine.h"

OneMailLine::OneMailLine(QWidget *parent, int i)
	: QWidget(parent), order_(i){
	ui = new Ui::OneMailLine();
	ui->setupUi(this);
}


void OneMailLine::SetLabels(const mysql::Mail& m)
{
	ui->Sender->setText(QString::fromStdString(m.get_sender()));
	ui->Title->setText(QString::fromStdString(m.get_title()));
	ui->Time->setText(QString::fromStdString(m.get_time()));
	ui->Choose->setText(QString::fromStdString(""));
}

void OneMailLine::SigFromChoose(int state)
{
	if (state==Qt::Checked)
		emit sendOrder(this->order_);
	else {
		emit sendOrder(-this->order_);
	}
	/*QLabel * label = new QLabel();
	label->setText(QString::number(order));
	label->show();*/
}