#pragma once

#include <QWidget>
#include "ui_OneMailLine.h"
#include "src/include/Mail.h"
#include <string>
#include <iostream>

class OneMailLine : public QWidget
{
	Q_OBJECT

public:
	OneMailLine(QWidget *parent = Q_NULLPTR, int i = 0);
	~OneMailLine(){delete ui;};
	void SetLabels(const mysql::Mail& m);

private:
	Ui::OneMailLine *ui;
	int order_;

private slots:
	void SigFromChoose(int);

signals:
    void sendOrder(int order);

};
