#ifndef PATIENTS_FILTER_PROXY_MODEL_H
#define PATIENTS_FILTER_PROXY_MODEL_H

#include <QSortFilterProxyModel>

class PatientsFilterProxyModel
        : public QSortFilterProxyModel
{
    Q_OBJECT

public:

    PatientsFilterProxyModel(QObject *parent = 0);

    void setStillHere(bool still_here);
    void setLeft(bool left);

protected:

    /*
     * Allows to:
     * 1. Sort by patient's presence
     * 2. Search by first name or last name or father name
     * 3. Search by Full name using format: [first name] [last name] [father name]
     * */
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:

    bool m_stillHere, m_Left;

};

#endif // PATIENTS_FILTER_PROXY_MODEL_H
