#include "patients_window.h"
#include "patient.h"

#include "dbmanager.h"
#include "dbmanager_exceptions.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PatientsWindow w;
    w.setWindowIcon(QIcon(":/app_images/medicine.png"));
    w.show();
    return a.exec();
}
