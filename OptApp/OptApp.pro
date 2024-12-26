QT       += core gui
QT += charts
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dimensionselector.cpp \
    main.cpp \
    mainwindow.cpp \
    ../OptimizationMethods/Area.cpp \
    ../OptimizationMethods/FucntionTest.cpp \
    ../OptimizationMethods/Function.cpp \
    ../OptimizationMethods/OptMethodNewton.cpp \
    ../OptimizationMethods/OptMethodRandomSearch.cpp \
    #../OptimizationMethods/OptimizationMethods.cpp \
    ../OptimizationMethods/StopCriterion.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS += \
    OptimizationWindow.h \
    dimensionselector.h \
    mainwindow.h \
    ../OptimizationMethods/Area.h \
    ../OptimizationMethods/FucntionTest.h \
    ../OptimizationMethods/Function.h \
    ../OptimizationMethods/FunctionAbs.h \
    ../OptimizationMethods/FunctionDefault.h \
    ../OptimizationMethods/FunctionEasom.h \
    ../OptimizationMethods/FunctionHarmonic.h \
    ../OptimizationMethods/FunctionRosenbrock.h \
    ../OptimizationMethods/OptMethod.h \
    ../OptimizationMethods/OptMethodNewton.h \
    ../OptimizationMethods/OptMethodRandomSearch.h \
    ../OptimizationMethods/StopCriterion.h \
    ../OptimizationMethods/StopCriterionArea.h \
    ../OptimizationMethods/StopCriterionEps.h \
    ../OptimizationMethods/StopCriterionFEps.h \
    ../OptimizationMethods/StopCriterionFEpsUnnormalized.h \
    ../OptimizationMethods/StopCriterionFVal.cpp \
    ../OptimizationMethods/StopCriterionN.h \
    ../OptimizationMethods/TransferData.h \
    ../OptimizationMethods/config.h \
    qcustomplot/qcustomplot.h


FORMS += \
    mainwindow.ui

INCLUDEPATH += ../packages/Eigen.3.3.3/build/native/include


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
