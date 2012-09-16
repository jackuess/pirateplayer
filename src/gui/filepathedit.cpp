#include "filepathedit.h"

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QFileInfo>
#include <QFileDialog>

QStringList FilePathEdit::filter = QStringList() << "Alla filer (*.*)" << "Flashvideo (*.flv)" << "Untertexter (*.srt)";

FilePathEdit::FilePathEdit(QString dir, QWidget *parent) :
    QWidget(parent)
{
    defaultDir = dir;

    layout = new QHBoxLayout(this);
    editPath = new QLineEdit(this);
    buttonBrowse = new QPushButton(QString::fromUtf8("Bläddra"), this);

    connect(editPath, SIGNAL(textChanged(QString)), this, SLOT(pathChanged(QString)));
    connect(buttonBrowse, SIGNAL(clicked()), this, SLOT(browse()));

    layout->addWidget(editPath);
    layout->addWidget(buttonBrowse);

    QTimer::singleShot(0, this, SLOT(setToHomeLocation()));
}

FilePathEdit::~FilePathEdit() {
    ;
}

QString FilePathEdit::filePath() {
    return editPath->text();
}

bool FilePathEdit::isValid() {
    QFileInfo info(filePath());
    return !info.isDir();
}

void FilePathEdit::setFilePath(QString newFilePath) {
    editPath->setText(newFilePath);
}

void FilePathEdit::pathChanged(QString newPath) {
    QFileInfo info(newPath);

    if (info.isDir())
        emit dirChanged(newPath);
    else
        emit dirChanged(info.path());

    emit pathValid(!info.isDir());
}

void FilePathEdit::setToHomeLocation() {
    editPath->setText(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + QDir::separator());
}

void FilePathEdit::browse() {
    QString chosenFilter;
    QString filePath = QFileDialog::getSaveFileName(this, "Spara fil", defaultDir, filter[0], &chosenFilter, QFileDialog::DontConfirmOverwrite);
    if (filePath != "")
        editPath->setText(filePath);
}

QString FilePathEdit::extFromFilter(QString f) {
    QString r = f.right(f.length()-f.lastIndexOf('.'));
    r.truncate(r.length()-1);
    return r;
}

void FilePathEdit::setDir(QString newDir) {
    QFileInfo info(editPath->text());
    if (!newDir.endsWith(QDir::separator()))
        newDir += QDir::separator();
    editPath->setText(newDir + info.fileName());
}
