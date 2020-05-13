/**
 * lru_replacer_test.cpp
 */
#include <iostream>
#include <cstdio>

#include "gtest/gtest.h"

#include "SqlConnectionPool.h"
#include "Admin.h"
#include "User.h"
#include "logger.h"

namespace test_admin {

    TEST(user_test, email_send_test) {
        /* test connection create user */
        vector<string> temp;
        temp.emplace_back("/Users/huangbenson/Desktop/webdist2.html");
        temp.emplace_back("/Users/huangbenson/Desktop/100X102.jpg");
        try {
            mysql::Admin a;
            a.create_user("test_1", "123456");
            a.create_user("test_2", "123456");
            mysql::user u("test_1", "123456");
            string _1 = "test_2"; string _2 = "test_2"; string _3 = "test_3"; string _4 = "test_3";
            u.user_send_email("test_2", "title2", "context2", "caption2", "/Users/huangbenson/Desktop/webdist2.html");
            u.user_send_email(_1, _2, _3, _4, temp);
//            u.user_send_email("test_2", "title3", "context3", "caption3", "/Users/huangbenson/Desktop/webdist2.html");
        }
        catch (mysql::mysqlpoolException&){
            EXPECT_EQ(0, 1);
        }
        catch (mysql::mysqlexecException& e){
            EXPECT_EQ(0, 1);
        }

        /* test check email */
        mysql::user u("test_2", "123456");
        auto m = u.user_check_inbox();
        EXPECT_EQ((--m.end())->get_attachmentid(), "1;2");
        EXPECT_EQ((--m.end())->get_title(), "test_2");
        EXPECT_EQ((--m.end())->get_sender(), "test_1");
        EXPECT_EQ((--m.end())->get_context(), "test_3");

        /* test delete email */
        mysql::user u2("test_1", "123456");
        u2.user_delete_email(25);

        /* test */
        auto res = u2.user_get_attachment("1;2");
//        EXPECT_EQ(res.)

        /* check */
        auto conn_pool_ = mysql::ConnectionPool::get_instance("localhost", "root", "123456", "email", 0, 8);
        EXPECT_EQ(conn_pool_->get_free_conn(), 8);

        /* test path */
        char* path = getcwd(nullptr, 0);
        strcat(path, "/tmp.file");
        puts(path);
        free(path);

        /* test path2 */
        char *path_ = __ROOTPATH__;

    }


} // namespace cmudb
