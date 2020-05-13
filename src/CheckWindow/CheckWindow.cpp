#include "CheckWindow.h"

namespace Uni{
    extern mysql::Mail mail;
    extern mysql::user* user;
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

CheckWindow::CheckWindow(QWidget *parent, int order)
	: QWidget(parent)
{
	ui = new Ui::CheckWindow();
	ui->setupUi(this);
	this->order = order;
    item_model_ = new QStandardItemModel(this);
    connect(ui->listView,
        SIGNAL(doubleClicked(QModelIndex)),
        this,
        SLOT(CheckWindowClickedIndex(QModelIndex)));
}

void CheckWindow::set_content_(const std::string& sender, const std::string& title,
        const std::string& time, const std::string& content){
	ui->from->setText(QString::fromStdString(sender));
	ui->to->setText(QString::fromStdString(title));
	ui->time->setText(QString::fromStdString(time));
	ui->context->setText(QString::fromStdString(content));
}

void CheckWindow::set_attachment_(const std::vector<std::pair<std::string, std::string>>& attachment){
    /* set attachment */
    item_model_->clear();
    QStringList str_list_;
    for (auto& a: attachment) {
        auto splited_ = split_(a.first, "/");
        auto qstr_ = QString::fromStdString((*--splited_.end()));
        str_list_.append(qstr_);
        auto *item = new QStandardItem(qstr_);
        item->setSizeHint(QSize(50,60));
        item_model_->appendRow(item);
    }
    ui->listView->setModel(item_model_);
}

void CheckWindow::CheckWindowClickedDelete(){
    emit Check2Main();
    this->hide();
}

void CheckWindow::CheckWindowClickedForward(){
    emit Check2Send();
}

void CheckWindow::CheckWindowClickedReply(){
    emit Check2Send();
}

void CheckWindow::CheckWindowClickedIndex(QModelIndex i){
    /* deal with string */
    auto attachment_list_ = Uni::user->user_get_attachment(Uni::mail.get_attachmentid());
    auto attachment_ = attachment_list_[i.data().toInt()];
    auto splited_ = split_(attachment_.first, "/");
    auto qstr_ = (*--splited_.end());
    char* path = new char[SQL_BUFFER_SIZE];
    strcpy(path, __ROOTPATH__);
    strcat(path, qstr_.c_str());
    char *cstr = new char[attachment_.second.length() + 1];
    strcpy(cstr, attachment_.second.c_str());

    /* deal with file */
    if (access(path, F_OK ) == -1) {
        FILE *fp = std::fopen(path, "w+");
        fprintf(fp, "%s", cstr);
        fclose(fp);
    }

    /* open with QT*/
    char temp[SQL_BUFFER_SIZE] = "file://"; strcat(temp, path);
    QDesktopServices::openUrl(QUrl(QString::fromStdString(temp)));
    delete [] cstr;
    delete [] path;

}

void CheckWindow::MainClickedIndex(){
    set_content_(Uni::mail.get_sender(), Uni::mail.get_title(), Uni::mail.get_time(), Uni::mail.get_context());
    set_attachment_(Uni::user->user_get_attachment(Uni::mail.get_attachmentid()));

    this->show();
}

