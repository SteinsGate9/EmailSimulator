#pragma once
#include "ui_sendWindow.h"
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>

#include <iostream>
#include <string>
#include <cstdlib>

#include "User.h"
#include "head.h"

namespace Ui { class sendWindow; };

class sendWindow : public QWidget
{
	Q_OBJECT

 public:
  explicit sendWindow(QWidget *parent = Q_NULLPTR);

 public:
  void replyWindowSet(std::string Receiver);
  void forwardWindowSet(std::string Content);
  void setUser(mysql::user u);

 private:
  void split_(std::vector<std::string> &Result, std::string &Input, const char* Regex);

 private:
  Ui::sendWindow *ui;

 private:
  mysql::user* myuser_;
  std::string password_;
  std::string user_;

  std::string sendto_;
  std::string title_;
  std::string context_;
  std::string appendix_address_;
  QStringList file_names_;


private slots:
  /* from Send */
  void SendInputSendto(const QString& x);
  void SendInputTitle(const QString& x);
  void SendInputContent();

  void SendClickedBack();
  void SendClickedAppendix();
  void SendClickedSend();
  void SendClickedCancel();

  /* from Main */
  void MainClickedSend(); // from Main

  /* from Check */
  void CheckClickedReply();

signals:
  void Send2Main();

};



