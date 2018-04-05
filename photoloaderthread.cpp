#include "photoloaderthread.h"

#include "dbmanager.h"

#include <QSqlQuery>

PhotoLoaderThread::PhotoLoaderThread(int patientId, QObject *parent)
    : mPatientId(patientId), QThread(parent)
{
    mutex.lock();
    m_abort = false;
    mutex.unlock();
}

void PhotoLoaderThread::run() {
    QSqlQuery query = DBManager::instance().photoDao.photosByPatientIdQuery(mPatientId);
    while (query.next()) {

        int temp_id = query.value(0).toInt();
        int temp_patient_id = query.value(1).toInt();
        QString temp_filename = query.value(2).toString();

        QPixmap temp_pixmap = QPixmap();
        temp_pixmap.loadFromData(query.value(3).toByteArray());

        QDate temp_created_on = query.value(4).toDate();
        Photo * p = new Photo(temp_patient_id, temp_filename, temp_pixmap, temp_created_on);
        p->SetId(temp_id);
        emit PhotoLoaded(p);
        if (m_abort) {
            emit Abort();
            return;
        }
    }
}

void PhotoLoaderThread::stopProcess() {
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}
