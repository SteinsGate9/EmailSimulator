#include <mysql/mysql.h>

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <exception>

#include "src/include/SqlConnectionPool.h"

namespace mysql {

    ConnectionPool *ConnectionPool::conn_pool_ = nullptr;
    static std::mutex mutex_;

    ConnectionPool::ConnectionPool(const std::string& url, const std::string& User, const std::string& PassWord, const std::string& DBName,
            const int& Port, const unsigned int& MaxConn):
            free_conn_(0) {
        /* append conns */
        for (int i = 0; i < MaxConn; i++) {
            MYSQL *con = nullptr;
            con = mysql_init(con);
            if (con == nullptr) {
                LOG_ERROR("sql connection init failed %d!", mysql_errno(con));
                throw mysqlpoolException();
            }

            con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), 0, NULL, 0);
            if (con == nullptr) {
                LOG_ERROR("sql connection connection failed %d!", mysql_errno(con));
                throw mysqlpoolException();
            }
            conn_list_.push_back(con);
            ++free_conn_;
        }
        this->max_conn_ = MaxConn;
        this->cur_conn_ = 0;
    }

    ConnectionPool *ConnectionPool::get_instance(const std::string& url, const std::string& User, const std::string& PassWord, const std::string& DBName,
            const int& Port, const unsigned int& MaxConn){
        std::lock_guard<std::mutex> lck(mutex_);
        /* if not exist*/
        if (conn_pool_ == nullptr) {
            try {
                conn_pool_ = new ConnectionPool(url, User, PassWord, DBName, Port, MaxConn);
            }
            catch (mysqlpoolException& e) {
                conn_pool_->destroy_pool_();
                return nullptr;
            }
        }
        return conn_pool_;
    }

    MYSQL *ConnectionPool::get_connection() {
        MYSQL *con = nullptr;
        std::lock_guard<std::mutex> lck(mutex_);
        /* conn_list */
        if (!conn_list_.empty()) {
            con = conn_list_.front();
            conn_list_.pop_front();

            --free_conn_;
            ++cur_conn_;

            return con;
        }
        return nullptr;
    }

    bool ConnectionPool::release_connection(MYSQL *con) {
        std::lock_guard<std::mutex> lck(mutex_);
        /* release conn */
        if (con != nullptr) {
            conn_list_.push_back(con);

            ++free_conn_;
            --cur_conn_;

            return true;
        }
        return false;
    }

    void ConnectionPool::destroy_pool_() {
        std::lock_guard<std::mutex> lck(mutex_);
        if (!conn_list_.empty()) {
            std::list<MYSQL *>::iterator it;
            for (it = conn_list_.begin(); it != conn_list_.end(); ++it) {
                MYSQL *con = *it;
                mysql_close(con);
            }
            cur_conn_= 0;
            free_conn_ = 0;
            conn_list_.clear();
        }
    }
}