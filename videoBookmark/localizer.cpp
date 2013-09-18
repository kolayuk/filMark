#include "localizer.h"
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QStringList>

Localizer::Localizer(int language,QObject *parent) :
    QObject(parent)
{
    lang=language;
    QFile file("lang/lang.setting");
    file.open (QFile::ReadOnly);
    QTextStream stream ( &file );
    stream.setCodec("UTF-8");
    QString line;
    int j=0;
    QStringList lst;
    while( !stream.atEnd())
    {
         line = stream.readLine();
         lst=line.split(":");
         locs.insert(lst[0].toInt(),lst[1].toInt());
         j++;
    }
    file.close(); // when your done.

   lang=GetLang(lang);

   QString filename;
   filename.append("lang/strings.l");
   filename.append(QString::number(lang));

   QStringList stringlist;
   QFile locfile(filename);
   QString txt;
   if (locfile.exists())
   {
   locfile.open (QIODevice::ReadOnly);
   QTextStream locstream ( &locfile );
   locstream.setCodec("UTF-8");
   txt=locstream.readAll();
   file.close(); // when your done.
   }
   int c=txt.count();
   stringlist=txt.split("\n***\n");
   QStringList ids;
   for (int i=0;i<stringlist.count();i++)
   {
       if (stringlist[i]!="")
       {
       ids=stringlist[i].split(":::");
       strings.insert(ids[0],ids[1]);
       stringslist.append(ids[1]);
       }
   }
}
int Localizer::GetLang(int lang)
{
    const int defaultval=1;
    return locs.value(lang,defaultval);
}
QString Localizer::GetStringById(char *aName)
{
    QString name(aName);
    QString def(" ");
    return strings.value(name,def);
}

QStringList Localizer::GetStringsAsList()
{
    return stringslist;
}
