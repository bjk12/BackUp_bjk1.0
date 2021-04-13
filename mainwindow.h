#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FileInfoExtractor.h"
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
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_4_returnPressed();

    void on_lineEdit_5_returnPressed();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    int serial;
    QDateTime* time1;
    QDateTime* time_file;
    QDateTime* crea_file;
    Ui::MainWindow *ui;
    FileInfoExtractor* file1;
};

#endif // MAINWINDOW_H
