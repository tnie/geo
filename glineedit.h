#ifndef GLINEEDIT_H
#define GLINEEDIT_H

#include <QLineEdit>

class GLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit GLineEdit(QWidget *parent = nullptr);

    double getValue() const;
    void setValue(double newValue);
    void reset();

protected:
    virtual QString fmt(const double value);
    virtual double fmt(const QString& strVal);
signals:

private:
//    避免格式化输出后丢失精度
    double value;
    void on_textEdited(const QString& text);
};

class GLonLineEdit: public GLineEdit
{
    Q_OBJECT
public:
    explicit GLonLineEdit(QWidget *parent = nullptr);

    static QString fmt(const double value, const QString fmt);
protected:
    QString fmt(const double value) override;
};

#endif // GLINEEDIT_H
