#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>

#include "mysql++/mysql++.h"
#include <string.h>
#include <iostream>
#include <string>
#include <mutex>

#include "logger.h"

namespace mysql {
    class mysqlpoolException: public std::exception{
        const char * what () const throw (){
            return "userException";
        }
    };

    class mysqlexecException: public std::exception{
    public:
        explicit mysqlexecException(const int& index):index_(index) {};
        explicit mysqlexecException(const std::string& str):excep_str_(str) {};

    public:
        int index(){
            return index_;
        }
        std::string excep_str(){
            return excep_str_;
        }

    private:
        int index_;
        std::string excep_str_;
    };

    class ConnectionPool {
    public:
        ~ConnectionPool() {destroy_pool_();};

    public:
        uint32_t get_free_conn() { return free_conn_; }

    public:
        static ConnectionPool *get_instance(const std::string& url, const std::string& User, const std::string& PassWord, const std::string& DBName,
                                        const int& Port, const unsigned int& MaxConn);
        MYSQL *get_connection();                 //获取数据库连接
        bool release_connection(MYSQL *conn); //释放连接

    private:
        ConnectionPool(const std::string& url, const std::string& User, const std::string& PassWord, const std::string& DBName,
                   const int& Port, const unsigned int& MaxConn);
        void destroy_pool_();                     //销毁所有连接

    private:
        /* conn related */
        std::list<MYSQL *> conn_list_; //连接池
        static ConnectionPool *conn_pool_;
        uint32_t max_conn_;  //最大连接数
        uint32_t cur_conn_;  //当前已使用的连接数
        uint32_t free_conn_; //当前空闲的连接数

    };
}
#endif
