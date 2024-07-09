#include <gtest/gtest.h>
#include "app.h"
#include <QQuickWindow>
#include <QMetaObject>
#include <QAbstractItemModel>
#include <QTest>
#include <QTimer>
using namespace testing;
TEST(app_test, simpleTest)
{
	EXPECT_EQ(1000 - 7, 993);
}
