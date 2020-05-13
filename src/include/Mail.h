#pragma once
#ifndef _MAIL_H
#define _MAIL_H
#include <string>

using namespace std;

namespace mysql {
    class Mail {
    public:
        friend class OneMailLine;

        explicit Mail(const int &id_, const string &title_, const string &context_, const string &sender_,
                const string &receiver_, const string &timer_, const string &attachmentid_)
                :id_(id_), title_(title_), context_(context_), sender_(sender_),
                receiver_(receiver_), time_(timer_), attachmentid_(attachmentid_) {};
        explicit Mail() = default;

    public:
        int get_id() const { return id_; }
        string get_title() const { return title_; };
        string get_context() const { return context_; };
        string get_sender() const { return sender_; };
        string get_receiver() const { return receiver_; };
        string get_time() const { return time_; };
        string get_attachmentid () const { return attachmentid_; }
        string get_all() const { return "From: " + sender_ + "\tTo: " + receiver_ + "\tId: " + std::to_string(id_)
            + "\tBrief: " + context_ ; }

    private:
        int id_;
        string title_;
        string context_;
        string sender_;
        string receiver_;
        string time_;
        string attachmentid_;
    };
}

#endif // !_MAIL_H

