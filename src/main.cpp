/*-----------------------------------本模块是主模块，实现所有模块的串联----------------------------------------------------------------------*/
/*-----------------------------------作者：黄士诚--------------------------------------------------------------------------------------------*/
#include "src/include/main.h"

namespace Uni {
	sendWindow *e;
	MainWindow *w;
	CreateUserWindow *c;
	ReceiveWindowFinal *r;
	CheckWindow *h;
	logWindow *l;
}

static void init_style_(){
    //加载样式表
    QFile file("../img/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

static void init_translator_(){
    //加载鼠标右键菜单翻译文件
    auto *translator1 = new QTranslator(qApp);
    translator1->load("../img/image/qt_zh_CN.qm");
    qApp->installTranslator(translator1);

    //加载富文本框鼠标右键菜单翻译文件
    auto *translator2 = new QTranslator(qApp);
    translator2->load("../img/image/widgets.qm");
    qApp->installTranslator(translator2);
}

static void register_signals_(){
    /* main <-> send */
    QObject::connect(
            Uni::w,
            SIGNAL(Main2Send()), // click send
            Uni::e,
            SLOT(MainClickedSend())
    );
    QObject::connect(
            Uni::e,
            SIGNAL(Send2Main()),
            Uni::w,
            SLOT(SendClickedBack())
    );

    /* main <-> check */
    QObject::connect(
        Uni::w,
        SIGNAL(Main2Check()),
        Uni::h,
        SLOT(MainClickedIndex())
    );
    QObject::connect(
        Uni::h,
        SIGNAL(Check2Main()),
        Uni::w,
        SLOT(CheckClickedDelete())
    );


    /* main -> receive */
    QObject::connect(
        Uni::w,
        SIGNAL(Main2Receive()),
        Uni::r,
        SLOT(getSigFromMainClickedReceive())
    );


    /* main <-> log */
    QObject::connect(
        Uni::w,
        SIGNAL(Main2Log()),
        Uni::l,
        SLOT(MainClickedLogout())
    );
    QObject::connect(
        Uni::l,
        SIGNAL(Log2Main()),
        Uni::w,
        SLOT(LogClickedLog())
    );

    /* create <-> log */
    QObject::connect(
        Uni::c,
        SIGNAL(Create2Log()),
        Uni::l,
        SLOT(CreateClickedBack())
    );
    QObject::connect(
        Uni::l,
        SIGNAL(Log2Create()),
        Uni::c,
        SLOT(LogClickedCreate())
    );

    /* check -> send */
    QObject::connect(
        Uni::h,
        SIGNAL(Check2Send()),
        Uni::e,
        SLOT(CheckClickedReply())
    );

}

int main(int argc, char *argv[]){
    /* all */
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("../img/main.ico"));
    init_style_();
    init_translator_();

    /* init windows */
	Uni::w = new MainWindow;
    Uni::e = new sendWindow;
    Uni::c = new CreateUserWindow;
    Uni::r = new ReceiveWindowFinal;
    Uni::l = new logWindow;
    Uni::h = new CheckWindow;
    Uni::w->setWindowTitle("Email");
	Uni::l->show();

    /* register signals */
    register_signals_();

    return a.exec();
}
