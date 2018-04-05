#ifndef PATIENTDAO_H
#define PATIENTDAO_H

#include <vector>


class QSqlDatabase;
class Patient;
class QSqlQuery;
class QString;

class PatientDao {

public:

    PatientDao(QSqlDatabase & database);
    void init() const;

    void addPatient(const Patient & patient) const;
    void updatePatient(const Patient & patient) const;
    void removePatient(int id) const;

    std::vector<Patient *> * patientsAll() const;
    std::vector<Patient *> * patientsByFirstName(const QString & firstName) const;
    std::vector<Patient *> * patientsByLastName(const QString & lastName) const;

private:

    QSqlDatabase & mDatabase;

    std::vector<Patient *> * GetPatientsFromQuery(QSqlQuery & _q) const;

};

#endif // PATIENTDAO_H
