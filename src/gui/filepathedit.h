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
    explicit FilePathEdit(QWidget *parent = 0);

    QString filePath();
    void setFilePath(const QString &newFilePath);
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

    static QStringList filter;
    static QString extFromFilter(QString f);

private slots:
    void pathChanged(QString newPath);
    void browse();
};

#endif // FILEPATHEDIT_H
