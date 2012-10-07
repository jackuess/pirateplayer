#include "filepathedit.h"

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QFileInfo>
#include <QFileDialog>

QStringList FilePathEdit::filter = QStringList() << "Alla filer (*.*)" << "Flashvideo (*.flv)" << "Untertexter (*.srt)";

FilePathEdit::FilePathEdit(QWidget *parent) :
    QWidget(parent)
{
    layout = new QHBoxLayout(this);
    editPath = new QLineEdit(this);
    buttonBrowse = new QPushButton(QString::fromUtf8("Bläddra"), this);

    connect(editPath, SIGNAL(textChanged(QString)), this, SLOT(pathChanged(QString)));
    connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(browse()));

    layout->addWidget(editPath);
    layout->addWidget(buttonBrowse);
    layout->setContentsMargins(0, 0, 0, 0);
}

QString FilePathEdit::filePath() {
    return QDir::fromNativeSeparators(editPath->text());
}

bool FilePathEdit::isValid() {
    QFileInfo info(filePath());
    return !info.isDir();
}

void FilePathEdit::setFilePath(const QString &newFilePath) {
    editPath->setText(QDir::toNativeSeparators(newFilePath));
}

void FilePathEdit::pathChanged(QString newPath) {
    QFileInfo info(newPath);

    if (info.isDir())
        emit dirChanged(newPath);
    else
        emit dirChanged(info.path());

    emit pathValid(!info.isDir());
}

void FilePathEdit::browse() {
    QString chosenFilter;
    QDir dir(editPath->text());
    QString filePath = QFileDialog::getSaveFileName(this, "Spara fil", dir.path(), filter[0], &chosenFilter, QFileDialog::DontConfirmOverwrite);
    if (filePath != "")
        setFilePath(filePath);
}


void FilePathEdit::setDir(QString newDir) {
    QFileInfo info(editPath->text());
    if (!newDir.endsWith(QDir::separator()))
        newDir += QDir::separator();

    if (info.isDir())
        setFilePath(newDir);
    else
        setFilePath(newDir + info.fileName());
}
