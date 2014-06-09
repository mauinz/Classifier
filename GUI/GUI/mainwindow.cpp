#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QSize>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setText("Choose Image");

}

MainWindow::~MainWindow()
{
    delete ui;
}

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
        QSize pixSize = ui->myLabel->pixmap()->size();

        ui->myLabel->setPixmap((QPixmap::fromImage(image)).scaled(pixSize,Qt::KeepAspectRatio));
        //QSize pixSize = ui->myLabel->pixmap()->size();
        //pixSize.scale(size(), Qt::KeepAspectRatio);
        //ui->myLabel->setFixedSize(pixSize);
        ui->myLabel->show();

    }

}
