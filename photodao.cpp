#include "photodao.h"
#include "photo.h"

#include "dbmanager.h"
#include "dbmanager_exceptions.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

PhotoDao::PhotoDao(QSqlDatabase & database)
    : mDatabase(database)
{
}


void PhotoDao::init() const {
    if (!mDatabase.tables().contains("photo")) {
        QSqlQuery query(mDatabase);
        query.exec("CREATE TABLE photo (id INTEGER PRIMARY KEY,"
                   "patient_id INTEGER,"
                   "filename TEXT,"
                   "image_data BLOB,"
                   "created_on TEXT,"
                   "FOREIGN KEY (patient_id) REFERENCES patient(id) ON DELETE CASCADE);");
    }
}

void PhotoDao::addPhoto(const Photo &photo) const {
    QSqlQuery query(mDatabase);
    query.prepare("INSERT INTO photo"
                  "(patient_id, filename, image_data, created_on) "
                  "VALUES(:patient_id, :filename, :image_data, :created_on)");
    query.bindValue(":patient_id", photo.GetPatientId());
    query.bindValue(":filename", photo.GetFileName());
    query.bindValue(":image_data", photo.GetImageBytes());
    query.bindValue(":created_on", photo.GetCreationDate());

    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();
    photo.SetId(query.lastInsertId().toInt());
}

void PhotoDao::removePhoto(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM photo WHERE id = (:id)");
    query.bindValue(":id", id);
    if (!query.exec())
        DBManagerEx::SqlQueryFailed(query.lastError()).raise();
}

std::vector<Photo *> * PhotoDao::photosByPatientId(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("SELECT * FROM photo WHERE patient_id=(:patient_id)");
    query.bindValue(":patient_id", id);
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
