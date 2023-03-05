#include "geo.h"
#include "secommontranslater.h"
#include <QDebug>
Lat::Lat(double)
{

}

QString Lat::fmt(double v)
{
    QString str = SeCommonTranslater::FormatLatitude(v, 0);
    qDebug() << str;
    QString str1 = SeCommonTranslater::FormatLatitude(v, 1);
    qDebug() << str1;
    return str;
}

