#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "crc.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool ifFileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::on_generateCRCBtn_clicked()
{
    // CRC-32
    // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  0  0  1  1  0  1  1  0  1  1  1
    ui->infoLabel->setText("Computing...");
    if(ifFileExists(ui->filepathText->text())){
        try{
            CRC crc;
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            QString crcsum = QString::fromStdString(crc.countCRC(filepath));
            ui->crcText->setText(crcsum);
            ui->infoLabel->setText("Your CRC32 checksum has been computed: " + ui->crcText->text());
        }
        catch(std::exception e){
            ui->infoLabel->setText("Something got wrong, try again...");
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
}

void MainWindow::on_checkCRCBtn_clicked()
{
    // CRC-32
    // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  0  0  1  1  0  1  1  0  1  1  1
    ui->infoLabel->setText("Computing...");
    if(ifFileExists(ui->filepathText->text())){
        try{
            CRC crc;
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            QString crcsum = QString::fromStdString(crc.countCRC(filepath));

            if(crcsum == ui->crcText->text()){
                ui->infoLabel->setText("CRC32 checksum is correct!");
            }
            else{
                ui->infoLabel->setText("CRC32 checksum is not correct!\nCorrect CRC32: " + crcsum);
            }
        }
        catch(std::exception e){
            ui->infoLabel->setText("Something got wrong, try again...");
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
}

void MainWindow::on_filepathBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "./",
                "All Files (*.*);;Text File (*.txt);;Music file(*.mp3)"
                );
    ui->filepathText->setText(filename);
}
