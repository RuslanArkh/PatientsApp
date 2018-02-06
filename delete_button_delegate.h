#ifndef DELETE_BUTTON_DELEGATE_H
#define DELETE_BUTTON_DELEGATE_H

#include <QItemDelegate>
#include <QWidget>
#include <QPushButton>
#include <QModelIndex>
#include <QEvent>
#include <QMouseEvent>

class DeleteButtonDelegate
        : public QItemDelegate
{
    Q_OBJECT

public:

    DeleteButtonDelegate(QObject *parent = 0);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index);
private:
    mutable QModelIndex m_sourceIndex;
};

#endif // DELETE_BUTTON_DELEGATE_H
