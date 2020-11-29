QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/MapWidget.cpp

HEADERS += \
    include/UI/MainWindow.hpp\
    include/UI/MapWidget.hpp\
    include/ACO/aco_ant.hpp\
    include/ACO/aco_as.hpp\
    include/ACO/aco_nodes.hpp\
    include/ACO/cost_matrix_interface.hpp\
    include/ACO/sample_tree.hpp\
    include/ACO/test_input.hpp\
    include/ACO/tree_nodes.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
