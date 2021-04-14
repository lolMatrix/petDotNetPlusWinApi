#include "regexvalidator.h"

RegexValidator::RegexValidator()
{

}

QValidator::State RegexValidator::validate(QString &str, int &pos) const
{
    QRegExpValidator *val = new QRegExpValidator(QRegExp("^([{a-zA-zа-яА-ЯёЁ} \".,!])+$"));
    State s = val->validate(str, pos);
    if (s == Acceptable){
        if (str.contains(QChar('_')) || str.contains(QChar('^'))) return QValidator::Invalid;
    }
    return s;
}
