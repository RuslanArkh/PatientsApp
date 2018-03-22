#include "patientdao.h"
#include "patient.h"

#include "dbmanager.h"
#include "dbmanager_exceptions.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

PatientDao::PatientDao(QSqlDatabase & database)
    : mDatabase(database)
{
}

void PatientDao::init() const {
    if (!mDatabase.tables().contains("patient")) {
        QSqlQuery query(mDatabase);
        query.exec("PRAGMA foreign_keys = ON");
        query.exec("create table patient (id INTEGER primary key, "
                   "first_name varchar(30),"
                   "last_name varchar(30),"
                   "father_name varchar(30),"
                   "address TEXT,"
                   "birth_date TEXT,"
                   "arrival_date TEXT,"
                   "leave_date TEXT DEFAULT NULL)");
    }
}

void PatientDao::addPatient(const Patient & patient) const {
    QSqlQuery query(mDatabase);

    query.prepare("INSERT INTO patient"
          "(first_name, last_name, father_name, address, birth_date, arrival_date, leave_date) "
          "VALUES(:fname, :lname, :dadname, :addr, :bdate, :arrdate, :leavedate)");
    query.bindValue(":fname", patient.GetFirstName());
    query.bindValue(":lname", patient.GetLastName());
    query.bindValue(":dadname", patient.GetFatherName());
    query.bindValue(":addr", patient.GetAddress());
    query.bindValue(":bdate", patient.GetBirthDate());
    query.bindValue(":arrdate", patient.GetArriveDate());

    if (!patient.LeaveDate())
        query.bindValue(":leavedate", QVariant::Int);
    else
        query.bindValue(":leavedate", *patient.LeaveDate());

    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();

    qDebug() << "Patient inserted!";
    patient.SetId(query.lastInsertId().toInt());

}

void PatientDao::updatePatient(const Patient & patient) const {
    QSqlQuery query(mDatabase);
    query.prepare("UPDATE patient SET "
              "first_name=(:fname), "
              "last_name=(:lname), "
              "father_name=(:father_name), "
              "address=(:address), "
              "birth_date=(:birth_date), "
              "arrival_date=(:arrival_date), "
              "leave_date=(:leave_date) "
              "WHERE id=(:p_id)"
    );
    query.bindValue(":fname", patient.GetFirstName());
    query.bindValue(":lname", patient.GetLastName());
    query.bindValue(":father_name", patient.GetFatherName());
    query.bindValue(":address", patient.GetAddress());
    query.bindValue(":birth_date", patient.GetBirthDate());
    query.bindValue(":arrival_date", patient.GetArriveDate());
    if (patient.LeaveDate())
        query.bindValue(":leave_date", *patient.LeaveDate());
    else
        query.bindValue(":leave_date", QVariant::Int);
    query.bindValue(":p_id", patient.GetId());

    if (!query.exec())
      DBManagerEx::SqlQueryFailed(query.lastError()).raise();
}

void PatientDao::removePatient(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM patient WHERE id = (:id)");
    query.bindValue(":id", id);
    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();
}

std::vector<Patient *> * PatientDao::patientsAll() const {
    QSqlQuery q(mDatabase);
    q.prepare("SELECT * FROM patient");
    if (q.exec()) {
        return GetPatientsFromQuery(q);
    } else {
        //  TODO: Raise exception
        DBManagerEx::SqlQueryFailed(q.lastError()).raise();
    }
}

std::vector<Patient *> * PatientDao::patientsByFirstName(const QString &firstName) const {
    QSqlQuery q(mDatabase);
    q.prepare("SELECT * FROM patient "
              "WHERE first_name LIKE '(:first_name)%'");
    q.bindValue(":first_name", firstName);
    if (q.exec())
        return GetPatientsFromQuery(q);
    else
        DBManagerEx::SqlQueryFailed(q.lastError()).raise();
}

std::vector<Patient *> * PatientDao::patientsByLastName(const QString &lastName) const {
    QSqlQuery q(mDatabase);
    q.prepare("SELECT * FROM patient "
              "WHERE last_name LIKE '(:last_name)%'");
    q.bindValue(":first_name", lastName);
    if (q.exec())
        return GetPatientsFromQuery(q);
    else
        DBManagerEx::SqlQueryFailed(q.lastError()).raise();
}

std::vector<Patient *> * PatientDao::GetPatientsFromQuery(QSqlQuery & _q) const {
    std::vector<Patient *> * temp_patients = new std::vector<Patient *>();
    while (_q.next()) {
        int id = _q.value(0).toInt();
        QString fname = _q.value(1).toString();
        QString lname = _q.value(2).toString();
        QString father_name = _q.value(3).toString();
        QString address = _q.value(4).toString();
        QDate birth_date = _q.value(5).toDate();
        QDate arr_date = _q.value(6).toDate();
        QDate * leave_date;
        if (_q.value(7).isNull())
            leave_date = nullptr;
        else
            leave_date = new QDate(_q.value(7).toDate());
            Patient * temp_p = new Patient(fname,
                                           lname,
                                           father_name,
                                           address,
                                           birth_date,
                                           arr_date,
                                           *leave_date);
            temp_p->SetId(id);
            temp_patients->push_back(temp_p);
    }
    return temp_patients;
}


