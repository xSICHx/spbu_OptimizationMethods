#ifndef DIMENSIONSELECTOR_H
#define DIMENSIONSELECTOR_H

#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class DimensionSelector : public QWidget {
    Q_OBJECT

public:
    DimensionSelector(QWidget *parent = nullptr);
    QVector<QPair<int, int>> getBounds() const; // Новый метод

private slots:
    void onDimensionChanged(int dimension);

signals:
    void dimensionChanged(int dimension);

private:
    QComboBox *dimensionComboBox;
    QFormLayout *boundsLayout;
    QVBoxLayout *mainLayout;
    QVector<QPair<QSpinBox*, QSpinBox*>> boundsFields; // Список полей для ввода границ

    void clearBounds();
    void createBoundsFields(int dimension);
};

#endif // DIMENSIONSELECTOR_H
