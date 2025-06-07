#include <iostream>
#include "mylib.hpp"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QtPlugin>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QLabel label("Hello, static Qt!");
    label.show();
    return app.exec();
}