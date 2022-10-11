/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionopenFiles;
    QAction *actioncaptureFullScreen;
    QAction *actioncaptureSelRect;
    QAction *actionoutputToEdit;
    QAction *actionset;
    QAction *actionselElipseCapture;
    QAction *actionfixedRectCapture;
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(451, 62);
        actionopenFiles = new QAction(MainWindow);
        actionopenFiles->setObjectName(QStringLiteral("actionopenFiles"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionopenFiles->setIcon(icon);
        actioncaptureFullScreen = new QAction(MainWindow);
        actioncaptureFullScreen->setObjectName(QStringLiteral("actioncaptureFullScreen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/window2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actioncaptureFullScreen->setIcon(icon1);
        actioncaptureSelRect = new QAction(MainWindow);
        actioncaptureSelRect->setObjectName(QStringLiteral("actioncaptureSelRect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/selrec.png"), QSize(), QIcon::Normal, QIcon::Off);
        actioncaptureSelRect->setIcon(icon2);
        actionoutputToEdit = new QAction(MainWindow);
        actionoutputToEdit->setObjectName(QStringLiteral("actionoutputToEdit"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/output.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionoutputToEdit->setIcon(icon3);
        actionset = new QAction(MainWindow);
        actionset->setObjectName(QStringLiteral("actionset"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/set.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionset->setIcon(icon4);
        actionselElipseCapture = new QAction(MainWindow);
        actionselElipseCapture->setObjectName(QStringLiteral("actionselElipseCapture"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/selelipse.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionselElipseCapture->setIcon(icon5);
        actionfixedRectCapture = new QAction(MainWindow);
        actionfixedRectCapture->setObjectName(QStringLiteral("actionfixedRectCapture"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/rec.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionfixedRectCapture->setIcon(icon6);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(false);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 451, 17));
        MainWindow->setMenuBar(menuBar);

        mainToolBar->addAction(actionopenFiles);
        mainToolBar->addAction(actioncaptureFullScreen);
        mainToolBar->addAction(actioncaptureSelRect);
        mainToolBar->addAction(actionfixedRectCapture);
        mainToolBar->addAction(actionselElipseCapture);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionoutputToEdit);
        mainToolBar->addAction(actionset);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionopenFiles->setText(QApplication::translate("MainWindow", "openFiles", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionopenFiles->setToolTip(QApplication::translate("MainWindow", "\345\234\250\347\274\226\350\276\221\345\231\250\346\211\223\345\274\200\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actioncaptureFullScreen->setText(QApplication::translate("MainWindow", "captureFullScreen", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actioncaptureFullScreen->setToolTip(QApplication::translate("MainWindow", "\346\210\252\345\217\226\345\205\250\345\261\217", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actioncaptureSelRect->setText(QApplication::translate("MainWindow", "captureSelRect", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actioncaptureSelRect->setToolTip(QApplication::translate("MainWindow", "\346\215\225\350\216\267\347\237\251\345\275\242\345\214\272\345\237\237", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionoutputToEdit->setText(QApplication::translate("MainWindow", "outputToEdit", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionoutputToEdit->setToolTip(QApplication::translate("MainWindow", "\350\276\223\345\207\272 -> \347\274\226\350\276\221\345\231\250", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionset->setText(QApplication::translate("MainWindow", "set", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionset->setToolTip(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionselElipseCapture->setText(QApplication::translate("MainWindow", "selElipseCapture", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionselElipseCapture->setToolTip(QApplication::translate("MainWindow", "\346\215\225\350\216\267\345\234\206\345\275\242\345\214\272\345\237\237\346\210\252\345\233\276", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionfixedRectCapture->setText(QApplication::translate("MainWindow", "fixedRectCapture", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionfixedRectCapture->setToolTip(QApplication::translate("MainWindow", "\346\215\225\350\216\267\345\233\272\345\256\232\347\237\251\345\275\242\345\214\272\345\237\237\346\210\252\345\233\276", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
