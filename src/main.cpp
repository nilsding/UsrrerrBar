#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

#include "config.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    a.setApplicationName("UsrrerrBar");
    a.setApplicationDisplayName("UsrrerrBar user bar generator");
    a.setOrganizationName("rrerr.net");
    a.setOrganizationDomain("rrerr.net");
    a.setApplicationVersion(PROJECT_VERSION);

    qSetMessagePattern(
        "%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}, "
        "[%{time yyyy-MM-ddTHH:mm:ss.zzz} #%{pid}] %{type} -- %{category}: %{message}"
    );

    QFontDatabase::addApplicationFont(":/visitor.ttf");

    MainWindow w;
    w.show();
    return a.exec();
}
