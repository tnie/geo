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
