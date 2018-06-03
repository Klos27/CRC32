#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "crc.h"
#include <iostream>
#include <time.h>
#include <boost/crc.hpp>  // for boost::crc_32_type

#include <cstdlib>    // for EXIT_SUCCESS, EXIT_FAILURE
#include <exception>  // for std::exception
#include <fstream>    // for std::ifstream
#include <ios>        // for std::ios_base, etc.
#include <ostream>    // for std::endl
#include <sstream>

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

double diffclock(clock_t clockStart, clock_t clockEnd)
{
    double diffticks = clockEnd - clockStart;
    double diffms = diffticks / CLOCKS_PER_SEC;
    return diffms;
}

void MainWindow::on_generateCRCBtn_clicked()
{
    clock_t timeStart, timeEnd;
    timeStart = clock();
    // CRC-32
    // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  0  0  1  1  0  1  1  0  1  1  1
    ui->infoLabel->setText("Generating CRC32...");
    ui->checkCRCBtn->setDisabled(true);
    ui->generateCRCBtn->setDisabled(true);
    ui->boostCheckBtn->setDisabled(true);
    ui->boostGenBtn->setDisabled(true);
    ui->generateCRCTabBtn->setDisabled(true);
    ui->checkCRCTabBtn->setDisabled(true);
    QWidget::repaint();

    if(ifFileExists(ui->filepathText->text())){
        try{
            CRC crc;
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            QString crcsum = QString::fromStdString(crc.countCRC(filepath));
            ui->crcText->setText(crcsum.toUpper());
            ui->infoLabel->setText("Your CRC32 checksum has been computed: " + ui->crcText->text());
        }
        catch(std::exception e){
            ui->checkCRCBtn->setDisabled(false);
            ui->generateCRCBtn->setDisabled(false);
            ui->boostCheckBtn->setDisabled(false);
            ui->boostGenBtn->setDisabled(false);
            ui->generateCRCTabBtn->setDisabled(false);
            ui->checkCRCTabBtn->setDisabled(false);
            ui->infoLabel->setText("Something got wrong, try again...");
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
    ui->checkCRCBtn->setDisabled(false);
    ui->generateCRCBtn->setDisabled(false);
    ui->boostCheckBtn->setDisabled(false);
    ui->boostGenBtn->setDisabled(false);
    ui->generateCRCTabBtn->setDisabled(false);
    ui->checkCRCTabBtn->setDisabled(false);
    timeEnd = clock();
    double timeElapsed = diffclock(timeStart, timeEnd);
    std::string timeString = "Time elapsed: " + std::to_string(timeElapsed) + " [sec]";
    ui->timeLabel->setText(QString::fromStdString(timeString));
}

void MainWindow::on_checkCRCBtn_clicked()
{
    clock_t timeStart, timeEnd;
    timeStart = clock();
    // CRC-32
    // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  0  0  1  1  0  1  1  0  1  1  1
    ui->infoLabel->setText("Checking CRC32...");
    ui->checkCRCBtn->setDisabled(true);
    ui->generateCRCBtn->setDisabled(true);
    ui->boostCheckBtn->setDisabled(true);
    ui->boostGenBtn->setDisabled(true);
    ui->generateCRCTabBtn->setDisabled(true);
    ui->checkCRCTabBtn->setDisabled(true);
    QWidget::repaint();
    if(ifFileExists(ui->filepathText->text())){
        try{
            CRC crc;
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            QString crcsum = QString::fromStdString(crc.countCRC(filepath));
            if(crcsum.toUpper() == ui->crcText->text().toUpper()){
                ui->infoLabel->setText("CRC32 checksum is correct!");
            }
            else{
                ui->infoLabel->setText("CRC32 checksum is not correct!\nCorrect CRC32: " + crcsum.toUpper());
            }
        }
        catch(std::exception e){
            ui->infoLabel->setText("Something got wrong, try again...");
                ui->checkCRCBtn->setDisabled(false);
                ui->generateCRCBtn->setDisabled(false);
                ui->boostCheckBtn->setDisabled(false);
                ui->boostGenBtn->setDisabled(false);
                ui->generateCRCTabBtn->setDisabled(false);
                ui->checkCRCTabBtn->setDisabled(false);
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
    ui->checkCRCBtn->setDisabled(false);
    ui->generateCRCBtn->setDisabled(false);
    ui->boostCheckBtn->setDisabled(false);
    ui->boostGenBtn->setDisabled(false);
    ui->generateCRCTabBtn->setDisabled(false);
    ui->checkCRCTabBtn->setDisabled(false);
    timeEnd = clock();
    double timeElapsed = diffclock(timeStart, timeEnd);
    std::string timeString = "Time elapsed: " + std::to_string(timeElapsed) + " [sec]";
    ui->timeLabel->setText(QString::fromStdString(timeString));
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

void MainWindow::on_boostGenBtn_clicked()
{
    clock_t timeStart, timeEnd;
    timeStart = clock();
    ui->infoLabel->setText("Generating Boost CRC32...");
    ui->checkCRCBtn->setDisabled(true);
    ui->generateCRCBtn->setDisabled(true);
    ui->boostCheckBtn->setDisabled(true);
    ui->boostGenBtn->setDisabled(true);
    ui->generateCRCTabBtn->setDisabled(true);
    ui->checkCRCTabBtn->setDisabled(true);
    QWidget::repaint();

    if(ifFileExists(ui->filepathText->text())){
        try{
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            std::streamsize const  buffer_size = 1024;
            boost::crc_32_type  result;
            std::ifstream  ifs( filepath, std::ios_base::binary );
            if ( ifs ){
                do{
                    char buffer[ buffer_size ];
                    ifs.read( buffer, buffer_size );
                    result.process_bytes( buffer, ifs.gcount() );
                } while ( ifs );
            }

            std::stringstream crc32;
            crc32 << std::hex << std::uppercase << result.checksum();


            QString crcsum = QString::fromStdString(crc32.str());
            ui->crcText->setText(crcsum.toUpper());
            ui->infoLabel->setText("Your CRC32 checksum has been computed: " + ui->crcText->text());
        }
        catch(std::exception e){
            ui->checkCRCBtn->setDisabled(false);
            ui->generateCRCBtn->setDisabled(false);
            ui->boostCheckBtn->setDisabled(false);
            ui->boostGenBtn->setDisabled(false);
            ui->generateCRCTabBtn->setDisabled(false);
            ui->checkCRCTabBtn->setDisabled(false);
            ui->infoLabel->setText("Something got wrong, try again...");
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
    ui->checkCRCBtn->setDisabled(false);
    ui->generateCRCBtn->setDisabled(false);
    ui->boostCheckBtn->setDisabled(false);
    ui->boostGenBtn->setDisabled(false);
    ui->generateCRCTabBtn->setDisabled(false);
    ui->checkCRCTabBtn->setDisabled(false);
    timeEnd = clock();
    double timeElapsed = diffclock(timeStart, timeEnd);
    std::string timeString = "Time elapsed: " + std::to_string(timeElapsed) + " [sec]";
    ui->timeLabel->setText(QString::fromStdString(timeString));
}

void MainWindow::on_boostCheckBtn_clicked()
{
    clock_t timeStart, timeEnd;
    timeStart = clock();
    ui->infoLabel->setText("Generating Boost CRC32...");
    ui->checkCRCBtn->setDisabled(true);
    ui->generateCRCBtn->setDisabled(true);
    ui->boostCheckBtn->setDisabled(true);
    ui->boostGenBtn->setDisabled(true);
    ui->generateCRCTabBtn->setDisabled(true);
    ui->checkCRCTabBtn->setDisabled(true);
    QWidget::repaint();

    if(ifFileExists(ui->filepathText->text())){
        try{
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            std::streamsize const  buffer_size = 1024;
            boost::crc_32_type  result;
            std::ifstream  ifs( filepath, std::ios_base::binary );
            if ( ifs ){
                do{
                    char buffer[ buffer_size ];
                    ifs.read( buffer, buffer_size );
                    result.process_bytes( buffer, ifs.gcount() );
                } while ( ifs );
            }

            std::stringstream crc32;
            crc32 << std::hex << std::uppercase << result.checksum();


            QString crcsum = QString::fromStdString(crc32.str());

            if(crcsum.toUpper() == ui->crcText->text().toUpper()){
                ui->infoLabel->setText("CRC32 checksum is correct!");
            }
            else{
                ui->infoLabel->setText("CRC32 checksum is not correct!\nCorrect CRC32: " + crcsum);
            }
        }
        catch(std::exception e){
            ui->checkCRCBtn->setDisabled(false);
            ui->generateCRCBtn->setDisabled(false);
            ui->boostCheckBtn->setDisabled(false);
            ui->boostGenBtn->setDisabled(false);
            ui->generateCRCTabBtn->setDisabled(false);
            ui->checkCRCTabBtn->setDisabled(false);
            ui->infoLabel->setText("Something got wrong, try again...");
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
    ui->checkCRCBtn->setDisabled(false);
    ui->generateCRCBtn->setDisabled(false);
    ui->boostCheckBtn->setDisabled(false);
    ui->boostGenBtn->setDisabled(false);
    ui->generateCRCTabBtn->setDisabled(false);
    ui->checkCRCTabBtn->setDisabled(false);
    timeEnd = clock();
    double timeElapsed = diffclock(timeStart, timeEnd);
    std::string timeString = "Time elapsed: " + std::to_string(timeElapsed) + " [sec]";
    ui->timeLabel->setText(QString::fromStdString(timeString));
}

void MainWindow::on_generateCRCTabBtn_clicked()
{
    clock_t timeStart, timeEnd;
    timeStart = clock();
    // CRC-32
    // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  0  0  1  1  0  1  1  0  1  1  1
    ui->infoLabel->setText("Generating CRC32 via Tab...");
    ui->checkCRCBtn->setDisabled(true);
    ui->generateCRCBtn->setDisabled(true);
    ui->boostCheckBtn->setDisabled(true);
    ui->boostGenBtn->setDisabled(true);
    ui->generateCRCTabBtn->setDisabled(true);
    ui->checkCRCTabBtn->setDisabled(true);
    QWidget::repaint();

    if(ifFileExists(ui->filepathText->text())){
        try{
            CRC crc;
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            QString crcsum = QString::fromStdString(crc.countCRCviaTab(filepath));
            ui->crcText->setText(crcsum.toUpper());
            ui->infoLabel->setText("Your CRC32 checksum has been computed: " + ui->crcText->text());
        }
        catch(std::exception e){
            ui->checkCRCBtn->setDisabled(false);
            ui->generateCRCBtn->setDisabled(false);
            ui->boostCheckBtn->setDisabled(false);
            ui->boostGenBtn->setDisabled(false);
            ui->generateCRCTabBtn->setDisabled(false);
            ui->checkCRCTabBtn->setDisabled(false);
            ui->infoLabel->setText("Something got wrong, try again...");
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
    ui->checkCRCBtn->setDisabled(false);
    ui->generateCRCBtn->setDisabled(false);
    ui->boostCheckBtn->setDisabled(false);
    ui->boostGenBtn->setDisabled(false);
    ui->generateCRCTabBtn->setDisabled(false);
    ui->checkCRCTabBtn->setDisabled(false);
    timeEnd = clock();
    double timeElapsed = diffclock(timeStart, timeEnd);
    std::string timeString = "Time elapsed: " + std::to_string(timeElapsed) + " [sec]";
    ui->timeLabel->setText(QString::fromStdString(timeString));
}

void MainWindow::on_checkCRCTabBtn_clicked()
{
    clock_t timeStart, timeEnd;
    timeStart = clock();
    // CRC-32
    // 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    //	1  0  0  0  0  0  1  0  0  1  1  0  0  0  0  0  1  0  0  0  1  1  0  0  1  1  0  1  1  0  1  1  1
    ui->infoLabel->setText("Checking CRC32 via Tab...");
    ui->checkCRCBtn->setDisabled(true);
    ui->generateCRCBtn->setDisabled(true);
    ui->boostCheckBtn->setDisabled(true);
    ui->boostGenBtn->setDisabled(true);
    ui->generateCRCTabBtn->setDisabled(true);
    ui->checkCRCTabBtn->setDisabled(true);
    QWidget::repaint();
    if(ifFileExists(ui->filepathText->text())){
        try{
            CRC crc;
            std::string filepath = ui->filepathText->text().toUtf8().constData();
            QString crcsum = QString::fromStdString(crc.countCRCviaTab(filepath));
            if(crcsum.toUpper() == ui->crcText->text().toUpper()){
                ui->infoLabel->setText("CRC32 checksum is correct!");
            }
            else{
                ui->infoLabel->setText("CRC32 checksum is not correct!\nCorrect CRC32: " + crcsum.toUpper());
            }
        }
        catch(std::exception e){
            ui->infoLabel->setText("Something got wrong, try again...");
                ui->checkCRCBtn->setDisabled(false);
                ui->generateCRCBtn->setDisabled(false);
                ui->boostCheckBtn->setDisabled(false);
                ui->boostGenBtn->setDisabled(false);
                ui->generateCRCTabBtn->setDisabled(false);
                ui->checkCRCTabBtn->setDisabled(false);
        }
    }
    else{
        ui->infoLabel->setText("This file is not correct!");
        QMessageBox::information(this, tr("Warning"), "This file is not correct!");
    }
    ui->checkCRCBtn->setDisabled(false);
    ui->generateCRCBtn->setDisabled(false);
    ui->boostCheckBtn->setDisabled(false);
    ui->boostGenBtn->setDisabled(false);
    ui->generateCRCTabBtn->setDisabled(false);
    ui->checkCRCTabBtn->setDisabled(false);
    timeEnd = clock();
    double timeElapsed = diffclock(timeStart, timeEnd);
    std::string timeString = "Time elapsed: " + std::to_string(timeElapsed) + " [sec]";
    ui->timeLabel->setText(QString::fromStdString(timeString));
}
