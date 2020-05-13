/********************************************
*   user.cpp
*   Usage:
********************************************/
#include "User.h"

namespace mysql {

    user::user(const std::string &username, const std::string &userpassword)
            : username_(username), userpassword_(userpassword),
              conn_pool_(mysql::ConnectionPool::get_instance("localhost", username, userpassword, "email", 0, 8)) {
        if (!conn_pool_) {
            throw mysql::mysqlpoolException();
        }
    }

    void
    user::user_send_email(const std::string &to, const std::string &title, const std::string &context,
                          const std::string &caption, const std::string &path) {
        if (!path.empty()) {
            QString myfile;
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

            /* file 2 Qstring */
            QFile file(QString::fromLocal8Bit(path.c_str()));
            file.open(QIODevice::ReadOnly);
            if (file.isOpen()) {
                QTextStream in(&file);
                QString line = in.readLine();
                while (!line.isNull()) {
                    myfile += line;
                    line = in.readLine();
                }
            }

            /* insert into attachment */
            auto temp_conn_ = conn_pool_->get_connection();
            char *temp_data_ = (char*)malloc(FILE_BUFFER_SIZE);
            char *real_sql_ = (char*)malloc(FILE_BUFFER_SIZE);
            char *temp_file = myfile.toLocal8Bit().data();
            // executee
            string temp_str_ = "INSERT INTO ATTACHMENT(attachmentPath, attachmentCaption, attachmentContent) "
                              "SELECT '%s', '%s', '%s' FROM dual WHERE NOT EXISTS (SELECT * FROM ATTACHMENT "
                              "WHERE attachmentPath = '%s');";
            char *temp_sql_ = (char*)temp_str_.c_str();
            mysql_real_escape_string(temp_conn_, temp_data_, temp_file, strlen(temp_file));
            int len = sprintf(real_sql_, temp_sql_, path.c_str(), caption.c_str(), temp_data_, path.c_str());
            if (mysql_real_query(temp_conn_, real_sql_, len)) {
                LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                          mysql_errno(temp_conn_), mysql_error(temp_conn_));
                throw mysqlexecException(0);
            }
            // get index
            temp_str_ = "SELECT attachmentId FROM ATTACHMENT WHERE attachmentPath = '%s';";
            temp_sql_ = (char*)temp_str_.c_str();
            len = sprintf(real_sql_, temp_sql_, path.c_str());
            if (mysql_real_query(temp_conn_, real_sql_, len)) {
                LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                          mysql_errno(temp_conn_), mysql_error(temp_conn_));
                throw mysqlexecException(0);
            }
            auto result = mysql_store_result(temp_conn_);
            MYSQL_ROW row = mysql_fetch_row(result);

            /* insert into email */
            char temp_time_[20];
            time_t rawtime_ = time(0);
            strftime(temp_time_, 20, "%Y-%m-%d %X", localtime(&rawtime_));
            temp_str_ = "INSERT INTO EMAIL(emailTitle, emailContext, emailSender, emailReceiver, emailTime, emailAttachment)"
                        " VALUE ('%s', '%s', '%s', '%s', '%s', %s);";
            temp_sql_ = (char*)temp_str_.c_str();
            sprintf(real_sql_, temp_sql_,
                    title.c_str(), context.c_str(), username_.c_str(), to.c_str(), temp_time_, row[0]);
            if (mysql_real_query(temp_conn_, real_sql_, (unsigned int) strlen(real_sql_))) {
                LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                          mysql_errno(temp_conn_), mysql_error(temp_conn_));
                throw mysqlexecException(0);
            }

            /* release */
            conn_pool_->release_connection(temp_conn_);
            free(temp_data_);
            free(real_sql_);
        }
    }

    void
    user::user_send_email(const std::string& to, const std::string& title, const std::string& context, const std::string& caption, std::vector<std::string>& path){
        /* get connecetion */
        string attachmentid_;
        auto temp_conn_ = conn_pool_->get_connection();

        /* parse string attachment id */
        for (auto &p: path) {
            QString myfile;
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

            /* file 2 Qstring */
            QFile file(QString::fromLocal8Bit(p.c_str()));
            file.open(QIODevice::ReadOnly);
            if (file.isOpen()) {
                QTextStream in(&file);
                QString line = in.readLine();
                while (!line.isNull()) {
                    myfile += line;
                    line = in.readLine();
                }
            }

            /* insert into attachment */
            char *temp_data_ = (char*)malloc(FILE_BUFFER_SIZE);
            char *real_sql_ = (char*)malloc(FILE_BUFFER_SIZE);
            char *temp_file = myfile.toLocal8Bit().data();
            // execute
            string temp_str_ = "INSERT INTO ATTACHMENT(attachmentPath, attachmentCaption, attachmentContent) "
                                    "SELECT '%s', '%s', '%s' FROM dual WHERE NOT EXISTS (SELECT * FROM ATTACHMENT "
                                    "WHERE attachmentPath = '%s');";
            char* temp_sql_ = (char*)temp_str_.c_str();
            mysql_real_escape_string(temp_conn_, temp_data_, temp_file, strlen(temp_file));
            int len = sprintf(real_sql_, temp_sql_, p.c_str(), caption.c_str(), temp_data_, p.c_str());
            if (mysql_real_query(temp_conn_, real_sql_, len)) {
                LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                          mysql_errno(temp_conn_), mysql_error(temp_conn_));
                throw mysqlexecException(0);
            }

            /* attachment id */
            temp_str_ = "SELECT attachmentId FROM ATTACHMENT WHERE attachmentPath = '%s';";
            temp_sql_ = (char*)temp_str_.c_str();
            len = sprintf(real_sql_, temp_sql_, p.c_str());
            if (mysql_real_query(temp_conn_, real_sql_, len)) {
                LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                          mysql_errno(temp_conn_), mysql_error(temp_conn_));
                throw mysqlexecException(0);
            }
            auto result = mysql_store_result(temp_conn_);
            MYSQL_ROW row = mysql_fetch_row(result);
            attachmentid_ += std::string(row[0]) + ";";
            free(temp_data_);
            free(real_sql_);
        }
        if (!path.empty())
            attachmentid_.erase(--attachmentid_.end());

        /* insert into email */
        char temp_time_[20];
        time_t rawtime_ = time(0);
        strftime(temp_time_, 20, "%Y-%m-%d %X", localtime(&rawtime_));
        string temp_str_ =
            "INSERT INTO EMAIL(emailTitle, emailContext, emailSender, emailReceiver, emailTime, emailAttachment)"
            " VALUE ('%s', '%s', '%s', '%s', '%s', '%s');";
        char* temp_sql_ = (char*)temp_str_.c_str();
        char real_sql_[SQL_BUFFER_SIZE];
        sprintf(real_sql_, temp_sql_,
                title.c_str(), context.c_str(), username_.c_str(), to.c_str(), temp_time_, attachmentid_.c_str());
        if (mysql_real_query(temp_conn_, real_sql_, (unsigned int) strlen(real_sql_))) {
            LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                      mysql_errno(temp_conn_), mysql_error(temp_conn_));
            throw mysqlexecException(0);
        }

        /* release */
        conn_pool_->release_connection(temp_conn_);
    }

    std::vector<mysql::Mail> user::user_check_inbox() {
        /* sql */
        auto temp_conn_ = conn_pool_->get_connection();
        char real_sql_[SQL_BUFFER_SIZE];
        string temp_str_ = "SELECT * FROM %s_emailget;";
        char* temp_sql_ = (char*)temp_str_.c_str();
        int len = sprintf(real_sql_, temp_sql_, username_.c_str());
        if (mysql_real_query(temp_conn_, real_sql_, len)) {
            LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                      mysql_errno(temp_conn_), mysql_error(temp_conn_));
            throw mysqlexecException(0);
        }
        auto result = mysql_store_result(temp_conn_);
        MYSQL_ROW row;
        std::vector<mysql::Mail> temp_vector_;
        while ((row = mysql_fetch_row(result))) {
            temp_vector_.emplace_back(mysql::Mail(strtol(row[0], nullptr, 10), row[1], row[2], row[3], row[4], row[5], row[6]));

        }

        /* release */
        conn_pool_->release_connection(temp_conn_);

        return temp_vector_;
    }

    std::vector<mysql::Mail> user::user_check_sent() {
        /* sql */
        auto temp_conn_ = conn_pool_->get_connection();
        char real_sql_[SQL_BUFFER_SIZE];
        string temp_str_ = "SELECT * FROM %s_emailsend;";
        char* temp_sql_ = (char*)temp_str_.c_str();
        int len = sprintf(real_sql_, temp_sql_, username_.c_str());
        if (mysql_real_query(temp_conn_, real_sql_, len)) {
            LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                      mysql_errno(temp_conn_), mysql_error(temp_conn_));
            throw mysqlexecException(0);
        }
        auto result = mysql_store_result(temp_conn_);
        MYSQL_ROW row;
        std::vector<mysql::Mail> temp_vector_;
        while ((row = mysql_fetch_row(result))) {
            temp_vector_.emplace_back(mysql::Mail(strtol(row[0], nullptr, 10), row[1], row[2], row[3], row[4], row[5], row[6]));
        }

        /* release */
        conn_pool_->release_connection(temp_conn_);

        return temp_vector_;
    }

    void user::user_delete_email(int email_id) {
        /* sql */
        auto temp_conn_ = conn_pool_->get_connection();
        char real_sql_[SQL_BUFFER_SIZE];
        string temp_str_ = "delete from EMAIL where emailId = %d;";
        char* temp_sql_ = (char*)temp_str_.c_str();
        int len = sprintf(real_sql_, temp_sql_, email_id);
        if (mysql_real_query(temp_conn_, real_sql_, len)) {
            LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                      mysql_errno(temp_conn_), mysql_error(temp_conn_));
            throw mysqlexecException(0);
        }

        /* release */
        conn_pool_->release_connection(temp_conn_);
    }


    static vector<string> split_(const string& str, const string& delim) {
        vector<string> res;
        if ("" == str)
            return res;
        //先将要切割的字符串从string类型转换为char*类型
        char *strs = new char[str.length() + 1]; //不要忘了
        strcpy(strs, str.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(strs, d);
        while (p) {
            string s = p; //分割得到的字符串转换为string类型
            res.push_back(s); //存入结果数组
            p = strtok(NULL, d);
        }

        delete []strs;
        delete []d;

        return res;
    }

    std::vector<std::pair<std::string, std::string>> user::user_get_attachment(const string& attachmentid){
        /* sql */
        auto temp_conn_ = conn_pool_->get_connection();

        /* split */
        std::vector<std::pair<std::string, std::string>> output_;
        auto splited = split_(attachmentid, ";");
        for (auto& s: splited) {
            char real_sql_[SQL_BUFFER_SIZE];
            string temp_str_ = "SELECT attachmentPath, attachmentContent FROM ATTACHMENT WHERE attachmentId = %d;";
            char* temp_sql_ = (char*)temp_str_.c_str();
            int len = sprintf(real_sql_, temp_sql_, std::strtol(s.c_str(), nullptr, 10));
            if (mysql_real_query(temp_conn_, real_sql_, len)) {
                LOG_ERROR("sql exec error on line %d with error %d: %s", 0,
                          mysql_errno(temp_conn_), mysql_error(temp_conn_));
                throw mysqlexecException(0);
            }
            auto result = mysql_store_result(temp_conn_);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                output_.emplace_back(std::pair<string, string>{row[0], row[1]});
            }
        }

        /* release */
        conn_pool_->release_connection(temp_conn_);

        return output_;

    }
}


