#pragma once

#ifndef _CHECKWINDOW_H
#define _CHECKWINDOW_H
#include "ui_CheckWindow.h"
#include <QWidget>
#include <QStandardItem>
#include <QDesktopServices>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>

#include "Mail.h"
#include "User.h"

class CheckWindow : public QWidget
{
	Q_OBJECT

public:
    explicit CheckWindow(QWidget *parent = Q_NULLPTR, int order = 0);

private:
    void set_content_(const std::string& sender, const std::string& title,
                  const std::string& time, const std::string& content);
    void set_attachment_(const std::vector<std::pair<std::string, std::string>>&);

private:
	Ui::CheckWindow *ui;
    QStandardItemModel* item_model_;
    string temp_path_ = "../../temp/";
	int order;


private slots:
    /* from main */
	void CheckWindowClickedReply();
	void CheckWindowClickedDelete();
	void CheckWindowClickedForward();
    void CheckWindowClickedIndex(QModelIndex);

    /* from main */
    void MainClickedIndex();

signals:
    void Check2Main();
    void Check2Send();

};

#endif // !_CHECKWINDOW_H