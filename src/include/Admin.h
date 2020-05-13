/*-----------------------------------��ģ���Ǻ���ģ�飬ʵ�ֹ���Ա���û��Ĵ����͹���----------------------------------------------------------*/
/*-----------------------------------���ߣ���ʿ��--------------------------------------------------------------------------------------------*/
#pragma once

#ifndef ADMIN_H
#define ADMIN_H

#include "mysql/mysql.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "SqlConnectionPool.h"

namespace mysql {

    class Admin {
        public:
            Admin();

        public:
            void create_user(const std::string& userName, const std::string& userPassword);
            void delete_user(const std::string& userName, long int index = 9);
            void init_database();


        private:
            std::unordered_map<std::string, std::string> users_;
            mysql::ConnectionPool* conn_pool_;
    };
}
#endif