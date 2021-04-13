#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    file1=new FileInfoExtractor(this);
    time1=new QDateTime();
    time_file=new QDateTime();
    crea_file=new QDateTime();
    serial=0;
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->lineEdit_5->setPlaceholderText(tr("格式举例(输入法英文)：2020-7-2 1:5:1"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//搜索源文件
void MainWindow::on_pushButton_2_clicked()
{
    QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"\\";//桌面路径
    QDir dir;
//    QFileDialog::Options options;
    //       options  |= QFileDialog::DontUseNativeDialog;//和系统画风不一致
    QString selectedFilter;

    //QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("选择路径哎嘿"), location, QObject::tr("All files(*.*)"), &selectedFilter, options);
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);//去掉这句就可以用来查找文件
    QString fileName = fileDialog->getExistingDirectory(this,QObject::tr("选择路径哎嘿"),location);

    if (!fileName.isNull()) {
        file1->bjk_filename.clear();
        file1->bjk_filecrea.clear();
        file1->bjk_filesize.clear();
        file1->bjk_filemodi.clear();
        file1->bjk_filelare.clear();
        serial=0;
        ui->lineEdit_4->setText(QDir::toNativeSeparators(fileName));
        file1->FileInfoAnalyzer(fileName);
        ui->pushButton_3->setEnabled(true);
        ui->lineEdit->setText(file1->bjk_filename.at(serial));
        ui->lineEdit_2->setText(file1->bjk_filecrea.at(serial));
        int num=file1->bjk_filesize.at(serial).toInt()/1024;
        ui->lineEdit_3->setText(QString(tr("%L1 (第%2/共%3个文件)").arg(num).arg(serial+1).arg(file1->bjk_filename.size())));
        serial=1;
    }
}
//下一个按钮
void MainWindow::on_pushButton_3_clicked()
{
    if(serial==file1->bjk_filename.size()){
        serial=0;
        //注销下面这句就可以无限循环查看文件夹里的文件
        //ui->pushButton_3->setEnabled(false);
    }
    ui->lineEdit->setText(file1->bjk_filename.at(serial));
    ui->lineEdit_2->setText(file1->bjk_filecrea.at(serial));
    int num=file1->bjk_filesize.at(serial).toInt()/1024;
    ui->lineEdit_3->setText(QString(tr("%L1 (第%2/共%3个文件)").arg(num).arg(serial+1).arg(file1->bjk_filename.size())));
    serial++;
}
//搜索源文件
void MainWindow::on_lineEdit_4_returnPressed()
{
    QString path =ui->lineEdit_4->text();
    path= QDir::toNativeSeparators(path);
    if (!path.isNull()) {
        file1->bjk_filename.clear();
        file1->bjk_filecrea.clear();
        file1->bjk_filesize.clear();
        file1->bjk_filemodi.clear();
        file1->bjk_filelare.clear();
        serial=0;
        file1->FileInfoAnalyzer(path);
        ui->pushButton_3->setEnabled(true);
        ui->lineEdit->setText(file1->bjk_filename.at(serial));
        ui->lineEdit_2->setText(file1->bjk_filecrea.at(serial));
        int num=file1->bjk_filesize.at(serial).toInt()/1024;
        ui->lineEdit_3->setText(QString(tr("%1").arg(num)));
        serial=1;
    }
}
//录入备份日期
void MainWindow::on_lineEdit_5_returnPressed()
{
    QString time_back =ui->lineEdit_5->text();
    *time1=QDateTime::fromString(time_back, "yyyy-M-d h:m:s");
    QMessageBox::about(this, "温馨提示", "增量备份日期为："+time1->toString());
    ui->pushButton_4->setEnabled(true);
}
//录入备份日期
void MainWindow::on_pushButton_clicked()
{
    QString time_back =ui->lineEdit_5->text();
    *time1=QDateTime::fromString(time_back, "yyyy-M-d h:m:s");
    QMessageBox::about(this, "温馨提示","增量备份日期为："+time1->toString());
    ui->pushButton_4->setEnabled(true);
}
//备份文件按钮
void MainWindow::on_pushButton_4_clicked()
{
    on_lineEdit_4_returnPressed();
    QString path =ui->lineEdit_4->text();
    QString distpath=ui->lineEdit_6->text();
    QString path1,distpath1;
    ui->pushButton_4->setEnabled(false);
    for (int i = 0; i <file1->bjk_filename.size(); ++i) {
    *time_file= QDateTime::fromString(file1->bjk_filemodi.at(i), "yyyy-M-d h:m:s");
    *crea_file= QDateTime::fromString(file1->bjk_filecrea.at(i), "yyyy-M-d h:m:s");
        if(*time1<=*time_file || *time1<=*crea_file)
        {
            path1=path+"/"+file1->bjk_filename.at(i);
            distpath1=distpath+"\\"+file1->bjk_filename.at(i);
            qDebug()<<distpath1;
            file1->copyDir(path1,distpath1,true);
        }
    }
    QMessageBox::about(this, "温馨提示","备份已完成。");
}
//搜索目的文件夹
void MainWindow::on_pushButton_5_clicked()
{
    QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"\\";//桌面路径
    QDir dir;
//    QFileDialog::Options options;
    //       options  |= QFileDialog::DontUseNativeDialog;//和系统画风不一致
    QString selectedFilter;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::Directory);//去掉这句就可以用来查找文件
    QString fileName = fileDialog->getExistingDirectory(this,QObject::tr("选择路径哎嘿"),location);

    if (!fileName.isNull()) {
        ui->lineEdit_6->setText(QDir::toNativeSeparators(fileName));
    }
}
