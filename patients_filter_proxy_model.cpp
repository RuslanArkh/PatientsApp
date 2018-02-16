#include "patients_filter_proxy_model.h"

#include <QString>
#include <QStringList>
#include <QDate>
#include <QtDebug>

PatientsFilterProxyModel::PatientsFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

bool PatientsFilterProxyModel::filterAcceptsRow(
        int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);
    QStringList splitted_string = index0.data().toString().split(' ');
    QString fname = splitted_string.at(0);
    QString lname = splitted_string.at(1);
    QString dad_name = splitted_string.at(2);
    QDate checkDate;
    checkDate = QDate::fromString(index1.data().toString(), "dd/MM/yyyy");
    return (fname.contains(filterRegExp()) ||
            lname.contains(filterRegExp()) ||
            dad_name.contains(filterRegExp()) ||
            index0.data().toString().contains(filterRegExp())) &&
            ((m_stillHere && m_Left) ||
            (   (m_stillHere && !m_Left) &&
                (checkDate >= QDate::currentDate() || !checkDate.isValid())
            ) ||
            (   (!m_stillHere && m_Left) &&
                checkDate < QDate::currentDate() &&
                checkDate.isValid())
            );
}

void PatientsFilterProxyModel::setStillHere(bool still_here) {
    m_stillHere = still_here;
    invalidateFilter();
}

void PatientsFilterProxyModel::setLeft(bool left) {
    m_Left = left;
    invalidateFilter();
}
