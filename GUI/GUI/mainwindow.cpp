#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QSize>
#include <QPixmap>
#include "../../Segmentor/segmentor.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    s(new Segmentor),
    have_image(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete s;
    delete ui;
}

// Choose file and display it
void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "~/", "All files (*.*)");
    QMessageBox::information(this, tr("File Name"),filename);

    if (!filename.isEmpty()) {

        QImage image(filename);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Failed"),tr("Cannot load %1.").arg(filename));
            return;
        }

        QPixmap pix_image = QPixmap::fromImage(image);
        ui->label_pic->setPixmap(pix_image);
        ui->label_pic->show();
        have_image = true;
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(have_image){

    }
}

// Convers a cv::Mat object to QImage for display in the GUI
void MainWindow::matToImage(cv::Mat &_mat, QImage &_res){
    cv::Mat temp;
    cv::cvtColor(_mat, temp, cv::COLOR_BGR2RGB);

    _res = QImage((uchar*)temp.data,temp.cols,temp.rows,temp.step, QImage::Format_RGB888);
}
