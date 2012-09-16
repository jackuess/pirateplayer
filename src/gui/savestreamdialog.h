#ifndef SAVESTREAMDIALOG_H
#define SAVESTREAMDIALOG_H

#include "../models/streamtablemodel.h"
#include "filepathedit.h"

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

class SaveStreamDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaveStreamDialog(StreamTableModel *model, QHash<QString, QVariant> settings, QWidget *parent = 0);
    ~SaveStreamDialog();

    QString getFileName();
    QString getSubFileName();
    QUrl getUrl();
    QUrl getSubUrl();
    bool downloadSubs();

    
signals:
    
public slots:
    
private:
    FilePathEdit *editFileName;
    FilePathEdit *editSubFileName;
    QComboBox *comboQuality;
    QCheckBox *checkSubtitles;
    QLineEdit *editStreamUrl;
    QLineEdit *editSubUrl;
    QPushButton *buttonSubmit;
    QHash<QString,QVariant> userSettings;

private slots:
    void comboActivated(int index);
    void enableSubmit();
    void play();
    void checkOverWrite();
};

#endif // SAVESTREAMDIALOG_H
