#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QDialog>

#include <vector>
#include "photo.h"

namespace Ui {
class ImageView;
}

class ImageView : public QDialog
{
    Q_OBJECT

public:
    explicit ImageView(const std::vector<Photo *> & _photos, int _index, QWidget *parent = 0);
    ~ImageView();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    int m_currentIndex;
    const std::vector<Photo *> * p_photos;
    Ui::ImageView *ui;


    void ShowPhoto();
    void Next();
    void Previous();
};

#endif // IMAGEVIEW_H
