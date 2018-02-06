#ifndef CREMOVEPATIENTBUTTON_H
#define CREMOVEPATIENTBUTTON_H

#include <QPushButton>
#include <QModelIndex>

class CRemovePatientButton : public QPushButton
{
public:
    CRemovePatientButton(QWidget* parent = nullptr);

    void setModelIndex( const QModelIndex& index );

private:

    QModelIndex m_sourceIndex;

private slots:

    void onClicked();
};

#endif // CREMOVEPATIENTBUTTON_H
