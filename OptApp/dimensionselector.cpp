#include "dimensionselector.h"

DimensionSelector::DimensionSelector(QWidget *parent)
    : QWidget(parent) {
    QLabel *dimensionLabel = new QLabel("Выберите размерность:");
    dimensionComboBox = new QComboBox();
    for (int i = 1; i <= 4; ++i) {
        dimensionComboBox->addItem(QString::number(i), i);
    }

    connect(dimensionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DimensionSelector::onDimensionChanged);

    boundsLayout = new QFormLayout();
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(dimensionLabel);
    mainLayout->addWidget(dimensionComboBox);
    mainLayout->addLayout(boundsLayout);

    onDimensionChanged(0);  // Инициализация полей для начальной размерности
}

void DimensionSelector::onDimensionChanged(int index) {
    int dimension = dimensionComboBox->currentData().toInt();
    clearBounds();
    createBoundsFields(dimension);
    emit dimensionChanged(dimension); // Emit the signal
}

void DimensionSelector::clearBounds() {
    while (boundsLayout->count() > 0) {
        QLayoutItem *item = boundsLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    boundsFields.clear(); // Очищаем список полей
}

void DimensionSelector::createBoundsFields(int dimension) {
    for (int i = 0; i < dimension; ++i) {
        QSpinBox *minSpinBox = new QSpinBox();
        minSpinBox->setRange(-1000, 1000);
        minSpinBox->setValue(-10);

        QSpinBox *maxSpinBox = new QSpinBox();
        maxSpinBox->setRange(-1000, 1000);
        maxSpinBox->setValue(10);

        boundsLayout->addRow(QString("Мин (Измерение %1):").arg(i + 1), minSpinBox);
        boundsLayout->addRow(QString("Макс (Измерение %1):").arg(i + 1), maxSpinBox);

        boundsFields.append(qMakePair(minSpinBox, maxSpinBox));
    }
}

QVector<QPair<int, int>> DimensionSelector::getBounds() const {
    QVector<QPair<int, int>> bounds;
    for (const auto &pair : boundsFields) {
        int minValue = pair.first->value();
        int maxValue = pair.second->value();
        bounds.append(qMakePair(minValue, maxValue));
    }
    return bounds;
}
