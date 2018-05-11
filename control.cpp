#include <QMessageBox>
#include "control.h"

#include "hdd.xpm"

control::control():QWidget()
{
    setupUi(this);
    makeTray();
    flagSend=true;
    flagClose=false;
    setWindowTitle(tr("Монитор дисков"));
    setWindowIcon(QIcon(hdd_xpm));
    process = new QProcess;
    process->setProcessChannelMode(QProcess::MergedChannels);
    timer = new QTimer;
    timerBreak = new QTimer;
    connect(timer, SIGNAL(timeout()), SLOT(slotRun()));
    connect(timerBreak, SIGNAL(timeout()), SLOT(slotBreak()));
    connect(this, SIGNAL(send()), SLOT(slotSend()));
    connect(process, SIGNAL(finished(int)), SLOT(slotEnd(int)));
    slotRun();
}
void control::makeTray()
{
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(hdd_xpm));
    actHide = new QAction(tr("Скрыть"), this);
    actShow = new QAction(tr("Отобразить"), this);
    actExit = new QAction(tr("Закрыть"), this);
    connect(actHide, SIGNAL(triggered()), SLOT(hide()));
    connect(actShow, SIGNAL(triggered()), SLOT(show()));
    connect(actExit, SIGNAL(triggered()), SLOT(slotExit()));
    connect(tray, SIGNAL(messageClicked()), SLOT(show()));
    QMenu *menu = new QMenu;
    menu->addAction(actHide);
    menu->addAction(actShow);
    menu->addSeparator();
    menu->addAction(actExit);
    tray->setContextMenu(menu);
    tray->show();
}
void control::slotExit()
{
    flagClose=true;
    close();
}
void control::slotBreak()
{
    flagSend=true;
}
void control::slotRun()
{
    process->start("df --sync");
}
void control::slotEnd(int s)
{
    if(s)
    {
        timer->start(100);
        return;
    }
    QString str;
    QStringList lst;
    QTreeWidgetItem *status;
    QProgressBar *progress;
    process->readLine();
    int proc=0, i;
    treeWidget->clear();
    while(!process->atEnd())
    {
        status = new QTreeWidgetItem(treeWidget);
        str=process->readLine();
        lst=str.split(QRegExp("\\s+"));
        if(lst.count()>4)
        {
            status->setText(0, lst.at(0));
            progress = new QProgressBar;
            progress->setMinimum(0);
            progress->setMaximum(lst.at(1).toInt());
            progress->setValue(lst.at(2).toInt());
            progress->setStyleSheet("QProgressBar{border: 2px solid grey; border-radius: 2px; text-align:center;}"
                                    "QProgressBar::chunk{background-color: #05B8CC; width: 20px;}");
            progress->setFormat("%v (%m) Kb");
            progress->setMaximumHeight(20);
            status->setText(2, lst.at(4));
            status->setText(3, lst.at(5));
            str=lst.at(4);
            str=str.replace(QString("%"),QString(" "));
            i=str.toInt();
            if(proc<i)
                proc=i;
            treeWidget->setItemWidget(status, 1, progress);
        }
    }
    timer->start(1000);
    if((100-(spinBox->value()))<=proc)
        emit send();
    else
        flagSend=true;
}
void control::slotSend()
{
    if(!flagSend)
        return;
    flagSend=false;
    timerBreak->start(300000);
    tray->showMessage(tr("Опасно"), tr("На диске заканчивается свободное место"), QSystemTrayIcon::Critical, 60000);
}
