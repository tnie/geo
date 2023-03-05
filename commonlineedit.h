#ifndef COMMONLINEEDIT_H
#define COMMONLINEEDIT_H

#include <QLineEdit>
#include <QComboBox>
#include <QRegExpValidator>
#include <QValidator>
#include <qevent.h>
#include <QLabel>


//	经度模式编辑框
//	输入范围 000°00'.000[W] 到 179°59'.999[E]
class SeCommonLineEditLon: public QLineEdit {
    Q_OBJECT

public:
    ~SeCommonLineEditLon(void);
    SeCommonLineEditLon(QWidget* parent);
    double getRad();
    double getDegree();
    void inputText(double dLon);
    void SetValid(bool bState, QString strText = QString::fromLocal8Bit("---°--'.----"));
    void SetRowColumn(int iRow, int iColumn)
    {
        m_iRow = iRow;
        m_iColumn = iColumn;
    }
    bool GetValid()
    {
        return m_bState;
    }
    bool GetIsInEdit()
    {
        return m_bIsInEdit;
    }
signals:
    void  textFinishEdit(int iRow, int icolumn, double dDegree);
    void  FinishEdit(double dDegree);
protected:
    //virtual void paintEvent(QPaintEvent*) ;
    virtual void focusInEvent(QFocusEvent * e) override;
    virtual void focusOutEvent(QFocusEvent * e) override;
    virtual void keyPressEvent(QKeyEvent * event) override;
protected slots:
    void	textEditing(const QString& text);

private:
    QString preText;
    QValidator* validator;
    QString strCov;
    bool m_bState;
    bool m_bEditState;
    bool m_bChangeState;
    QString m_strUnValidText;
    int m_iRow;
    int m_iColumn;
    bool m_bIsInEdit;
};

//纬度模式编辑框
//	输入范围 00°00'.000[S] 到 89°59'.999[N]
class SeCommonLineEditLat: public QLineEdit {
    Q_OBJECT

public:
    ~SeCommonLineEditLat(void);
    SeCommonLineEditLat(QWidget* parent);
    double getRad();
    double getDegree();
    void inputText(double dLat);
    void SetValid(bool bState, QString strText = QString::fromLocal8Bit("--°--'.----"));
    bool GetValid()
    {
        return m_bState;
    }
    bool GetIsInEdit()
    {
        return m_bIsInEdit;
    }
signals:
    void  FinishEdit(double dDegree);
protected:
    //virtual void mousePressEvent(QMouseEvent*);
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);
    virtual void keyPressEvent(QKeyEvent * event);
protected slots:
    void	textEditing(const QString& text);

private:
    QValidator* validator;
    bool m_bState;
    bool m_bEditState;
    bool m_bChangeState;
    QString m_strUnValidText;
    bool m_bIsInEdit;
};

//	角模式编辑框
//	输入范围 000°00.000' 到 359°59.999'
class SeCommonLineEditDegree: public QLineEdit {
    Q_OBJECT

public:
    ~SeCommonLineEditDegree(void);
    SeCommonLineEditDegree(QWidget* parent);
    double getRad();
    double getDegree();
    void inputText(double dAngle);
    void SetValid(bool bState, QString strText = QString::fromLocal8Bit("---°--.---'"));
    void SetRowColumn(int iRow, int iColumn)
    {
        m_iRow = iRow;
        m_iColumn = iColumn;
    }
    bool GetIsInEdit()
    {
        return m_bIsInEdit;
    }
signals:
    void  textFinishEdit(int iRow, int icolumn, double dDegree);
    void  FinishEdit(double dDegree);
protected:
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);
    virtual void keyPressEvent(QKeyEvent * event);
protected slots:
    void  textEditing(const QString& text);
private:
    QString preText;
    QString strCov;
    QValidator* validator;
    bool m_bState;
    bool m_bEditState;
    bool m_bChangeState;
    QString m_strUnValidText;
    int m_iRow;
    int m_iColumn;
    bool m_bIsInEdit;
};

//speed .000
class CLineEditSpeed: public QLineEdit {
    Q_OBJECT

public:
    CLineEditSpeed(QWidget* parent);
    ~CLineEditSpeed(void);
    double getDegree();
    void inputText(double dValue);
    void setRange(int iMaxInput);
    void SetValid(bool bState, QString strText = "-");
    void SetNormalUnit();
    void SetRowColumn(int iRow, int iColumn)
    {
        m_iRow = iRow;
        m_iColumn = iColumn;
    }
    void SetNeTag(bool bTag)
    {
        m_bNeTag = bTag;
        GetRegExp();
    }
    bool GetIsInEdit()
    {
        return m_bIsInEdit;
    }
signals:
    void  textFinishEdit(int iRow, int icolumn, double dDegree);
    void  FinishEdit(double dDegree);
protected:
    //virtual void paintEvent(QPaintEvent*) ;
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);
    virtual void keyPressEvent(QKeyEvent * event);
    protected slots:
        void	textEditing(const QString& text);
        void	slot_LineUnitUpdate(QString& sUnit);
private:
    void GetRegExp();
    void unitUpdate(QString strUnit);
    QString preText;
    QValidator* validator;
    QString strCov;
    int iMax;
    QString strUnitInfo;
    int strInputFormat;
    bool m_bState;
    bool m_bEditState;
    bool m_bChangeState;
    QString m_strUnValidText;
    int m_iRow;
    int m_iColumn;
    bool m_bNeTag;
    bool m_bIsInEdit;
};

//distance large .000
class CLineEditDistance: public QLineEdit {
    Q_OBJECT

public:
    /*CLineEditDistance(QWidget* parent, int maxVal=500000);*/
    CLineEditDistance(QWidget* parent, int maxVal=200);
    ~CLineEditDistance(void);
    // unit is NM.
    double getDegree() const;
    void inputText(double dValue /*NM*/);
    void setRange(int iMaxInput);
    void SetValid(bool bState, QString strText = "-");
    void SetRowColumn(int iRow, int iColumn)
    {
        m_iRow = iRow;
        m_iColumn = iColumn;
    }
public:
    double getDegreeFromText(const QString& text);
    void SetNormalUnit();
signals:
    void  textFinishEdit(int iRow, int icolumn, double dDegree);
    void  FinishEdit(double dDegree);
protected:
    //virtual void paintEvent(QPaintEvent*) ;
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    protected slots:
        void	textEditing(const QString& text);
        void	slot_LineUnitUpdate(QString& sUnit);
private:
    void GetRegExp();
    void unitUpdate(QString strUnit);
    QString preText;
    QValidator* validator;
    QString strCov;
    int iMax;
    QString strUnitInfo;
    int strInputFormat;
    bool m_bState;
    bool m_bEditState;
    bool m_bChangeState;
    QString m_strUnValidText;
    int m_iRow;
    int m_iColumn;

    bool bPress_changeUnit;//true表示单击切换单位
};

//draft .000
class CLineEditDraft: public QLineEdit {
    Q_OBJECT

public:
    CLineEditDraft(QWidget* parent);
    ~CLineEditDraft(void);
    double getDegree();
    void inputText(double dValue);
    void setRange(int iMaxInput);
    void SetValid(bool bState, QString strText = "-");
    void SetRowColumn(int iRow, int iColumn)
    {
        m_iRow = iRow;
        m_iColumn = iColumn;
    }
signals:
    void  textFinishEdit(int iRow, int icolumn, double dDegree);
    void  FinishEdit(double dDegree);
protected:
    //virtual void paintEvent(QPaintEvent*) ;
    virtual void focusInEvent(QFocusEvent * e);
    virtual void focusOutEvent(QFocusEvent * e);
    virtual void keyPressEvent(QKeyEvent * event);
    protected slots:
        void	textEditing(const QString& text);
        void	slot_LineUnitUpdate(QString& sUnit);
private:
    void GetRegExp();
    void unitUpdate(QString strUnit);
    QString preText;
    QValidator* validator;
    QString strCov;
    int iMax;
    QString strUnitInfo;
    int strInputFormat;
    bool m_bState;
    bool m_bEditState;
    bool m_bChangeState;
    QString m_strUnValidText;
    int m_iRow;
    int m_iColumn;
};
class QExClickLable: public QLabel {
    Q_OBJECT
public:
    QExClickLable(QWidget *parent = 0) : QLabel(parent)
    {};
signals:
    void clicked();
protected:
    virtual void mousePressEvent ( QMouseEvent * e )
    {
        if(e->button() == Qt::LeftButton)
        {
            emit clicked();
        }

    };
};
class QLineEditEnter: public QLineEdit {
    Q_OBJECT
public:
    QLineEditEnter(QWidget *parent = 0) : QLineEdit(parent)
    {};
protected:
    virtual void keyPressEvent(QKeyEvent * event)
    {
        QLineEdit::keyPressEvent(event);
        int iKey = event->key();
        if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
        {
            QFocusEvent x(QEvent::FocusOut);
            this->focusOutEvent(&x);
            this->focusNextChild();
        }
    }
};

class QLineEditNum: public QLineEdit {
    Q_OBJECT
public:
    QLineEditNum(QWidget *parent = 0) : QLineEdit(parent)
    {};
protected:
    virtual void focusOutEvent( QFocusEvent * e )
    {
        emit FinishEdit(this->text());
    }
    virtual void keyPressEvent(QKeyEvent * event)
    {
        QLineEdit::keyPressEvent(event);
        int iKey = event->key();
        if (iKey == Qt::Key_Return || iKey == Qt::Key_Enter)
        {
            QFocusEvent x(QEvent::FocusOut);
            this->focusOutEvent(&x);
            this->focusNextChild();
        }
    }
signals:
    void  FinishEdit(QString str);
};


#endif // COMMONLINEEDIT_H
