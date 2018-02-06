#include "imageview.h"
#include "ui_imageview.h"

ImageView::ImageView(const std::vector<Photo *> & _photos, int _index, QWidget *parent) :
    QDialog(parent),
    m_currentIndex(_index),
    p_photos(&_photos),

    ui(new Ui::ImageView)
{
    ui->setupUi(this);
    ShowPhoto();
}

void ImageView::ShowPhoto() {
    Photo * _photo = p_photos->at(m_currentIndex);
    QByteArray  _barr = _photo->GetImageBytes();
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(_barr);
    ui->label->setPixmap(outPixmap.scaled(ui->label->width(),
                                          ui->label->height(),
                                          Qt::KeepAspectRatio));
}

void ImageView::Next() {
    if (m_currentIndex == p_photos->size() - 1)
        m_currentIndex = 0;
    else
        ++m_currentIndex;
}

void ImageView::Previous() {
    if (m_currentIndex == 0)
        m_currentIndex = p_photos->size() - 1;
    else
        --m_currentIndex;
}

ImageView::~ImageView()
{
    delete ui;
}

void ImageView::on_pushButton_clicked()
{
    Previous();
    ShowPhoto();
}

void ImageView::on_pushButton_2_clicked()
{
    Next();
    ShowPhoto();
}
