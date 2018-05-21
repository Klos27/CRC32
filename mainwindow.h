#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generateCRCBtn_clicked();

    void on_checkCRCBtn_clicked();

    void on_filepathBtn_clicked();

    void on_boostGenBtn_clicked();

    void on_boostCheckBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
