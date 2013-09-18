#ifndef APP_H
#define APP_H

#include <QObject>
#include <localizer.h>
class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);
    int IsServerWorking();
    Q_INVOKABLE void OtherApps();
    Q_INVOKABLE void Switch(int aStatus);
    Localizer* locs;
signals:

public slots:

};

#endif // APP_H
