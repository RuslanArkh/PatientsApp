#include "dbmanager_exceptions.h"

using namespace DBManagerEx;

SqlFileWrongFormat::SqlFileWrongFormat(const QString & message)
    :_message(message){}

SqlQueryFailed::SqlQueryFailed(const QSqlError & sqlmessage)
    : _sqlMessage{sqlmessage} {}

