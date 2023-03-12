#ifndef LAT_H
#define LAT_H
#include <QString>

class Lat
{
public:
    explicit Lat(double);
    explicit Lat(char const *);
    explicit Lat(const QString&);
    QString fmt();
    static QString fmt(const Lat&);
    static QString fmt(double);
//    不对外提供格式化后的字符串转回 double 的接口。
//    如果提供上述接口会造成精度丢失，又无法避免用户滥用
private:
    double v;
};

class Lon
{
public:
    explicit Lon(double);
private:
    double v;
};

#endif // LAT_H
