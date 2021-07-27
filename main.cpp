#include "MainInterface.h"
#include <QtWidgets/QApplication>
#include "MainMenu.h"
#include "Helper.h"
#include <qfile.h>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
    
    QApplication app(argc, argv);
    app.setAttribute(Qt::ApplicationAttribute::AA_ShareOpenGLContexts, true);
    QCoreApplication::setOrganizationName("Goose");
    QCoreApplication::setOrganizationDomain("Elizgerd@yandex.ru");
    QCoreApplication::setApplicationName("Poker");

    if (QFile(QDir::currentPath() + "/imgs/icon.ico").exists())
        app.setWindowIcon(QIcon("./imgs/icon.ico"));
    /*if (!QFile("src/settings.ini").exists())
    {
        QSettings settings(Helper::getSettings());
        settings.beginGroup("game");
        settings.setValue("city", 1);
        settings.setValue("deposit", 1000);
    }*/
    
    /*QSettings settings("Goose", "Poker");
    settings.beginGroup("game");
    settings.setValue("city", 1);
    settings.setValue("deposit", 100000);*/

    MainMenu mainMenu;
    //mainMenu.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    mainMenu.setWindowFlags(Qt::FramelessWindowHint);
    //mainMenu.setAttribute(Qt::WA_NoSystemBackground, true);
    //mainMenu.setAttribute(Qt::WA_TranslucentBackground, true);

    if (mainMenu.exec())
    {
        MainInterface mainInterface;
        mainInterface.show();
        int end = app.exec();
        return end;
    }
    else
    {
        app.quit();
        return 0;
    }
}
