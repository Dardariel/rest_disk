#include <QtGui>
#include <QTranslator>


#include "control.h"

int main(int argc, char *argv[])
{
    QTextCodec* c = QTextCodec::codecForName("KOI8R");
    QTextCodec::setCodecForTr(c);
    QApplication a(argc, argv);
    control con;

    con.show();
    //con.hide();
    return a.exec();
}
