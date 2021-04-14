#ifndef REGEXVALIDATOR_H
#define REGEXVALIDATOR_H

#include <QValidator>

class RegexValidator : public QValidator
{
public:
    RegexValidator();

    // QValidator interface
public:
    State validate(QString &, int &) const;
};

#endif // REGEXVALIDATOR_H
