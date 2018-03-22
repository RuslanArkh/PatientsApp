#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QException>

#include <regex>
#include <vector>
#include <list>

#include <QString>

#include "patient.h"
#include "photo.h"

#include "patientdao.h"
#include "photodao.h"

const QString DATABASE_FILENAME = "patients.db";

class QSqlDatabase;
class QSqlQuery;


class DBManager {

public:

    static DBManager & instance();

    ~DBManager();

protected:

    DBManager(const QString & databaseName = DATABASE_FILENAME);
    DBManager & operator=(const DBManager & rhs);

private:

    QSqlDatabase * mDatabase;

public:

    const PatientDao patientDao;
    const PhotoDao photoDao;

    bool HasTable(const QString & _table);
};


#endif // DBMANAGER_H
