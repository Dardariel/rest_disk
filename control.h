#ifndef CONTROL_H
#define CONTROL_H

#include <QtGui>
#include "ui_control.h"

class control : public QWidget, public Ui::Control
{
    Q_OBJECT
public:
    control();
    QSystemTrayIcon *tray;
    QAction *actHide;
    QAction *actShow;
    QAction *actExit;
    QTimer *timer;
    QTimer *timerBreak;
    QProcess *process;
    bool flagSend;
    bool flagClose;

    void makeTray();  //создание иконки в трее
public slots:
    void slotBreak();  //запуск повнорного отображения информации
    void slotRun();  //запуск консольного модуля
    void slotEnd(int s); //обработка информации от консольного модуля
    void slotSend();  //вывод предупреждающего сообщения
    void slotExit();  //выход из приложения
signals:
    void send(); //сигнал вывода предупреждающего сообщения
protected:
    void closeEvent( QCloseEvent *e ) //перехват сигнала о закрытии
    {
        if(flagClose)
        {
            e->accept();
        }
        else
        {
            hide();
            e->ignore();
        }
    }
};

#endif // CONTROL_H
