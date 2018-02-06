#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <QSqlQuery>
#include <QString>
#include <QException>

#include <regex>
#include <vector>
#include <list>

#include "patient.h"
#include "photo.h"


//  TODO: make it singleton
//  TODO: make two derived classes: PatientManager and PhotoManager
class DBManager {
    QSqlDatabase m_db;

    static const std::regex db_file_format;

public:

    DBManager(const QString & _databaseName);

    Patient * insert(Patient & p) const;
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

inline bool DBManager::HasTable(const QString & _table) { return m_db.tables().contains(_table); }

#endif // DBMANAGER_H
