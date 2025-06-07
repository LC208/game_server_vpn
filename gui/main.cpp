#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QtPlugin>
#include <QString>


Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString text = QString("Hello, static Qt!");
    QLabel label(text);
    label.show();
    return app.exec();
}