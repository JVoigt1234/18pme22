#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QThread>

class SqlDatabase : public QThread
{
public:
    SqlDatabase();
private:
    void run();
};

#endif // SQLDATABASE_H
