#include "mainwindow.h"
#include <QApplication>
#include "nodeeditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NodeEditor editor;
    editor.show();

    return a.exec();
}
