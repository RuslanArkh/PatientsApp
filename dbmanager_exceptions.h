#ifndef DBMANAGER_EXCEPTIONS_H
#define DBMANAGER_EXCEPTIONS_H

#include <QException>
#include <QString>
#include <QSqlError>

namespace DBManagerEx {

    class SqlFileWrongFormat : public QException {
        const QString _message;
    public:
        SqlFileWrongFormat(const QString & message);
        void raise() const { throw *this; }
        SqlFileWrongFormat *clone() const { return new SqlFileWrongFormat(*this); }
        const QString & message() const;
    };
    inline const QString & SqlFileWrongFormat::message() const { return _message; }


    class SqlQueryFailed : public QException {
        const QSqlError _sqlMessage;
    public:
        SqlQueryFailed(const QSqlError & sqlmessage);
        void raise() const { throw *this; }
        SqlQueryFailed *clone() const { return new SqlQueryFailed(*this); }
        const QSqlError & message() const;
    };
    inline const QSqlError & SqlQueryFailed::message() const { return _sqlMessage; }


    class TableNotExist: public QException {
    public:
        void raise() const { throw *this; }
        TableNotExist *clone() const { return new TableNotExist(*this); }
    };
}

#endif // DBMANAGER_EXCEPTIONS_H
