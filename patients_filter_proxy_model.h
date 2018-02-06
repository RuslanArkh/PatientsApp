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

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:

    bool m_stillHere, m_Left;

};

#endif // PATIENTS_FILTER_PROXY_MODEL_H
