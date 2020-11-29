#include <iostream>
#include <ACO/test_input.hpp>
#include <ACO/aco_as.hpp>
#include <UI/MainWindow.hpp>
#include <sstream>
#include <QApplication>

int main(int argc, char ** argv)
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    return 0;
}