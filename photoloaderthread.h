#ifndef PHOTOLOADER_H
#define PHOTOLOADER_H

#include "photo.h"

#include <QThread>
#include <QMutex>

class PhotoLoaderThread : public QThread {

    Q_OBJECT

public:

    PhotoLoaderThread(int patientId, QObject *parent = nullptr);

    void run() override;

    void stopProcess();

signals:

    void PhotoLoaded(Photo * photo);
    void Abort();

private:
    int mPatientId;
    bool m_abort;
    QMutex mutex;
};

#endif // PHOTOLOADER_H
