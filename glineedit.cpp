#include "glineedit.h"
#include <cmath>

GLineEdit::GLineEdit(QWidget *parent)
    : QLineEdit{parent}
{
    connect(this, &QLineEdit::textEdited, this, &GLineEdit::on_textEdited);
}

double GLineEdit::getValue() const
{
    return value;
}

void GLineEdit::setValue(double newValue)
{
    value = newValue;
    if(!std::isnan(value))
    {
        const QString strVal = fmt(value);
        return setText(strVal);
    }
    setText("==°==。===′");
}

void GLineEdit::reset()
{
    setValue(std::nan("NaN"));
}

QString GLineEdit::fmt(const double value)
{
    return QString::number(value);
}

double GLineEdit::fmt(const QString &strVal)
{
    bool ok = false;
    const double v = strVal.toDouble(&ok);
    return ok ? v : std::nan("NaN");
}

void GLineEdit::on_textEdited(const QString &text)
{
    value = fmt(text);
}


GLonLineEdit::GLonLineEdit(QWidget *parent)
    : GLineEdit{parent}
{

}

QString GLonLineEdit::fmt(const double value)
{
    return "TODO";
}
