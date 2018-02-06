#include "dbmanager.h"
#include "dbmanager_exceptions.h"

#include <QDir>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QtDebug>
#include <QPixmap>
#include <QVariant>

const std::regex DBManager::db_file_format { R"(^\w*[.](sqlite)$)" };

DBManager::DBManager(const QString & _str) {
    const QString Driver("QSQLITE");
    m_db = QSqlDatabase::addDatabase(Driver);
    if (!std::regex_match(_str.toStdString(), db_file_format))
        DBManagerEx::SqlFileWrongFormat(_str).raise();
    m_db.setDatabaseName(qApp->applicationDirPath() + QDir::separator() + _str);

    if (!m_db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
    }

    QSqlQuery query;
    query.exec("create table patient (id INTEGER primary key, "
               "first_name varchar(30),"
               "last_name varchar(30),"
               "father_name varchar(30),"
               "address TEXT,"
               "birth_date TEXT,"
               "arrival_date TEXT,"
               "leave_date TEXT DEFAULT NULL)"
               );

    query.exec("CREATE TABLE photo (id INTEGER PRIMARY KEY,"
               "patient_id INTEGER,"
               "filename TEXT,"
               "image_data BLOB,"
               "created_on TEXT,"
               "FOREIGN KEY (patient_id) REFERENCES patient(id))");

}


Patient * DBManager::insert(Patient & p) const {
    QSqlQuery query;
    query.prepare("INSERT INTO patient"
          "(first_name, last_name, father_name, address, birth_date, arrival_date, leave_date) "
          "VALUES(:fname, :lname, :dadname, :addr, :bdate, :arrdate, :leavedate)");
    query.bindValue(":fname", p.GetFirstName());
    query.bindValue(":lname", p.GetLastName());
    query.bindValue(":dadname", p.GetFatherName());
    query.bindValue(":addr", p.GetAddress());
    query.bindValue(":bdate", p.GetBirthDate());
    query.bindValue(":arrdate", p.GetArriveDate());

    if (!p.LeaveDate())
        query.bindValue(":leavedate", QVariant::Int);
    else
        query.bindValue(":leavedate", *p.LeaveDate());

    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();

    qDebug() << "Patient inserted!";
    p.SetId(query.lastInsertId().toInt());
    return &p;
}

void DBManager::insert(const Photo & _photo) const {
    QSqlQuery query;
    query.prepare("INSERT INTO photo"
                  "(patient_id, filename, image_data, created_on) "
                  "VALUES(:patient_id, :filename, :image_data, :created_on)");
    query.bindValue(":patient_id", _photo.GetPatientId());
    query.bindValue(":filename", _photo.GetFileName());
    query.bindValue(":image_data", _photo.GetImageBytes());
    query.bindValue(":created_on", _photo.GetCreationDate());

    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();
    _photo.SetId(query.lastInsertId().toInt());

    qDebug() << "Photo inserted!";

}


void DBManager::removePatient(int _id) {
    QSqlQuery query;
    query.prepare("DELETE FROM patient WHERE id = (:id)");
    query.bindValue(":id", _id);
    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();
}

void DBManager::removePhoto(int _id) {
    QSqlQuery query;
    query.prepare("DELETE FROM photo WHERE id = (:id)");
    query.bindValue(":id", _id);
    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();
}

std::vector<Patient *> * DBManager::SelectAllPatients() {
    QSqlQuery q;
    q.prepare("SELECT * FROM patient");
    if (q.exec()) {
        return GetPatientsFromQuery(q);
    } else {
        //  TODO: Raise exception
        DBManagerEx::SqlQueryFailed(q.lastError()).raise();
    }
}

std::vector<Patient *> * DBManager::SelectByFirstNameOrLastName(const QString & _str) {
    QSqlQuery q;
    q.prepare("SELECT * FROM patient "
              "WHERE first_name LIKE '(:first_name)%' OR "
              "last_name LIKE '(:last_name)%'");
    q.bindValue(":first_name", _str);
    q.bindValue(":last_name", _str);
    if (q.exec())
        return GetPatientsFromQuery(q);
    else
        DBManagerEx::SqlQueryFailed(q.lastError()).raise();
}

std::vector<Photo *> * DBManager::SelectByPatientId(int _id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM photo WHERE patient_id=(:patient_id)");
    query.bindValue(":patient_id", _id);
    if (query.exec()) {
        std::vector<Photo *> * photos = new std::vector<Photo *>;
        while (query.next()) {
            int temp_id = query.value(0).toInt();
            int temp_patient_id = query.value(1).toInt();
            QString temp_filename = query.value(2).toString();

            QPixmap temp_pixmap = QPixmap();
            temp_pixmap.loadFromData(query.value(3).toByteArray());

            QDate temp_created_on = query.value(4).toDate();
            Photo * p = new Photo(temp_patient_id, temp_filename, temp_pixmap, temp_created_on);
            p->SetId(temp_id);
            photos->push_back(p);
        }
        return photos;
    } else {
        //  TODO: Exception
        return nullptr;
    }
}

void DBManager::Update(Patient & _p) {
    QSqlQuery query;
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
    query.bindValue(":fname", _p.GetFirstName());
    query.bindValue(":lname", _p.GetLastName());
    query.bindValue(":father_name", _p.GetFatherName());
    query.bindValue(":address", _p.GetAddress());
    query.bindValue(":birth_date", _p.GetBirthDate());
    query.bindValue(":arrival_date", _p.GetArriveDate());
    if (_p.LeaveDate())
        query.bindValue(":leave_date", *_p.LeaveDate());
    else
        query.bindValue(":leave_date", QVariant::Int);
    query.bindValue(":p_id", _p.GetId());

    if (!query.exec())
      DBManagerEx::SqlQueryFailed(query.lastError()).raise();
}

std::vector<Patient *> * DBManager::GetPatientsFromQuery(QSqlQuery & _q) {
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
        /*try {
            Patient stack_p(fname,
                            lname,
                            father_name,
                            address,
                            birth_date,
                            arr_date,
                            *leave_date);*/
            Patient * temp_p = new Patient(fname,
                                           lname,
                                           father_name,
                                           address,
                                           birth_date,
                                           arr_date,
                                           *leave_date);
            temp_p->SetId(id);
            temp_patients->push_back(temp_p);
        /*} catch (Patient_Ex::BirthAndArriveDateLogicError & _ex) {
            QString exc_msg{};
            exc_msg += "WARNING: Patient: [";
            exc_msg += "ID: " + QString::number(id) + " First Name: " + fname +
                    " Last Name: " + lname + "]\n";
            exc_msg += "Was not added to list due to date logic exception:\n";
            exc_msg += _ex.message();
            qDebug() << exc_msg;
        }*/
    }
    return temp_patients;
}
