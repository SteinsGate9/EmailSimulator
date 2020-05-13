/**
 * lru_replacer_test.cpp
 */
#include <iostream>
#include <cstdio>
#include "gtest/gtest.h"

#include "SqlConnectionPool.h"
#include "Admin.h"
#include "logger.h"

namespace test_admin {

    TEST(admin_test, SampleTest) {
        /* test connection create user */
        try {
            mysql::Admin temp_a;
            temp_a.create_user("test_1", "123456");
        }
        catch (mysql::mysqlpoolException&){
            EXPECT_EQ(0, 1);
        }
        catch (mysql::mysqlexecException& e){
            EXPECT_EQ(0, 1);
        }

        /* test create user */
        auto conn_pool_ = mysql::ConnectionPool::get_instance("localhost", "root", "123456", "email", 0, 8);
        MYSQL* temp_conn = conn_pool_->get_connection();
        std::string sql = "select userName, userPasswd from USER where username='test_1'";
        int flag = mysql_real_query(temp_conn, sql.c_str(), (unsigned int)strlen(sql.c_str()));
        EXPECT_EQ(flag, 0);

        /* test select user */
        std:: string temp_e;
        MYSQL_ROW row;
        auto result = mysql_store_result(temp_conn);
        while ((row = mysql_fetch_row(result))){
            temp_e = row[1];
        }
        EXPECT_EQ(temp_e, "123456");

        /* test delete user */
        try {
            mysql::Admin temp_a;
            temp_a.delete_user("test_1");
        }
        catch (mysql::mysqlpoolException&){
            EXPECT_EQ(0, 1);
        }
        catch (mysql::mysqlexecException& e){
            EXPECT_EQ(0, 1);
        }

        /* test delete user */
        flag = mysql_real_query(temp_conn, sql.c_str(), (unsigned int)strlen(sql.c_str()));
        result = mysql_store_result(temp_conn);
        row = mysql_fetch_row(result);
        EXPECT_EQ(row, nullptr);

        conn_pool_->release_connection(temp_conn);
    }


} // namespace cmudb
