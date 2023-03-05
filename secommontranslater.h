#ifndef SECOMMONTRANSLATER_H
#define SECOMMONTRANSLATER_H

#include <QString>
class SeCommonTranslater{
public:
    SeCommonTranslater() {}

    // FormatLongitude - 格式化经度
    // 参数:
    //      lon - 经度，单位为度
    //      fmtMode - 0: 返回格式为"000°00.000'E"
    //              - 1: 返回格式为"999°99.999'E"
    //              - 2:
    static QString FormatLongitude(const double lon, const int fmtMode = 1);
    // GetDegreeFromLonQstr - 转换字符串格式的经纬度为双精度数
    // 参数:
    //      qstrLon  - 字符串格式的经纬度信息
    //      fmtMode     - 0: "00°00.000'[EW]"
    static double GetDegreeFromLonQstr(const QString qstrLon, const int fmtMode = 0);

    // FormatLatitude - 格式化纬度
    // 参数:
    //      lat - 纬度，单位为度
    //      fmtMode - 0: 返回格式为"00°00.000'N"
    //              - 1: 返回格式为"99°99.999'N"
    //              - 2:
    static QString FormatLatitude(const double lat, const int fmtMode = 1);
    // GetDegreeFromLatQstr - 转换字符串格式的经纬度为双精度数
    // 参数:
    //      qstrLat  - 字符串格式的经纬度信息
    //      fmtMode     - 0: "00°00.000'[NS]"
    static double GetDegreeFromLatQstr(const QString qstrLat, const int fmtMode = 0);

    // FormatDegree  - 格式化角度
    // 参数:
    //		angle  - 度，单位为度
    //      返回格式为"000°00.000'"
    static QString FormatAngle(const double angle);
    // GetDegreeFromQstr - 转换字符串格式的经纬度为双精度数
    // 参数:
    //      qstrLonLat  - 字符串格式的经纬度信息
    //      fmtMode     - 0: "00°00.000'"
    static double GetDegreeFromQstr(const QString qstrLonLat, const int fmtMode = 0);



    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // GetSpeedFromSpeedQstr - 转换字符串格式的速度为双精度数
    // 参数:
    //      qstrSpeed   - 字符串格式的度数(注:格式为123.45kn,"kn"和数字之间允许有空格)
    static double GetSpeedFromSpeedQstr(const QString qstrSpeed);
    // GetDistanceFromDistanceQstr - 转换字符串格式的距离为双精度数
    // 参数
    //      qstrDistance- 字符串格式的距离(注:格式为123.45nmile,"nmile"和数字之间允许有空格)
    static double GetDistanceFromDistanceQstr(const QString qstrDistance);


    // GetDegreeFromDegreeQstr - 转换字符串格式的度数为双经度数
    // 参数:
    //      qstrDegree  - 字符串格式的度数(注:格式为123.45°,"°"和数字之间允许有空格)
    static double GetDegreeFromDegreeQstr(const QString qstrDegree);

    // FormatDms - 格式化度数
    // 参数:
    //      value   - 度数，单位为度
    //      degree  - 度数
    //      minute  - 分数
    //      dot_minute - 分数的小数部分
    //      precision dot_minute的精度
    // 返回值:
    //      true: 正数
    //      false: 负数
    static bool FormatDms(const double value,
        uint* degree,
        uint* minute,
        uint* dot_minute,
        const uint precision);


    /*static QString FormatLatitude(double latitude);
    static QString FormatLongitude(double lon);*/

    // FormatSqlLog - 获取std::string格式的sql语句
    // 参数
    //      fmt - 格式
    static std::string FormatSqlLog(const char* fmt, ...);

    /** 格式化以秒为单位的时间为ddhhmm
    */
    static std::string FormatTimeRangeFromSecond(int iTime);

    /** 格式化单位显示
    dValue-数值
    sUnit-单位
    nLength-总长度，不包含正负符号
    bUnit-是否包含正负符号单位
    sAccu-显示精度
    */
    static QString FormatUnit(double dValue, QString sUnit, int nLength, bool bUnit, const char* sAccu = "%.3f");
};

#endif // SECOMMONTRANSLATER_H
