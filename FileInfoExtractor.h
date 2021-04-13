#ifndef FILEINFOEXTRACTOR_H
#define FILEINFOEXTRACTOR_H
#include<QFileInfoList>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <windows.h>
#include <QDateTime>
#include<QDir>

class FileInfoExtractor : public QObject
{
    Q_OBJECT

public:
    FileInfoExtractor(QObject *parent=nullptr);
    ~FileInfoExtractor();
public:
    QVector<QString> bjk_filename;
    QVector<QString> bjk_filesize;
    QVector<QString> bjk_filecrea;
    QVector<QString> bjk_filemodi;
    QVector<QString> bjk_filelare;
    //获取某一目录下的文件列表
    QFileInfoList GetFileList(QString& path);

    //分析指定目录下的文件信息
    void FileInfoAnalyzer(QString& ftp_dir);

    //递归复制文件
    bool copyDir(const QString& source,const QString& destination,bool override);

    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);

    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);

};
#endif // FILEINFOEXTRACTOR_H
