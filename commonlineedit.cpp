//#include "Header_pch.h"
#include <QDate>
#include "secommontranslater.h"
#include "commonlineedit.h"
#include "qevent.h"
#include "qdebug.h"

//	经度模式编辑框
//	输入范围 000°00'.000[W] 到 179°59'.999[E]
SeCommonLineEditLon::~SeCommonLineEditLon(void){
    disconnect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    delete validator;
}
SeCommonLineEditLon::SeCommonLineEditLon(QWidget* parent):
BaseLineEdit(parent)
{
    m_bIsInEdit = false;
    m_bState = true;//true is valid ,false is unvalid
    m_bEditState = false;//true is infocus， false is outfocus
    m_bChangeState = false;//true is changed, false is not changed
    m_iRow = -1;
    m_iColumn = -1;
    m_strUnValidText = QString::fromLocal8Bit("---°--'.----");
    QRegExp rxAngel(strCov.fromLocal8Bit("[0-1]\\d{2,2}°[0-5]'\\d.\\d{3,3}[ewEW]"));
    validator = new QRegExpValidator(rxAngel, this);
    setValidator(validator);
    setInputMask(strCov.fromLocal8Bit("000°00'.000A"));
    setText(strCov.fromLocal8Bit("000°00'.000E"));
    setAlignment(Qt::AlignRight);
    connect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
}
void SeCommonLineEditLon::inputText(double dLon)
{
    if (!m_bState)
    {
        return ;
    }
    this->setText(SeCommonTranslater::FormatLongitude(dLon, 1));
}
double SeCommonLineEditLon::getDegree(){
    if (!m_bState)
    {
        return 0.0;
    }
    return SeCommonTranslater::GetDegreeFromLonQstr(this->text(), 0);
}
double SeCommonLineEditLon::getRad(){
    if (!m_bState)
    {
        return 0.0;
    }
    return SeCommonTranslater::GetDegreeFromLonQstr(this->text(), 0)*3.1415926/180;
}

void SeCommonLineEditLon::textEditing(const QString& text){
    if (!m_bState && !m_bEditState)
    {
        return ;
    }
    if (!m_bState && m_bEditState)
    {
        m_bChangeState = true;
    }
    int cursorPos = cursorPosition();
    if (text.size() != 12)
    {
        return;
    }
    if(cursorPos==1 && text.left(1).toInt()>1){
        undo();
        return;
    }
    if(cursorPos==1 && text.left(2).toInt()>17){
        undo();
        return;
    }
    if(cursorPos==2 && text.left(2).toInt()>17){
        undo();
        return;
    }
    if(cursorPos==12 && text.at(11).isLower()){
        QString str = text;
        str.replace(QString("e"), QString("E"));
        str.replace(QString("w"), QString("W"));
        setText(str);
        return;
    }
    if(cursorPos==1){	// 防止第一个数字也被删除
        this->setCursorPosition(2);
        this->setCursorPosition(1);
    }
}
void SeCommonLineEditLon::SetValid( bool bState, QString strText)
{
    if (m_bState == bState || m_bEditState)
    {
        return;
    }
    m_strUnValidText = strText;
    m_bState = bState;
    if (bState)
    {
        //valid
        QRegExp rxAngel(strCov.fromLocal8Bit("[0-1]\\d{2,2}°[0-5]\\d'.\\d{3,3}[ewEW]"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("000°00'.000A"));
        setText(strCov.fromLocal8Bit("000°00'.000E"));
    }
    else
    {
        //unvalid
        QRegExp rxAngel(strCov.fromLocal8Bit("------------"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("XXXXXXXXXXXX"));
        setText(strText);
    }
}

void SeCommonLineEditLon::focusInEvent( QFocusEvent * e )
{
    m_bIsInEdit = true;
    BaseLineEdit::focusInEvent(e);
    if (this->isReadOnly() || !(this->isEnabled()) || m_bState)
    {
        return;
    }
    //valid
    m_bEditState = true;
    QRegExp rxAngel(strCov.fromLocal8Bit("[0-1]\\d{2,2}°[0-5]\\d'.\\d{3,3}[ewEW]"));
    if (NULL != validator)
    {
        delete validator;
        validator = NULL;
    }
    validator = new QRegExpValidator(rxAngel, this);
    setValidator(validator);
    setInputMask(strCov.fromLocal8Bit("000°00'.000A"));
    setText(strCov.fromLocal8Bit("000°00'.000E"));
    m_bChangeState = false;
}

void SeCommonLineEditLon::focusOutEvent( QFocusEvent * e )
{
    m_bIsInEdit = false;
    BaseLineEdit::focusOutEvent(e);
    if(!m_bState && m_bChangeState)
    {
        m_bState = true;
    }
    else if (!m_bState)
    {
        QRegExp rxAngel(strCov.fromLocal8Bit("------"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("XXXXXX"));
        setText(m_strUnValidText);
    }
    m_bEditState = false;
    m_bChangeState = false;
    if (m_bState && m_iRow != -1 && m_iColumn != -1)
    {
        emit textFinishEdit(m_iRow, m_iColumn, getDegree());
    }
    else
    {
        emit FinishEdit(getDegree());
    }
}

void SeCommonLineEditLon::keyPressEvent( QKeyEvent * event )
{
    BaseLineEdit::keyPressEvent(event);
    int iKey = event->key();
    if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
    {
        QFocusEvent x(QEvent::FocusOut);
        this->focusOutEvent(&x);
        this->focusNextChild();
    }
}

//纬度模式编辑框
//	输入范围 00°00'.000[S] 到 89°59'.999[N]
SeCommonLineEditLat::~SeCommonLineEditLat(void){
    disconnect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    delete validator;
}
SeCommonLineEditLat::SeCommonLineEditLat(QWidget* parent):
BaseLineEdit(parent)
{
    m_bIsInEdit = false;
    m_bState = true;//true is valid ,false is unvalid
    m_bEditState = false;//true is infocus， false is outfocus
    m_bChangeState = false;//true is changed, false is not changed
    m_iRow = -1;
    m_iColumn = -1;
    m_strUnValidText = QString::fromLocal8Bit("--°--'.----");
    QRegExp rxAngel(strCov.fromLocal8Bit("[0-8]\\d°[0-5]\\d'.\\d{3,3}[SNsn]"));
    validator = new QRegExpValidator(rxAngel, this);
    setValidator(validator);
    setInputMask(strCov.fromLocal8Bit("00°00'.000A"));
    setText(strCov.fromLocal8Bit(("00°00'.000N")));
    setAlignment(Qt::AlignRight);
    connect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
}
void SeCommonLineEditLat::inputText( double dLat )
{
    if (!m_bState)
    {
        return ;
    }
    this->setText(SeCommonTranslater::FormatLatitude(dLat, 1));
}
double SeCommonLineEditLat::getDegree()
{
    if (!m_bState)
    {
        return 0.0;
    }
    return SeCommonTranslater::GetDegreeFromLatQstr(this->text(), 0);
}
double SeCommonLineEditLat::getRad(){
    if (!m_bState)
    {
        return 0.0;
    }
    return SeCommonTranslater::GetDegreeFromLatQstr(this->text(), 0)*3.1415926/180;
}

void SeCommonLineEditLat::textEditing(const QString& text){
    if (!m_bState && !m_bEditState)
    {
        return ;
    }
    if (!m_bState && m_bEditState)
    {
        m_bChangeState = true;
    }
    int cursorPos = cursorPosition();
    if (text.size() != 11)
    {
        return;
    }
    if(cursorPos==11 && text.at(10).isLower()){
        QString str = text;
        str.replace(QString("s"), QString("S"));
        str.replace(QString("n"), QString("N"));
        setText(str);
    }
}
void SeCommonLineEditLat::SetValid( bool bState, QString strText )
{
    if (m_bState == bState || m_bEditState)
    {
        return;
    }
    m_strUnValidText = strText;
    m_bState = bState;
    if (bState)
    {
        //valid
        QRegExp rxAngel(strCov.fromLocal8Bit("[0-8]\\d°[0-5]\\d'.\\d{3,3}[SNsn]"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("00°00'.000A"));
        setText(strCov.fromLocal8Bit("00°00'.000N"));
    }
    else
    {
        //unvalid
        QRegExp rxAngel(strCov.fromLocal8Bit("-----------"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("XXXXXXXXXXX"));
        setText(strText);
    }
}

void SeCommonLineEditLat::focusInEvent( QFocusEvent * e )
{
    m_bIsInEdit = true;
    BaseLineEdit::focusInEvent(e);
    if (this->isReadOnly() || !(this->isEnabled()) || m_bState)
    {
        return;
    }
    //valid
    m_bEditState = true;
    QRegExp rxAngel(strCov.fromLocal8Bit("[0-8]\\d°[0-5]\\d'.\\d{3,3}[SNsn]"));
    if (NULL != validator)
    {
        delete validator;
        validator = NULL;
    }
    validator = new QRegExpValidator(rxAngel, this);
    setValidator(validator);
    setInputMask(strCov.fromLocal8Bit("00°00'.000A"));
    setText(strCov.fromLocal8Bit("00°00'.000N"));
    m_bChangeState = false;
}

void SeCommonLineEditLat::focusOutEvent( QFocusEvent * e )
{
    m_bIsInEdit = false;
    BaseLineEdit::focusOutEvent(e);
    if(!m_bState && m_bChangeState)
    {
        m_bState = true;
    }
    else if (!m_bState)
    {
        QRegExp rxAngel(strCov.fromLocal8Bit("-----------"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("XXXXXXXXXXX"));
        setText(m_strUnValidText);
    }
    m_bEditState = false;
    m_bChangeState = false;
    if (m_bState && m_iRow != -1 && m_iColumn != -1)
    {
        emit textFinishEdit(m_iRow, m_iColumn, getDegree());
    }
    else
    {
        emit FinishEdit(getDegree());
    }
}

void SeCommonLineEditLat::keyPressEvent( QKeyEvent * event )
{
    //int cursorPos = cursorPosition();
    BaseLineEdit::keyPressEvent(event);
    //cursorPos = cursorPosition();
    int iKey = event->key();
    if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
    {
        QFocusEvent x(QEvent::FocusOut);
        this->focusOutEvent(&x);
        this->focusNextChild();
    }
}

//	角模式编辑框
//	输入范围 000°00.000' 到 359°59.999'
SeCommonLineEditDegree::~SeCommonLineEditDegree(void){
    disconnect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    delete validator;
}
SeCommonLineEditDegree::SeCommonLineEditDegree(QWidget* parent):BaseLineEdit(parent){
    m_bIsInEdit = false;
    m_bState = true;//true is valid ,false is unvalid
    m_bEditState = false;//true is infocus， false is outfocus
    m_bChangeState = false;//true is changed, false is not changed
    m_iRow = -1;
    m_iColumn = -1;
    m_strUnValidText = QString::fromLocal8Bit("---°--.---'");
    QRegExp rxAngel(strCov.fromLocal8Bit("[0-3]\\d{2,2}°[0-5]\\d.\\d{3,3}'"));
    validator = new QRegExpValidator(rxAngel, this);
    setValidator(validator);
    setInputMask(strCov.fromLocal8Bit("000°00.000'"));
    setText(strCov.fromLocal8Bit("000°00.000'"));
    setAlignment(Qt::AlignRight);
    connect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
}
double SeCommonLineEditDegree::getDegree()
{
    if (!m_bState)
    {
        return 0.0;
    }
    QString strText = this->text();
    if (strText.size() == 11)
    {
        double dDreenInfo = 0.0;
        QChar QDreen[4];
        int iDreen = 0;
        QDreen[0] = strText.at(0);
        QDreen[1] = strText.at(1);
        QDreen[2] = strText.at(2);
        QDreen[3] = 0;
        QString strInfo(QDreen);
        iDreen = strInfo.toInt();
        QChar QMin[3];
        int iMin = 0;
        QMin[0] = strText.at(4);
        QMin[1] = strText.at(5);
        QMin[2] = 0;
        QString strInfoMin(QMin);
        iMin = strInfoMin.toInt();

        QChar QFloat[4];
        int iFloat = 0;
        QFloat[0] = strText.at(7);
        QFloat[1] = strText.at(8);
        QFloat[2] = strText.at(9);
        QFloat[3] = 0;
        QString strInfoFloat(QFloat);
        iFloat = strInfoFloat.toDouble();

        dDreenInfo = iDreen + iMin /60.0 + iFloat/1000.0/60.0;
        if (dDreenInfo < 0.000002)
        {
            return 0.0;
        }
        else if (dDreenInfo > 359.99998)
        {
            return 359.99998;
        }
        return dDreenInfo;
    }
    return 0.0;
}
double SeCommonLineEditDegree::getRad(){
    if (!m_bState)
    {
        return 0.0;
    }
    return SeCommonTranslater::GetDegreeFromDegreeQstr(this->text())*3.1415926/180;
}

void SeCommonLineEditDegree::textEditing(const QString& text){
    if (!m_bState && !m_bEditState)
    {
        return ;
    }
    if (!m_bState && m_bEditState)
    {
        m_bChangeState = true;
    }
    int cursorPos = cursorPosition();
    if(cursorPos==2 && text.left(2).toInt()>35){
        undo();
        return;
    }
    if(cursorPos==1){	// 防止第一个数字也被删除
        this->setCursorPosition(2);
        this->setCursorPosition(1);
    }
}

void SeCommonLineEditDegree::inputText( double dAngle )
{
    if (!m_bState)
    {
        return ;
    }
    this->setText(SeCommonTranslater::FormatAngle(dAngle));
}

void SeCommonLineEditDegree::SetValid( bool bState, QString strText)
{
    if (m_bState == bState || m_bEditState)
    {
        return;
    }
    m_strUnValidText = strText;
    m_bState = bState;
    if (bState)
    {
        //valid
        QRegExp rxAngel(strCov.fromLocal8Bit("[0-3]\\d{2,2}°[0-5]\\d.\\d{3,3}'"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("000°00.000'"));
        setText(strCov.fromLocal8Bit("000°00.000'"));
    }
    else
    {
        //unvalid
        QRegExp rxAngel(strCov.fromLocal8Bit("-----------"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("XXXXXXXXXXX"));
        setText(strText);
    }
}

void SeCommonLineEditDegree::focusInEvent( QFocusEvent * e )
{
    m_bIsInEdit = true;
    BaseLineEdit::focusInEvent(e);
    if (this->isReadOnly() || !(this->isEnabled()) || m_bState)
    {
        return;
    }
    //valid
    m_bEditState = true;
    QRegExp rxAngel(strCov.fromLocal8Bit("[0-3]\\d{2,2}°[0-5]\\d.\\d{3,3}'"));
    if (NULL != validator)
    {
        delete validator;
        validator = NULL;
    }
    validator = new QRegExpValidator(rxAngel, this);
    setValidator(validator);
    setInputMask(strCov.fromLocal8Bit("000°00.000'"));
    setText(strCov.fromLocal8Bit("000°00.000'"));
    m_bChangeState = false;
}

void SeCommonLineEditDegree::focusOutEvent( QFocusEvent * e )
{
    m_bIsInEdit = false;
    BaseLineEdit::focusOutEvent(e);
    if(!m_bState && m_bChangeState)
    {
        m_bState = true;
    }
    else if (!m_bState)
    {
        QRegExp rxAngel(strCov.fromLocal8Bit("-----------"));
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask(strCov.fromLocal8Bit("XXXXXXXXXXX"));
        setText(m_strUnValidText);
    }
    m_bEditState = false;
    m_bChangeState = false;
    if (m_bState && m_iRow != -1 && m_iColumn != -1)
    {
        emit textFinishEdit(m_iRow, m_iColumn, getDegree());
    }
    else
    {
        emit FinishEdit(getDegree());
    }
}

void SeCommonLineEditDegree::keyPressEvent( QKeyEvent * event )
{
    BaseLineEdit::keyPressEvent(event);
    int iKey = event->key();
    if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
    {
        QFocusEvent x(QEvent::FocusOut);
        this->focusOutEvent(&x);
        this->focusNextChild();
    }
}

#define KNFACTOR 1.0
#define KPFACTOR 1.852
#define MSFACTOR 0.51444
static QMap<QString, double> *pMapAllUnit = NULL;
static int iPMapAllUnitCount = 0;
static QString strUnitValue = "kn";
static int iMaxValue = 99;
CLineEditSpeed::CLineEditSpeed( QWidget* parent):
BaseLineEdit(parent)
{
    m_bIsInEdit = false;
    m_bNeTag = false;
    m_bState = true;//true is valid ,false is unvalid
    m_bEditState = false;//true is infocus， false is outfocus
    m_bChangeState = false;//true is changed, false is not changed
    m_iRow = -1;
    m_iColumn = -1;
    m_strUnValidText = "-";
    validator = NULL;
    iPMapAllUnitCount++;
    if (NULL == pMapAllUnit)
    {
        pMapAllUnit = new QMap<QString, double>;
        pMapAllUnit->insert("kn", KNFACTOR);
        pMapAllUnit->insert("kp", KPFACTOR);
        pMapAllUnit->insert("m/s", MSFACTOR);
    }
    strUnitInfo = strUnitValue;
    iMax = iMaxValue;
    GetRegExp();
    setAlignment(Qt::AlignRight);
    inputText(0.0);
    connect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    connect( this, SIGNAL(SignUnitUpdate(QString&)), this, SLOT(slot_LineUnitUpdate(QString&)));
}
CLineEditSpeed::~CLineEditSpeed( void )
{
    disconnect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    disconnect(this, SIGNAL(SignUnitUpdate(QString&)), this, SLOT(slot_LineUnitUpdate(QString&)));
    iPMapAllUnitCount--;
    if (iPMapAllUnitCount ==0 && NULL != pMapAllUnit)
    {
        delete pMapAllUnit;
        pMapAllUnit = NULL;
    }

    if (NULL != validator)
    {
        delete validator;
        validator = NULL;
    }
}
void CLineEditSpeed::textEditing(const QString& text){
    if (!m_bState && !m_bEditState)
    {
        return ;
    }
    if (!m_bState && m_bEditState)
    {
        m_bChangeState = true;
    }
    int cursorPos = cursorPosition();
    QMap<QString, double>::iterator ItInfo = pMapAllUnit->find(strUnitInfo);
    if (ItInfo != pMapAllUnit->end())
    {
        int iCopy = iMax * ItInfo.value();
        QString strText = text;
        strText.remove(strUnitInfo);
        double dTextValue = strText.toDouble();
        if (abs((int)dTextValue) >= iCopy)
        {
            undo();
            return;
        }
    }
    if(cursorPos==1){	// 防止第一个数字也被删除
        this->setCursorPosition(2);
        this->setCursorPosition(1);
    }
}
void CLineEditSpeed::setRange(int iMaxInput)
{
    if (0 < iMaxInput)
    {
        iMax = iMaxInput;
    }
}
double CLineEditSpeed::getDegree()
{
    if (!m_bState)
    {
        return 0.0;
    }
    QMap<QString, double>::iterator ItInfo = pMapAllUnit->find(strUnitInfo);
    if (ItInfo != pMapAllUnit->end())
    {
        QString strText = text();
        strText.remove(strUnitInfo);
        double dTextValue = strText.toDouble();

        return dTextValue / ItInfo.value();
    }
    else
    {
        return 0.0;
    }
}
void CLineEditSpeed::inputText(double dValue)
{
    if (!m_bState)
    {
        return ;
    }
    QMap<QString, double>::iterator ItInfo = pMapAllUnit->find(strUnitInfo);
    if (ItInfo != pMapAllUnit->end())
    {
        if (abs((int)dValue) >=  iMax)
        {
            if (dValue > 0)
            {
                dValue = iMax - 0.0001;
            }
            else
            {
                dValue = 0.0001 - iMax;
            }
        }

        dValue *= ItInfo.value();
        QString strInputInfo = SeCommonTranslater::FormatUnit(dValue, strUnitInfo, strInputFormat, m_bNeTag);
        this->setText(strInputInfo);
    }
}
void CLineEditSpeed::unitUpdate(QString strUnit)
{
    QMap<QString, double>::iterator ItInfo = pMapAllUnit->find(strUnit);
    if (ItInfo != pMapAllUnit->end())
    {
        double dValue = getDegree();
        strUnitValue = strUnit;
        strUnitInfo = strUnit;
        if (!m_bState)
        {
            return ;
        }
        else
        {
            GetRegExp();
            inputText(dValue);
        }
    }
}

void CLineEditSpeed::SetValid( bool bState, QString strText )
{
    if (m_bState == bState || m_bEditState)
    {
        return;
    }
    m_strUnValidText = strText;
    m_bState = bState;
    if (bState)
    {
        //valid
        GetRegExp();
        inputText(0.0);
    }
    else
    {
        //unvalid
        QRegExp rxAngel("-");
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask("XXXXX");
        setText(strText);
    }
}

void CLineEditSpeed::focusInEvent( QFocusEvent * e )
{
    m_bIsInEdit = true;
    BaseLineEdit::focusInEvent(e);
    if (this->isReadOnly() || !(this->isEnabled()) || m_bState)
    {
        return;
    }
    //valid
    m_bEditState = true;
    //valid
    GetRegExp();

    QString strInputInfo = SeCommonTranslater::FormatUnit(0.0, strUnitInfo, strInputFormat, true);
    this->setText(strInputInfo);
    m_bChangeState = false;
}

void CLineEditSpeed::focusOutEvent( QFocusEvent * e )
{
    m_bIsInEdit = false;
    BaseLineEdit::focusOutEvent(e);
    if(!m_bState && m_bChangeState)
    {
        m_bState = true;
    }
    else if (!m_bState)
    {
        //unvalid
        QRegExp rxAngel("-");
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask("XXXXX");
        setText(m_strUnValidText);
    }
    m_bEditState = false;
    m_bChangeState = false;
    if (m_bState && m_iRow != -1 && m_iColumn != -1)
    {
        emit textFinishEdit(m_iRow, m_iColumn, getDegree());
    }
    else
    {
        emit FinishEdit(getDegree());
    }
}

void CLineEditSpeed::keyPressEvent( QKeyEvent * event )
{
    BaseLineEdit::keyPressEvent(event);
    int iKey = event->key();
    if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
    {
        QFocusEvent x(QEvent::FocusOut);
        this->focusOutEvent(&x);
        this->focusNextChild();
    }
}

void CLineEditSpeed::GetRegExp()
{
    QMap<QString, double>::iterator ItInfo = pMapAllUnit->find(strUnitInfo);
    if (ItInfo != pMapAllUnit->end())
    {
        int iCopy = iMax * ItInfo.value();
        QString strAdd1 = "";
        QString strAdd2 = "";
        QString strMask = "";
        strInputFormat = 4;
        int iCount = 0;

        if (m_bNeTag)
        {
            strAdd1.append("[- ?]");
            strMask.append("X");
        }

        while (0 != iCopy / 10)
        {
            iCopy /= 10;
            iCount++;
            strMask.append("0");
        }
        strMask.append("0");
        strAdd1.append("[0-").append(QString::number(iCopy)).append("]");
        if (0 != iCount)
        {
            strAdd2.append("\\d{").append(QString::number(iCount)).append(",").append

                (QString::number(iCount)).append("}");
            strAdd1.append(strAdd2);
            strInputFormat = iCount + 5;
        }
        strAdd1.append(".\\d{3,3}");
        strMask.append(".000");
        strAdd1.append(strUnitInfo);

        QString sUnitInfo = "";
        int iAdd = strUnitInfo.size();
        for (int ibegin = 0; ibegin < iAdd; ibegin++)
        {
            sUnitInfo.append("X");
        }

        QRegExp rxAngel(strAdd1);
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        strMask.append(sUnitInfo);
        setInputMask(strMask);
    }
}

void CLineEditSpeed::SetNormalUnit()
{
    QMap<QString, double>::iterator ItInfo = pMapAllUnit->find("kn");
    if (ItInfo != pMapAllUnit->end())
    {
        double dValue = getDegree();
        strUnitInfo = "kn";
        if (!m_bState)
        {
            return ;
        }
        else
        {
            GetRegExp();
            inputText(dValue);
        }
    }
}

void CLineEditSpeed::slot_LineUnitUpdate( QString& sUnit )
{
    unitUpdate(sUnit);
}

#define NMDISFACTOR 1.0
#define CBLDISFACTOR 10
#define KMDISFACTOR 1.852
#define FTDISFACTOR 6076.1155
#define MILDISFACTOR 1.15078
#define MMMDISFACTOR 1852.
static QMap<QString, double> *pMapAllDisUnit = NULL;
static int pMapAllDisUnitCount = 0;
static QString strDisUnitValue = "NM";
//static int iDisMaxValue = 500000;
CLineEditDistance::CLineEditDistance(QWidget* parent, int maxVal):
BaseLineEdit(parent)
{
    bPress_changeUnit = false;
    m_bState = true;//true is valid ,false is unvalid
    m_bEditState = false;//true is infocus， false is outfocus
    m_bChangeState = false;//true is changed, false is not changed
    m_iRow = -1;
    m_iColumn = -1;
    m_strUnValidText = "-";
    validator = NULL;
    pMapAllDisUnitCount++;
    if (NULL == pMapAllDisUnit)
    {
        pMapAllDisUnit = new QMap<QString, double>;
        pMapAllDisUnit->insert("NM", NMDISFACTOR);
        pMapAllDisUnit->insert("Cab", CBLDISFACTOR);
        pMapAllDisUnit->insert("km", KMDISFACTOR);
        pMapAllDisUnit->insert("ft", FTDISFACTOR);
        pMapAllDisUnit->insert("mil", MILDISFACTOR);
        pMapAllDisUnit->insert("m", MMMDISFACTOR);
    }
    strUnitInfo = strDisUnitValue;
    iMax = maxVal;
    GetRegExp();
    setAlignment(Qt::AlignRight);
    connect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    connect( this, SIGNAL(SignUnitUpdate(QString&)), this, SLOT(slot_LineUnitUpdate(QString&)));
    inputText(0.0);

    unitUpdate("Cab");
}
void CLineEditDistance::SetValid( bool bState, QString strText )
{
    if (m_bState == bState || m_bEditState)
    {
        return;
    }
    m_strUnValidText = strText;
    m_bState = bState;
    if (bState)
    {
        //valid
        GetRegExp();
        inputText(0.0);
    }
    else
    {
        //unvalid
        QRegExp rxAngel("-");
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask("XXXXX");
        setText(strText);
    }
}

void CLineEditDistance::focusInEvent( QFocusEvent * e )
{
    BaseLineEdit::focusInEvent(e);
    if (this->isReadOnly() || !(this->isEnabled()) || m_bState)
    {
        return;
    }
    //valid
    m_bEditState = true;
    //valid

    GetRegExp();
    QString strInputInfo = SeCommonTranslater::FormatUnit(0.0, strUnitInfo, strInputFormat, false);
    this->setText(strInputInfo);

    m_bChangeState = false;
}

void CLineEditDistance::focusOutEvent( QFocusEvent * e )
{
    BaseLineEdit::focusOutEvent(e);
    if(!m_bState && m_bChangeState)
    {
        m_bState = true;
    }
    else if (!m_bState)
    {
        //unvalid
        QRegExp rxAngel("-");
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask("XXXXX");
        setText(m_strUnValidText);
    }
    m_bEditState = false;
    m_bChangeState = false;
    if (m_bState && m_iRow != -1 && m_iColumn != -1)
    {
        emit textFinishEdit(m_iRow, m_iColumn, getDegree());
    }
    else
    {
        emit FinishEdit(getDegree());
    }
}

void CLineEditDistance::mouseDoubleClickEvent(QMouseEvent *)
{
    int iSelect = cursorPosition();
    QString strText = text();
    strText.remove(strUnitInfo);
    if(iSelect >= strText.length())
    {
        if (strUnitInfo == tr("NM"))
        {
            unitUpdate(tr("Cab"));
            bPress_changeUnit = true;
            setSelection(text().length(), 0);
        }
        else
        {
            unitUpdate(tr("NM"));
            bPress_changeUnit = true;
            setSelection(text().length(), 0);
        }

        /*QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
        if (ItInfo != pMapAllDisUnit->end())
        {
            ItInfo++;
            if (ItInfo != pMapAllDisUnit->end())
            {
                unitUpdate(ItInfo.key());
                bPress_changeUnit = true;
                setSelection(text().length(), 0);
            }
            else
            {
                ItInfo = pMapAllDisUnit->begin();
                unitUpdate(ItInfo.key());
                bPress_changeUnit = true;
                setSelection(text().length(), 0);
            }
        }*/
    }
}

void CLineEditDistance::mousePressEvent(QMouseEvent *e)
{
    BaseLineEdit::mousePressEvent(e);
    int iSelect = cursorPosition();
    QString strText = text();
    strText.remove(strUnitInfo);
    if(iSelect >= strText.length())
    {
        if (strUnitInfo == tr("NM"))
        {
            unitUpdate(tr("Cab"));
            bPress_changeUnit = true;
            setSelection(text().length(), 0);
        }
        else
        {
            unitUpdate(tr("NM"));
            bPress_changeUnit = true;
            setSelection(text().length(), 0);
        }
        /*QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
        if (ItInfo != pMapAllDisUnit->end())
        {
        ItInfo++;
        if (ItInfo != pMapAllDisUnit->end())
        {
        unitUpdate(ItInfo.key());
        bPress_changeUnit = true;
        setSelection(text().length(), 0);
        }
        else
        {
        ItInfo = pMapAllDisUnit->begin();
        unitUpdate(ItInfo.key());
        bPress_changeUnit = true;
        setSelection(text().length(), 0);
        }
        }*/
    }

}

void CLineEditDistance::keyPressEvent( QKeyEvent * event )
{
    BaseLineEdit::keyPressEvent(event);
    int iKey = event->key();
    if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
    {
        QFocusEvent x(QEvent::FocusOut);
        this->focusOutEvent(&x);
        this->focusNextChild();
    }
    else if (iKey == Qt::Key_Plus || iKey == Qt::Key_Up)
    {
        int iSelect = cursorPosition();
        QString strText = text();
        strText.remove(strUnitInfo);
        if(iSelect >= strText.length())
        {
            if (strUnitInfo == tr("NM"))
            {
                unitUpdate(tr("Cab"));
            }
            else
            {
                unitUpdate(tr("NM"));
            }
            /*QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->begin();
            QString unit_before = "";
            bool bFindFirst = false;
            for (; ItInfo != pMapAllDisUnit->end();ItInfo++)
            {
                if (bFindFirst)
                {
                    unit_before = ItInfo.key();
                    continue;
                }
                if (ItInfo.key() == strUnitInfo)
                {
                    if (unit_before != "")
                    {
                        break;
                    }
                    else
                    {
                        bFindFirst = true;
                    }
                }
                unit_before = ItInfo.key();
            }
            unitUpdate(unit_before);*/
        }
    }
    else if (iKey == Qt::Key_Minus || iKey == Qt::Key_Down)
    {
        int iSelect = cursorPosition();
        QString strText = text();
        strText.remove(strUnitInfo);
        if(iSelect >= strText.length())
        {
            if (strUnitInfo == tr("NM"))
            {
                unitUpdate(tr("Cab"));
            }
            else
            {
                unitUpdate(tr("NM"));
            }
            /*QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
            if (ItInfo != pMapAllDisUnit->end())
            {
            ItInfo++;
            if (ItInfo != pMapAllDisUnit->end())
            {
            unitUpdate(ItInfo.key());
            }
            else
            {
            ItInfo = pMapAllDisUnit->begin();
            unitUpdate(ItInfo.key());
            }
            }*/
        }
    }
}
CLineEditDistance::~CLineEditDistance( void )
{
    disconnect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    disconnect(this, SIGNAL(SignUnitUpdate(QString&)), this, SLOT(slot_LineUnitUpdate(QString&)));
    pMapAllDisUnitCount--;
    if (pMapAllDisUnitCount == 0 && NULL != pMapAllDisUnit)
    {
        delete pMapAllDisUnit;
        pMapAllDisUnit = NULL;
    }

    if (NULL != validator)
    {
        delete validator;
        validator = NULL;
    }
}

void CLineEditDistance::textEditing(const QString& text){
    if (!m_bState && !m_bEditState)
    {
        return ;
    }
    if (!m_bState && m_bEditState)
    {
        m_bChangeState = true;
    }
    int cursorPos = cursorPosition();
    QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDisUnit->end())
    {
        int iCopy = iMax * ItInfo.value();
        QString strText = text;
        strText.remove(strUnitInfo);
        double dTextValue = strText.toDouble();
        if ((int)dTextValue >= iCopy)
        {
            undo();
            return;
        }
    }
    if(cursorPos==1){	// 防止第一个数字也被删除
        this->setCursorPosition(2);
        this->setCursorPosition(1);
    }
}
void CLineEditDistance::setRange(int iMaxInput)
{
    if (0 < iMaxInput)
    {
        iMax = iMaxInput;
    }
}
double CLineEditDistance::getDegree() const
{
    if (!m_bState)
    {
        return 0.0;
    }
    QMap<QString, double>::const_iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDisUnit->end())
    {
        QString strText = text();
        strText.remove(strUnitInfo);
        double dTextValue = strText.toDouble();

        return dTextValue / ItInfo.value();
    }
    else
    {
        return 0.0;
    }
}

double CLineEditDistance::getDegreeFromText(const QString& text)
{
    const QString disUnitArr[6] = {"NM", "Cab", "km", "ft", "mil", "m"};

    QString preUnit = "";
    for (int i=0; i<pMapAllDisUnit->count(); i++)
    {
        if (text.contains(disUnitArr[i]))
        {
            preUnit = disUnitArr[i];
            break;
        }
    }

    double value = text.left(text.indexOf(preUnit)).toDouble();
    QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(preUnit);
    if (ItInfo != pMapAllDisUnit->end())
    {
        value /= ItInfo.value();
    }

    return value;
}

void CLineEditDistance::inputText(double dValue /*NM*/)
{
    if (!m_bState)
    {
        return ;
    }
    QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDisUnit->end())
    {
        if (dValue >  iMax)
        {
            dValue = iMax - 0.0001;
        }
        dValue *= ItInfo.value();

        QString strInputInfo = SeCommonTranslater::FormatUnit(dValue, strUnitInfo, strInputFormat, false);
        this->setText(strInputInfo);
    }
}
void CLineEditDistance::unitUpdate(QString strUnit)
{
    QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnit);
    if (ItInfo != pMapAllDisUnit->end())
    {
        double dValue = getDegree();
        strDisUnitValue = strUnit;
        strUnitInfo = strUnit;
        if (!m_bState)
        {
            return ;
        }
        else
        {
            GetRegExp();
            inputText(dValue);
        }
    }
}

void CLineEditDistance::SetNormalUnit()
{
    QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find("NM");
    if (ItInfo != pMapAllDisUnit->end())
    {
        double dValue = getDegree();
        strUnitInfo = "NM";
        if (!m_bState)
        {
            return ;
        }
        else
        {
            GetRegExp();
            inputText(dValue);
        }
    }
}

void CLineEditDistance::GetRegExp()
{
    QMap<QString, double>::iterator ItInfo = pMapAllDisUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDisUnit->end())
    {
        int iCopy = iMax * ItInfo.value();
        QString strAdd1 = "";
        QString strAdd2 = "";
        QString strMask = "";
        strInputFormat = 4;
        int iCount = 0;
        while (0 != iCopy / 10)
        {
            iCopy /= 10;
            iCount++;
            strMask.append("0");
        }
        strMask.append("0");
        strAdd1.append("[0-").append(QString::number(iCopy)).append("]");
        if (0 != iCount)
        {
            strAdd2.append("\\d{").append(QString::number(iCount)).append(",").append

                (QString::number(iCount)).append("}");
            strAdd1.append(strAdd2);
            strInputFormat = iCount + 5;
        }
        strAdd1.append(".\\d{3,3}");
        strMask.append(".000");
        strAdd1.append(strUnitInfo);

        QString sUnitInfo = "";
        int iAdd = strUnitInfo.size();
        for (int ibegin = 0; ibegin < iAdd; ibegin++)
        {
            sUnitInfo.append("X");
        }

        QRegExp rxAngel(strAdd1);
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        strMask.append(sUnitInfo);
        setInputMask(strMask);
    }
}

void CLineEditDistance::slot_LineUnitUpdate( QString& sUnit )
{
    unitUpdate(sUnit);
}

#define NMDRAFTFACTOR 1.0
#define FMDRAFTFACTOR 1012.6859
#define CMDRAFTFACTOR 185200
#define INDRAFTFACTOR 72913.3858
#define MDRAFTFACTOR 1852
#define KMDRAFTFACTOR 1.852
#define FTDRAFTFACTOR 6076.1155
static QMap<QString, double> *pMapAllDraftUnit = NULL;
static int pMapAllDraftUnitCount = 0;
static QString strDraftUnitValue = "m";
static int iDraftMaxValue = 500;
CLineEditDraft::CLineEditDraft( QWidget* parent):
BaseLineEdit(parent)
{
    m_bState = true;//true is valid ,false is unvalid
    m_bEditState = false;//true is infocus， false is outfocus
    m_bChangeState = false;//true is changed, false is not changed
    m_iRow = -1;
    m_iColumn = -1;
    m_strUnValidText = "-";
    validator = NULL;
    pMapAllDraftUnitCount++;
    if (NULL == pMapAllDraftUnit)
    {
        pMapAllDraftUnit = new QMap<QString, double>;
        pMapAllDraftUnit->insert("NM", NMDRAFTFACTOR);
        pMapAllDraftUnit->insert("fm", FMDRAFTFACTOR);
        pMapAllDraftUnit->insert("cm", CMDRAFTFACTOR);
        pMapAllDraftUnit->insert("in", INDRAFTFACTOR);
        pMapAllDraftUnit->insert("m", MDRAFTFACTOR);
        pMapAllDraftUnit->insert("km", KMDRAFTFACTOR);
        pMapAllDraftUnit->insert("ft", FTDRAFTFACTOR);
    }
    strUnitInfo = strDraftUnitValue;
    iMax = iDraftMaxValue;
    GetRegExp();
    setAlignment(Qt::AlignRight);
    connect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    connect(this, SIGNAL(SignUnitUpdate(QString&)), this, SLOT(slot_LineUnitUpdate(QString&)));
    inputText(0.0);
}
void CLineEditDraft::SetValid( bool bState, QString strText )
{
    if (m_bState == bState || m_bEditState)
    {
        return;
    }
    m_strUnValidText = strText;
    m_bState = bState;
    if (bState)
    {
        //valid
        GetRegExp();
        inputText(0.0);
    }
    else
    {
        //unvalid
        QRegExp rxAngel("-");
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask("XXXXX");
        setText(strText);
    }
}

void CLineEditDraft::focusInEvent( QFocusEvent * e )
{
    BaseLineEdit::focusInEvent(e);
    if (this->isReadOnly() || !(this->isEnabled()) || m_bState)
    {
        return;
    }
    //valid
    m_bEditState = true;
    //valid
    GetRegExp();

    QString strInputInfo = SeCommonTranslater::FormatUnit(0.0, strUnitInfo, strInputFormat, false);
    this->setText(strInputInfo);
    m_bChangeState = false;
}

void CLineEditDraft::focusOutEvent( QFocusEvent * e )
{
    BaseLineEdit::focusOutEvent(e);
    if(!m_bState && m_bChangeState)
    {
        m_bState = true;
    }
    else if (!m_bState)
    {
        //unvalid
        QRegExp rxAngel("-");
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        setInputMask("XXXXX");
        setText(m_strUnValidText);
    }
    m_bEditState = false;
    m_bChangeState = false;
    if (m_bState && m_iRow != -1 && m_iColumn != -1)
    {
        emit textFinishEdit(m_iRow, m_iColumn, getDegree());
    }
    else
    {
        emit FinishEdit(getDegree());
    }
}

void CLineEditDraft::keyPressEvent( QKeyEvent * event )
{
    BaseLineEdit::keyPressEvent(event);
    int iKey = event->key();
    if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
    {
        QFocusEvent x(QEvent::FocusOut);
        this->focusOutEvent(&x);
        this->focusNextChild();
    }
}
CLineEditDraft::~CLineEditDraft( void )
{
    disconnect(this,SIGNAL(textChanged(const QString&)),this,SLOT(textEditing(const QString&)));
    disconnect(this, SIGNAL(SignUnitUpdate(QString&)), this, SLOT(slot_LineUnitUpdate(QString&)));
    pMapAllDraftUnitCount--;
    if (pMapAllDraftUnitCount == 0 && NULL != pMapAllDraftUnit)
    {
        delete pMapAllDraftUnit;
        pMapAllDraftUnit = NULL;
    }

    if (NULL != validator)
    {
        delete validator;
        validator = NULL;
    }
}

void CLineEditDraft::textEditing(const QString& text){
    if (!m_bState && !m_bEditState)
    {
        return ;
    }
    if (!m_bState && m_bEditState)
    {
        m_bChangeState = true;
    }
    int cursorPos = cursorPosition();
    QMap<QString, double>::iterator ItInfo = pMapAllDraftUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDraftUnit->end())
    {
        int iCopy = iMax * ItInfo.value();
        QString strText = text;
        strText.remove(strUnitInfo);
        double dTextValue = strText.toDouble();
        if ((int)dTextValue >= iCopy)
        {
            undo();
            return;
        }
    }
    if(cursorPos==1){	// 防止第一个数字也被删除
        this->setCursorPosition(2);
        this->setCursorPosition(1);
    }
}
void CLineEditDraft::setRange(int iMaxInput)
{
    if (0 < iMaxInput)
    {
        iMax = iMaxInput;
    }
}
double CLineEditDraft::getDegree()
{
    if (!m_bState)
    {
        return 0.0;
    }
    QMap<QString, double>::iterator ItInfo = pMapAllDraftUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDraftUnit->end())
    {
        QString strText = text();
        strText.remove(strUnitInfo);
        double dTextValue = strText.toDouble();

        return dTextValue / ItInfo.value();
    }
    else
    {
        return 0.0;
    }
}
void CLineEditDraft::inputText(double dValue)
{
    if (!m_bState)
    {
        return ;
    }
    QMap<QString, double>::iterator ItInfo = pMapAllDraftUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDraftUnit->end())
    {
        if (dValue >=  iMax)
        {
            dValue = iMax - 0.0001;
        }
        dValue *= ItInfo.value();
        QString strInputInfo = SeCommonTranslater::FormatUnit(dValue, strUnitInfo, strInputFormat, false);
        this->setText(strInputInfo);
    }
}
void CLineEditDraft::unitUpdate(QString strUnit)
{
    QMap<QString, double>::iterator ItInfo = pMapAllDraftUnit->find(strUnit);
    if (ItInfo != pMapAllDraftUnit->end())
    {
        double dValue = getDegree();
        strDraftUnitValue = strUnit;
        strUnitInfo = strUnit;
        if (!m_bState)
        {
            return ;
        }
        else
        {
            GetRegExp();
            inputText(dValue);
        }
    }
}

void CLineEditDraft::GetRegExp()
{
    QMap<QString, double>::iterator ItInfo = pMapAllDraftUnit->find(strUnitInfo);
    if (ItInfo != pMapAllDraftUnit->end())
    {
        int iCopy = iMax * ItInfo.value();
        QString strAdd1 = "";
        QString strAdd2 = "";
        QString strMask = "";
        strInputFormat = 4;
        int iCount = 0;
        while (0 != iCopy / 10)
        {
            iCopy /= 10;
            iCount++;
            strMask.append("0");
        }
        strMask.append("0");
        strAdd1.append("[0-").append(QString::number(iCopy)).append("]");
        if (0 != iCount)
        {
            strAdd2.append("\\d{").append(QString::number(iCount)).append(",").append

                (QString::number(iCount)).append("}");
            strAdd1.append(strAdd2);
            strInputFormat = iCount + 5;
        }
        strAdd1.append(".\\d{3,3}");
        strMask.append(".000");
        strAdd1.append(strUnitInfo);

        QString sUnitInfo = "";
        int iAdd = strUnitInfo.size();
        for (int ibegin = 0; ibegin < iAdd; ibegin++)
        {
            sUnitInfo.append("X");
        }

        QRegExp rxAngel(strAdd1);
        if (NULL != validator)
        {
            delete validator;
            validator = NULL;
        }
        validator = new QRegExpValidator(rxAngel, this);
        setValidator(validator);
        strMask.append(sUnitInfo);
        setInputMask(strMask);
    }
}

void CLineEditDraft::slot_LineUnitUpdate( QString& sUnit )
{
    unitUpdate(sUnit);
}
