#pragma once
#include <qstring.h>
#include <iostream>
//This header must be included in all sources where gtest print with QString is used to avoid ODR violations
QT_BEGIN_NAMESPACE
void PrintTo(const QString& qString, ::std::ostream* os)
{
    *os << qPrintable(qString);
}
QT_END_NAMESPACE