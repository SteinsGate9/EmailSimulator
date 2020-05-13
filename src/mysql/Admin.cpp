/********************************************
*  Admin
*
********************************************/
#include "Admin.h"


namespace mysql{

    Admin::Admin()
        :conn_pool_(mysql::ConnectionPool::get_instance("localhost", "root", "123456", "email", 0, 8)){
        if (!conn_pool_){
            throw mysql::mysqlpoolException();
        }
        init_database();
    }

    void Admin::create_user(const std::string& userName, const std::string& userPassword){
        /* get instance */
        auto conn_ = conn_pool_->get_connection();

        /* create user table by sql */
        std::vector<std::string> strlist;
        strlist.emplace_back("CREATE USER IF NOT EXISTS '" + userName + "' @'%' IDENTIFIED BY '" + userPassword + "' ;");
        strlist.emplace_back("FLUSH PRIVILEGES;");
        strlist.emplace_back("GRANT select,update,delete,insert on EMAIL to '" + userName + "'@'%';");
        strlist.emplace_back("GRANT select,update,delete,insert on ATTACHMENT to '" + userName + "'@'%';");
        strlist.emplace_back("CREATE OR REPLACE VIEW " + userName + "_emailsend as SELECT * from EMAIL where emailSender = '" + userName + "';");
        strlist.emplace_back("CREATE OR REPLACE VIEW " + userName + "_emailget as SELECT * from EMAIL where emailReceiver = " + "'" + userName + "';");
        strlist.emplace_back("GRANT select,update,delete,insert on " + userName + "_" + "emailsend  to '" + userName + "'@'%';");
        strlist.emplace_back("GRANT select,update,delete,insert on " + userName + "_" + "emailget  to '" + userName + "'@'%';");
        strlist.emplace_back("INSERT INTO USER(userName, userPasswd) SELECT'" + userName + "', '" + userPassword + "' FROM dual WHERE NOT EXISTS (SELECT * FROM USER "
                                                                                                                   " WHERE userName = '" + userName + "');");
        /* create table */
        for(auto& s: strlist){
            if (mysql_real_query(conn_, s.c_str(), (unsigned int)strlen(s.c_str()))){
                long index = std::distance(strlist.begin(), find(strlist.begin(), strlist.end(), s));
                LOG_ERROR("sql exec error on line %ld with error %d: %s", index,
                        mysql_errno(conn_), mysql_error(conn_));
                delete_user(userName, index);
                throw mysql::mysqlexecException(index);
            }

        }

        /* create */
        users_.insert(std::pair<std::string, std::string>(userName, userPassword));

        /* release */
        conn_pool_->release_connection(conn_);
    }

    void Admin::delete_user(const std::string &userName, long int index){
        /* get instance */
        auto conn_ = conn_pool_->get_connection();

        /* delete user */
        std::vector<std::string> strlist;
        if (index >= 1)
            strlist.emplace_back("drop user " + userName + "@'%';");
        if (index >= 5)
            strlist.emplace_back("drop view " + userName + "_emailsend; ");
        if (index >= 6)
            strlist.emplace_back("drop view " + userName + "_emailget; ");
        if (index >= 9)
            strlist.emplace_back("delete from USER where username = '" + userName + "';");

        /* execution */
        for(auto& s: strlist){
            if (mysql_real_query(conn_, s.c_str(), (unsigned int)strlen(s.c_str()))){
                index = std::distance(strlist.begin(), find(strlist.begin(), strlist.end(), s));
                LOG_ERROR("sql exec error on line %ld with error %d: %s", index,
                          mysql_errno(conn_), mysql_error(conn_));
                throw mysql::mysqlexecException(index);
            }

        }

        /* delete */
        users_.erase(userName);

        /* release */
        conn_pool_->release_connection(conn_);
    }

    void Admin::init_database(){
        /* get instance */
        auto conn_ = conn_pool_->get_connection();

        /* sql */
        std::vector<std::string> strlist;
        strlist.emplace_back("CREATE TABLE IF NOT EXISTS EMAIL(\n"
                             "        emailId int PRIMARY KEY NOT NULL AUTO_INCREMENT,\n"
                             "            emailTitle varchar(255),\n"
                             "            emailContext varchar(255),\n"
                             "            emailSender varchar(255),\n"
                             "            emailReceiver varchar(255),\n"
                             "            emailTime datetime,\n"
                             "            emailAttachment varchar(255)\n"
                             "        );");

        strlist.emplace_back(" CREATE TABLE IF NOT EXISTS USER(\n"
                             "            userName varchar(255),\n"
                             "            userPasswd varchar(255)\n"
                             "        );");
        strlist.emplace_back("CREATE TABLE IF NOT EXISTS ATTACHMENT(\n"
                             "        attachmentId int PRIMARY KEY NOT NULL AUTO_INCREMENT,\n"
                             "            attachmentPath varchar(255),\n"
                             "            attachmentCaption varchar(255),\n"
                             "            attachmentContent blob\n"
                             "        );");

        /* execution */
        for(auto& s: strlist){
            if (mysql_real_query(conn_, s.c_str(), (unsigned int)strlen(s.c_str()))){
                int index = std::distance(strlist.begin(), find(strlist.begin(), strlist.end(), s));
                LOG_ERROR("sql exec error on line %d with error %d: %s", index,
                          mysql_errno(conn_), mysql_error(conn_));
                throw mysql::mysqlexecException(index);
            }
        }

        /* release */
        conn_pool_->release_connection(conn_);
    }

}


