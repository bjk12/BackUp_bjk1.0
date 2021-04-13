#include "FileInfoExtractor.h"

FileInfoExtractor::FileInfoExtractor(QObject *parent)
    : QObject(parent)
{
}

FileInfoExtractor::~FileInfoExtractor()
{
}

//拷贝文件：
bool FileInfoExtractor::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

//拷贝文件夹：
bool FileInfoExtractor::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}
//获取某一目录下的文件列表
QFileInfoList FileInfoExtractor::GetFileList(QString& path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        file_list.append(folder_list);
        //递归查子文件
//    for (int i = 0; i != folder_list.size(); i++) {
//        QString name = folder_list.at(i).absoluteFilePath();
//        QFileInfoList child_file_list = GetFileList(name);
//        file_list.append(child_file_list);
//    }

    return file_list;
}



//分析指定目录下的文件信息

void FileInfoExtractor::FileInfoAnalyzer(QString& ftp_dir)
{
    QDir dir(ftp_dir);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list_sub = GetFileList(ftp_dir);

    for (int i = 0; i < list_sub.size(); ++i) {
        QString filepath;
        QString full_path;
        QFileInfo fileInfo = list_sub.at(i);
        filepath.append(fileInfo.path());
        filepath += "/" + fileInfo.fileName();

        if (!filepath.isEmpty()){
            QFile file(filepath);
//取消注释则不显示文件夹信息，只显示文件信息
//            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))    {
//                return;
//            }

            QFileInfo info(filepath);
            if (info.exists())    {
                bjk_filename.append(QString(tr("%1").arg(info.fileName())));
                bjk_filesize.append(QString(tr("%1").arg(info.size())));
                QDateTime dt = info.birthTime();
                bjk_filecrea.append(QString(tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"))));
                dt = info.lastModified();
                bjk_filemodi.append(QString(tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"))));
//                QString file_id = tr("%1").arg(i);
                dt = info.lastRead();
                bjk_filelare.append(QString(tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"))));
            }
        }
    }
}

bool FileInfoExtractor::copyDir(const QString& source,const QString& destination,bool override)
{
    QFileInfo fileInfo(source);
    if (fileInfo.isFile() || fileInfo.isSymLink())
    {
        QDir *createfile     = new QDir;
        bool exist = createfile->exists(destination);
        if (exist){
                createfile->remove(destination);
        }
        QString srcPath = QDir::toNativeSeparators(source);
        QString dstPath = QDir::toNativeSeparators(destination);
        if (override)
        {
            QFile::setPermissions(dstPath, QFile::WriteOwner);
        }
        QFile::copy(srcPath, dstPath);
        return true;
    }
    else if (fileInfo.isDir())
    {
        QDir directory(source);
            if (!directory.exists())
            {
                return false;
            }
        QString srcPath = QDir::toNativeSeparators(source);
        if (!srcPath.endsWith(QDir::separator()))
            srcPath += QDir::separator();
        QString dstPath = QDir::toNativeSeparators(destination);
        if (!dstPath.endsWith(QDir::separator()))
            dstPath += QDir::separator();
        //重点！！copy文件 操作前一定要保证路径存在，不然复制不会成功
        QDir dstDir(dstPath);
        dstDir.mkpath(dstPath);
        bool error = false;
        QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        for (QStringList::size_type i=0; i != fileNames.size(); ++i)
        {
            QString fileName = fileNames.at(i);
            QString srcFilePath = srcPath + fileName;
            QString dstFilePath = dstPath + fileName;
            QFileInfo fileInfo(srcFilePath);
            if (fileInfo.isFile() || fileInfo.isSymLink())
            {
                QDir *createfile     = new QDir;
                bool exist = createfile->exists(dstFilePath);
                if (exist){
                        createfile->remove(dstFilePath);
                }
                if (override)
                {
                    QFile::setPermissions(dstFilePath, QFile::WriteOther);
                }
                QFile::copy(srcFilePath, dstFilePath);
            }
            else if (fileInfo.isDir())
            {
                //重点！！copy操作前一定要保证路径存在，不然复制不会成功
                QDir dstDir(dstFilePath);
                dstDir.mkpath(dstFilePath);
                if (!copyDir(srcFilePath, dstFilePath, override))
                {
                    error = true;
                }
            }
        }

        return !error;
    }
    return true;
}
