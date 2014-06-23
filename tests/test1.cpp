#include <QtCore>
#include <QtTest>
#include "tchar.h"



class TestQString: public QObject
{
 Q_OBJECT
private slots:
 void toUpper();
};

void TestQString::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

#include "test1.moc"
