#include "dbmanager.h"
#include "dbmanager_exceptions.h"

#include <QDir>
#include <QMessageBox>
#include <QtDebug>
#include <QPixmap>
#include <QVariant>

#include <QSqlDatabase>
#include <QSqlQuery>

//  Just leave it here for future project modifications
//const std::regex DBManager::db_file_format { R"(^\w*[.](sqlite)$)" };

DBManager & DBManager::instance() {
    static DBManager singleton;
    return singleton;
}

DBManager::DBManager(const QString & databaseName)
    : mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
      patientDao(*mDatabase),
      photoDao(*mDatabase)
{

    mDatabase->setDatabaseName(qApp->applicationDirPath() + QDir::separator() + databaseName);

    if (!mDatabase->open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
    }

    patientDao.init();
    photoDao.init();

}

DBManager::~DBManager() {
    mDatabase->close();
    delete mDatabase;
}


bool DBManager::HasTable(const QString & _table) {
    return mDatabase->tables().contains(_table);
}
