#include "patients_window.h"
#include "patient.h"

#include "dbmanager.h"
#include "dbmanager_exceptions.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        DBManager manager{"my_db.sqlite"};
        PatientsWindow w(&manager);
        w.show();
        return a.exec();
    } catch (DBManagerEx::SqlFileWrongFormat & _ex) {
        QString exc_msg{};
        exc_msg += "Wrong format of file: '" + _ex.message() + "'!\n";
        exc_msg += "Use '.sqlite' format.";
        QMessageBox::critical(nullptr, "SqlError",
                              exc_msg,
                              QMessageBox::Ok);
        return -1;
    }
}
