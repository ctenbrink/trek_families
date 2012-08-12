#include <QApplication>
#include "mainwindow.h"
#include "document.h"
int main(int argv, char *args[])
{
    
    QApplication app(argv, args);
    MainWindow main_window;
    main_window.show();
    return app.exec();
}
