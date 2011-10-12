/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Oct 11 15:54:50 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit_URL;
    QProgressBar *progressBar;
    QPushButton *pushButton_Fetch;
    QPushButton *pushButton_Cancel;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox_Stream;
    QFrame *line;
    QPushButton *pushButton_Download;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(493, 121);
        MainWindow->setMinimumSize(QSize(493, 121));
        MainWindow->setMaximumSize(QSize(493, 121));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lineEdit_URL = new QLineEdit(centralWidget);
        lineEdit_URL->setObjectName(QString::fromUtf8("lineEdit_URL"));
        lineEdit_URL->setGeometry(QRect(60, 10, 361, 23));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setGeometry(QRect(11, 90, 421, 23));
        progressBar->setValue(0);
        pushButton_Fetch = new QPushButton(centralWidget);
        pushButton_Fetch->setObjectName(QString::fromUtf8("pushButton_Fetch"));
        pushButton_Fetch->setGeometry(QRect(429, 10, 61, 24));
        pushButton_Cancel = new QPushButton(centralWidget);
        pushButton_Cancel->setObjectName(QString::fromUtf8("pushButton_Cancel"));
        pushButton_Cancel->setEnabled(false);
        pushButton_Cancel->setGeometry(QRect(440, 90, 51, 24));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 6, 51, 31));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 41, 21));
        comboBox_Stream = new QComboBox(centralWidget);
        comboBox_Stream->setObjectName(QString::fromUtf8("comboBox_Stream"));
        comboBox_Stream->setEnabled(false);
        comboBox_Stream->setGeometry(QRect(60, 40, 341, 24));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 70, 471, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pushButton_Download = new QPushButton(centralWidget);
        pushButton_Download->setObjectName(QString::fromUtf8("pushButton_Download"));
        pushButton_Download->setEnabled(false);
        pushButton_Download->setGeometry(QRect(410, 40, 81, 24));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Pirateplayer", 0, QApplication::UnicodeUTF8));
        pushButton_Fetch->setText(QApplication::translate("MainWindow", "H\303\244mta", 0, QApplication::UnicodeUTF8));
        pushButton_Cancel->setText(QApplication::translate("MainWindow", "Abryt", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Adress", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Str\303\266m", 0, QApplication::UnicodeUTF8));
        pushButton_Download->setText(QApplication::translate("MainWindow", "Ladda ner", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
