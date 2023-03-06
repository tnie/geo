#include "secommontranslater.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>
#include "math.h"

QString strCov = "";
// FormatDms - 格式化度数
// 参数:
//		value   - 度数，单位为度
//		degree  - 度数
//		minute  - 分数
//		dot_minute - 分数的小数部分
//		precision dot_minute的精度
// 返回值:
//		true: 正数
//		false: 负数
bool SeCommonTranslater::FormatDms(const double value,
                           uint* degree,
                           uint* minute,
                           uint* dot_minute,
                           const uint precision)
{
    const double fabsValue = fabs(value+0.00000833333333333333);

    *degree = floor(fabsValue);
    *minute = floor((fabsValue - *degree)*60);
    *dot_minute = (fabsValue*60 - (*degree)*60 - *minute)*pow((double)10, (double)precision);//add by bxy :+0.5

    return value < 0? false: true;
}

// FormatLatitude - 格式化纬度
// 参数:
//		lat - 纬度，单位为度
//		fmtMode - 0: 返回格式为"00°00'.000N"
//			- 1: 返回格式为"99°99'.999N"
//			- 2:
QString SeCommonTranslater::FormatLatitude(const double lat, const int fmtMode)
{
    if (fabs(lat) > 90)
        return QString("not in value range: %1").arg(QString::number(lat, 'f'));

    unsigned int degree = 0;
    unsigned int minute = 0;
    unsigned int dot_minute = 0;

    switch (fmtMode) {
        default:
            return QString("can't recognize: ")
                .append(QString::number(fmtMode));
        case 0: // 返回格式为"00°00'.000N"
            FormatDms(fabs(lat), &degree, &minute, &dot_minute, 3);
            return QString("0%1°%2'.%3")
                .arg(QString::number(degree))
                .arg(QString::number(minute))
                .arg(QString::number(dot_minute))
                .append(lat >=0 ? "N": "S");
        case 1: // 返回格式为"99°99'.999N"
            FormatDms(fabs(lat), &degree, &minute, &dot_minute, 3);
            char buf[32];
            memset(buf, 0, sizeof(buf));
            sprintf(buf,
                "%-2.2d°%-2.2d'.%-3.3d",
                degree, minute, dot_minute);
            return QString(buf)
                .append(lat >=0 ? "N": "S");
    }
}
//QString SeCommonTranslater::FormatLatitude(double latitude)
//{
//	QString strLat;
//	double temp=0.0;
//	if(latitude>=0)
//	{
//		temp = (latitude - int(latitude))*60;
//		strLat=QString("%1°%2'N").arg(int(latitude),3,10,QChar(' ')).arg(temp,6,'f',3,'0');
//	}
//	else{
//		latitude = -latitude;
//		temp = (latitude - int(latitude))*60;
//		strLat=QString("%1°%2'S").arg(int(latitude),3,10,QChar(' ')).arg(temp,6,'f',3,'0');
//	}
//	return strLat;
//}

// FormatLongitude - 格式化经度
// 参数:
//	lon - 经度，单位为度
//	fmtMode - 0: 返回格式为 "00°00'.000E"
//			- 1: 返回格式为"999°99'.999E"
//			- 2:
QString SeCommonTranslater::FormatLongitude(const double lon, const int fmtMode)
{
    if (fabs(lon) > 180)
        return QString("not in value range: %1").arg(QString::number(lon, 'f'));

    unsigned int degree = 0;
    unsigned int minute = 0;
    unsigned int dot_minute = 0;

    switch (fmtMode) {
        default:
            return QString("can't recognition: ")
                .append(QString::number(fmtMode));
        case 0: // 返回格式为"00°00'.000E"
            FormatDms(fabs(lon), &degree, &minute, &dot_minute, 3);
            return QString("0%1°%2'.%3")
                .arg(QString::number(degree))
                .arg(QString::number(minute))
                .arg(QString::number(dot_minute))
                .append(lon >=0 ? "E": "W");
        case 1: // 返回格式为"999°99'.999E"
            FormatDms(fabs(lon), &degree, &minute, &dot_minute, 3);
            char buf[32];
            memset(buf, 0, sizeof(buf));
            sprintf(buf,
                "%-3.3d°%-2.2d'.%-3.3d",
                degree, minute, dot_minute);
            return QString(buf)
                .append(lon >=0 ? "E": "W");
    }
}
//QString SeCommonTranslater::FormatLongitude(double longitude)
//{
//	QString strLon;
//	double temp=0.0;
//	if(longitude>=0)
//	{
//		temp = (longitude - int(longitude))*60;
//		strLon=QString("%1°%2'E").arg(int(longitude),3,10,QChar(' ')).arg(temp,6,'f',3,'0');
//	}else{
//		longitude = -longitude;
//		temp = (longitude - int(longitude))*60;
//		strLon=QString("%1°%2'W").arg(int(longitude),3,10,QChar(' ')).arg(temp,6,'f',3,'0');
//	}
//	return strLon;
//}
// GetDegreeFromQstr - 从
// 参数:
//		qstrLonLat  - 字符串格式的经纬度信息
//		fmtMode     - 0: "00°00'.000"
double SeCommonTranslater::GetDegreeFromQstr(const QString qstrLonLat, const int fmtMode)
{
    QString qstrDegree = qstrLonLat;
    switch(fmtMode) {
        default:
            //qDebug("input value wrong: %1", fmtMode);
            /*qDebug()<<QString("input value wrong: %1").arg(fmtMode);*/
            return 0.0;
            break;
        case 0: // "00°00'.000"
            {
                int pos = 0;
                QRegExp rx(strCov.fromLocal8Bit("\\d{1,3}°\\d{1,2}'.\\d{1,3}"));
                QRegExpValidator v(rx, NULL);
                if (v.validate(qstrDegree, pos) != QValidator::Acceptable) {
                    //qDebug("input value wrong: %1", qstrDegree);
                    //qDebug()<<QString("input value wrong: %1").arg(qstrDegree);
                    return 0.0;
                } else {
                    int degree = 0;
                    int minute1 = 0;
                    float minute2 = 0;
                    sscanf(qstrDegree.toLocal8Bit().data(),
                        "%d°%d'%f",
                        &degree, &minute1, &minute2);

                    return double(degree)+double(minute1)/60.0 + minute2/60.0;
                }
            }
            break;
    }
}

// GetDegreeFromLonQstr - 转换字符串格式的经纬度为双精度数
// 参数:
//		qstrLonLat  - 字符串格式的经纬度信息
//		fmtMode     - 0: "000°00'.000[EW]"
double SeCommonTranslater::GetDegreeFromLonQstr( const QString qstrLon, const int fmtMode )
{
    QString qstrDegree = qstrLon;
    switch(fmtMode) {
        default:
            //qDebug("input value wrong: %1", fmtMode);
            //qDebug()<<QString("input value wrong: %1").arg(fmtMode);

            return 0.0;
            break;
        case 0: // "000°00.000'[EW]"
            {
                int pos = 0;
                QRegExp rx(strCov.fromLocal8Bit("\\d{1,3}°\\d{1,2}'.\\d{1,3}[EW]"));
                QRegExpValidator v(rx, NULL);
                if (v.validate(qstrDegree, pos) != QValidator::Acceptable) {
                    //qDebug("input value wrong: %1", qstrDegree);
                    //qDebug()<<QString("input value wrong: %1").arg(qstrDegree);
                    return 0.0;
                } else {
                    return qstrLon.endsWith('E')? GetDegreeFromQstr(qstrDegree.remove(QChar('E')),0):
                            -GetDegreeFromQstr(qstrDegree.remove(QChar('W')),0);
                }
            }
            break;
    }
}

// GetDegreeFromLatQstr - 转换字符串格式的经纬度为双精度数
// 参数:
//		qstrLat  - 字符串格式的经纬度信息
//		fmtMode     - 0: "00°00'.000[NS]"
double SeCommonTranslater::GetDegreeFromLatQstr( const QString qstrLat, const int fmtMode )
{
    QString qstrDegree = qstrLat;
    qstrDegree = "0" + qstrDegree;
    switch(fmtMode) {
        default:
            //qDebug("input value wrong: %1", fmtMode);
            //qDebug()<<QString("input value wrong: %1").arg(fmtMode);
            return 0.0;
            break;
        case 0: // "00°00.000'[NS]"
            {
                int pos = 0;
                QRegExp rx(strCov.fromLocal8Bit("\\d{1,3}°\\d{1,2}'.\\d{1,3}[NS]"));
                QRegExpValidator v(rx, NULL);
                if (v.validate(qstrDegree, pos) != QValidator::Acceptable) {
                    //qDebug("input value wrong: %1", qstrDegree);
                    //qDebug()<<QString("input value wrong: %1").arg(qstrDegree);
                    return 0.0;
                } else {
                    return qstrLat.endsWith('N')? GetDegreeFromQstr(qstrDegree.remove(QChar('N')),0):
                            -GetDegreeFromQstr(qstrDegree.remove(QChar('S')),0);
                }
            }
            break;
    }
}

// GetDegreeFromDegreeQstr - 转换字符串格式的度数为双经度数
// 参数:
//		qstrDegree  - 字符串格式的度数(注:格式为123.45°,"°"和数字之间允许有空格)
double SeCommonTranslater::GetDegreeFromDegreeQstr( const QString qstrDegree )
{
    if (qstrDegree.isNull() || qstrDegree.isEmpty()) {
        //qDebug("input value wrong");
        //qDebug()<<QString("input value wrong");
        return -1;
    }

    QStringList list = qstrDegree.split(" ")[0].split("°");
    return list[0].toDouble();
}

// GetSpeedFromSpeedQstr - 转换字符串格式的速度为双精度数
// 参数:
//		qstrSpeed   - 字符串格式的度数(注:格式为123.45kn,"kn"和数字之间允许有空格)
double SeCommonTranslater::GetSpeedFromSpeedQstr( const QString qstrSpeed )
{
    if (qstrSpeed.isNull() || qstrSpeed.isEmpty()) {
        //qDebug("input value wrong");
        return -1;
    }

    QStringList list = qstrSpeed.split(" ")[0].split(QRegExp("c"));
    return list[0].toDouble();
}

// GetDistanceFromDistanceQstr - 转换字符串格式的距离为双精度数
// 参数
//	qstrDistance- 字符串格式的距离(注:格式为123.45nmile,"nmile"和数字之间允许有空格)
double SeCommonTranslater::GetDistanceFromDistanceQstr( const QString qstrDistance )
{
    if (qstrDistance.isNull() || qstrDistance.isEmpty()) {
        //qDebug("input value wrong");
        return -1;
    }
        //qDebug("input value wrong");

    QStringList list = qstrDistance.split(" ")[0].split(QRegExp("c"));
    return list[0].toDouble();
}

// FormatAngle  - 格式化角度
// 参数:
//		angle  - 度，单位为度
//		返回格式为"000°00.000'"

QString SeCommonTranslater::FormatAngle(const double angle)
{
    if (angle >= 360 || angle <0)
        return QString("not in value range: %1").arg(QString::number(angle, 'f'));

    unsigned int degree = 0;
    unsigned int minute = 0;
    unsigned int dot_minute = 0;

    FormatDms(fabs(angle), &degree, &minute, &dot_minute, 3);
    char buf[32];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%-3.3d°%-2.2d.%-3.3d'", degree, minute, dot_minute);
    return (buf);

}
// FormatSqlLog - 获取std::string格式的sql语句
// 参数
//		fmt - 格式
std::string SeCommonTranslater::FormatSqlLog( const char* fmt, ... )
{
    std::string strTmp(1024, 0);
    va_list args;
    va_start(args, fmt);
    vsprintf(&strTmp[0], fmt, args);
    va_end(args);
    strTmp.erase(strTmp.begin() + strTmp.find_first_of(char(0)), strTmp.end());

    return std::string(strTmp);
}

std::string SeCommonTranslater::FormatTimeRangeFromSecond(int iTime)
{
    std::string strTmp(16, 0);
    if (iTime < 0)
    {
        return strTmp;
    }
    iTime = iTime/60;
    int iDay = iTime / 1440;
    iTime -= iDay * 1440;
    int iHour = iTime/60;
    iTime -= iHour * 60;
    int iMin = iTime;
    char cBuffer[24];
    sprintf(cBuffer,"%03dd%02dh%02dm", iDay, iHour, iMin);
    return std::string(cBuffer);
}

QString SeCommonTranslater::FormatUnit(double dValue, QString sUnit, int nLength, bool bUnit, const char* sAccu )
{
    QString strUnit = "";
    QString strUnitRecord = "";
    strUnit.sprintf(sAccu, fabs(dValue));

    strUnitRecord = QString("%1").arg(strUnit, nLength, QLatin1Char('0'));

    if (bUnit)
    {
        if (dValue < 0.0)
        {
            strUnit = "-" + strUnitRecord;
        }
        else
        {
            strUnit = " " + strUnitRecord;
        }
    }
    else
    {
        strUnit = strUnitRecord;
    }
    return (strUnit + sUnit);
}
