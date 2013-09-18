#include "app.h"
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#ifdef Q_OS_SYMBIAN
#include <e32std.h>
#include <e32base.h>
_LIT(KMyServer,"filMarkSrv.exe");
#endif
#include <QDir>
#include <QFile>
#include <QTextStream>

App::App(QObject *parent) :
    QObject(parent)
{
    int lang;
#ifdef Q_OS_SYMBIAN
    lang=User::Language();
#else
    lang=16;
#endif
    locs=new Localizer(lang);
}
int App::IsServerWorking()
{
#ifdef Q_OS_SYMBIAN

    TBuf<255> a;
    a.Append(KMyServer);
    a.Append(_L("*"));
    TBool found=EFalse;
    TRAPD(err,
            {
            TFindProcess processFinder(a); // by name, case-sensitive
            TFullName result;
            RProcess processHandle;
            while ( processFinder.Next(result) == KErrNone)
                    {
                found=ETrue;
                    }
            });
            if (found){return 1;}
            else
               {return 0;}
            return 1;
#else
    return 1;
#endif
}
void App::OtherApps()
{
    QDesktopServices::openUrl(QUrl::fromUserInput(QString::fromLocal8Bit("http://store.ovi.mobi/publisher/Alexander+Fokin")));
}
void App::Switch(int aStatus)
{
#ifdef Q_OS_SYMBIAN
    if (aStatus==0)
    {
        QFile filedel("C:/System/Apps/filMark/autostart.ini");
        if (filedel.exists()){filedel.remove();}
    }
    TRAPD(err,
            {
            TBuf<255> a;
            a.Append(KMyServer);
            a.Append(_L("*"));
            TFindProcess processFinder(a); // by name, case-sensitive
            TFullName result;
            RProcess processHandle;
            while ( processFinder.Next(result) == KErrNone)
            {
               User::LeaveIfError(processHandle.Open ( processFinder, EOwnerThread));
               processHandle.Kill(KErrNone);
               processHandle.Close();
            }
            });
if (aStatus==1)
{
    QDir dir("C:/System/Apps/filMark/");
    if (!dir.exists()){dir.mkpath("C:/System/Apps/filMark/");}
    QFile file("C:/System/Apps/filMark/autostart.ini");
    if (!file.open (QIODevice::WriteOnly)) return;
    QTextStream stream ( &file );
    stream.setCodec("UTF-8");
    stream <<"1";
    file.close(); // when your done.
    RProcess proc;
    User::LeaveIfError(proc.Create(KMyServer,_L("")));
    proc.Resume();
    proc.Close();
}
#else
    if (aStatus==1)
    {
        QString title,txt("start server");
        QMessageBox::about(0,title,txt);
    }
    else
    {
        QString title,txt("stop server");
        QMessageBox::about(0,title,txt);
    }
#endif
}

