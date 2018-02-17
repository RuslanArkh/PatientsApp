#include "delete_button_delegate.h"

#include "cremovepatientbutton.h"

#include <QSortFilterProxyModel>
#include <QApplication>
#include <QtGui>
#include <QRect>
#include <QStyle>
#include <QPainter>
#include <QStyleOptionButton>
#include <QtDebug>

#include "patients_filter_proxy_model.h"
#include "patients_model.h"

DeleteButtonDelegate::DeleteButtonDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget * DeleteButtonDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{


    return nullptr;
}

void DeleteButtonDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QRect r = option.rect;//getting the rect of the cell
    int x,y,w,h;
    x = r.left() + r.width() - 57;//the X coordinate
    y = r.top() + 4;//the Y coordinate
    w = 56; //button width
    h = 30; //button height

    QStyleOptionButton button;
//    CRemovePatientButton *pB = new CRemovePatientButton;
//    pB->setText("Remove");
//    auto proxy_model = dynamic_cast<const QSortFilterProxyModel *>(index.model());
//    pB->setModelIndex(proxy_model->mapToSource(index));
//    pB->setGeometry(0, 0, 30, 30);

//    pB->rect = QRect(x,y,w,h);
//    pB->text = "Delete";
//    pB->state = QStyle::State_Enabled;

    button.rect = QRect(x,y,w,h);
    button.text = "Delete";
    button.state = QStyle::State_Enabled;

    auto patients_proxy_model = dynamic_cast<const PatientsFilterProxyModel *>(index.model());

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    m_sourceIndex = patients_proxy_model->mapToSource(index);
}

bool DeleteButtonDelegate::editorEvent(QEvent *event,
                                       QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent * e = (QMouseEvent *)event;
        int clickX = e->x();
        int clickY = e->y();

        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 45;//the X coordinate
        y = r.top();//the Y coordinate
        w = 45;//button width
        h = 30;//button height

        if( clickX > x && clickX < x + w ) {
            if( clickY > y && clickY < y + h )
             {
                emit dynamic_cast<const PatientsModel *>(m_sourceIndex.model())->itemForRemove(index);
             }
        }
    }
}
