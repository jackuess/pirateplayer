#ifndef FILEPATHEDIT_H
#define FILEPATHEDIT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDesktopServices>

class FilePathEdit : public QWidget
{
    Q_OBJECT
public:
    explicit FilePathEdit(QString dir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation), QWidget *parent = 0);

    QString filePath();
    void setFilePath(QString newFilePath);
    bool isValid();

    enum DIALOG_FILTER {
        Video,
        Subtitle
    };
    
signals:
    void pathValid(bool valid);
    void dirChanged(QString newDir);
    
public slots:
    void setDir(QString newDir);

private:
    QHBoxLayout *layout;
    QLineEdit *editPath;
    QPushButton *buttonBrowse;
    QString defaultDir;

    static QStringList filter;
    static QString extFromFilter(QString f);

private slots:
    void pathChanged(QString newPath);
    void setToDefault();
    void browse();
};

#endif // FILEPATHEDIT_H
