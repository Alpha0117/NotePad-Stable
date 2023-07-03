#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    cout << "**********************" << endl;
    cout << "INIT APP" << endl;
    cout << "**********************" << endl;

    QApplication a(argc, argv);
    a.setStyle("fusion");
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ProyectoFinal_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}

