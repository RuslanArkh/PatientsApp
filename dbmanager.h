#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <QSqlQuery>
#include <QString>
#include <QException>

#include <regex>
#include <vector>
#include <list>

#include <QString>

#include "patient.h"
#include "photo.h"

const QString DATABASE_FILENAME = "patients.db";

//  TODO: make it singleton - DONE
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

    //  TODO: Create DAO's for every model instead of following methods

    void insert(const Patient & p) const;
    void insert(const Photo & _photo) const;

    std::vector<Patient *> * SelectAllPatients();
    std::vector<Patient *> * SelectByFirstNameOrLastName(const QString & _str);

    std::vector<Photo *> * SelectByPatientId(int _id);

    void Update(Patient & _p);

    void removePatient(int _id);
    void removePhoto(int _id);

    bool HasTable(const QString & _table);

private:

    std::vector<Patient *> * GetPatientsFromQuery(QSqlQuery & _q);
};

inline bool DBManager::HasTable(const QString & _table) { return mDatabase->tables().contains(_table); }

#endif // DBMANAGER_H
