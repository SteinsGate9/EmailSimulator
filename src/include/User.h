/********************************************
*   user.h
*
********************************************/

/********************************************
*   EMAIL : CREATE TABLE EMAIL(
        emailId int PRIMARY KEY NOT NULL AUTO_INCREMENT,
        emailTitle varchar(255),
        emailContext varchar(255),
        emailSender varchar(255),
        emailReceiver varchar(255),
        emailTime datetime,
        emailAttachment int
);

*   USER : CREATE TABLE USER(
        userName varchar(255),
        userPasswd varchar(255)
);

*   ATTACHMENT : CREATE TABLE ATTACHMENT(
        attachmentId int PRIMARY KEY NOT NULL AUTO_INCREMENT,
        attachmentPath varchar(255),
        attachmentCaption varchar(255),
        attachmentContent blob
);
********************************************/
#pragma once

#ifndef USER_H
#define USER_H

#include <QTextCodec>
#include <qstring.h>
#include <qfile.h>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtWidgets/qpushbutton.h>
#include <QMessageBox>

#include "mysql++/mysql++.h"
#include <fstream>
#include <time.h>

#include "Mail.h"
#include "SqlConnectionPool.h"
#include "config.h"


namespace mysql {

    class user {
    public :
        explicit user(const std::string &, const std::string &);
        explicit user(){};

    public:
        std::string username() { return username_; };
        std::string userpassword() { return userpassword_; };

    public:
        std::vector<mysql::Mail> user_check_inbox();
        std::vector<mysql::Mail> user_check_sent();
        void
        user_send_email(const std::string& to, const std::string& title, const std::string& context, const std::string& caption, const std::string& path);
        void
        user_send_email(const std::string& to, const std::string& title, const std::string& context, const std::string& caption, std::vector<std::string>& path);
        void user_delete_email(int email_id);
        std::vector<std::pair<std::string, std::string>> user_get_attachment(const string& attachmentid);


    private :
        std::string username_;
        std::string userpassword_;
        mysql::ConnectionPool* conn_pool_;
    };
}
#endif
