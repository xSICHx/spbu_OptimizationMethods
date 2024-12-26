#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPixmap>
#include <QColor>
#include <vector>
#include <cmath>
#include <algorithm>

// Пример функции
double your_function(double x, double y) {
    return std::sin(std::sqrt(x * x + y * y));  // Пример функции
}

// Генерация значений функции на сетке
std::vector<std::vector<double>> generate_values(int width, int height, double x_min, double x_max, double y_min, double y_max) {
    std::vector<std::vector<double>> values(height, std::vector<double>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double x = x_min + j * (x_max - x_min) / (width - 1);
            double y = y_min + i * (y_max - y_min) / (height - 1);
            values[i][j] = your_function(x, y);
        }
    }

    return values;
}

// Нормализация значений функции
void normalize_values(std::vector<std::vector<double>> &values) {
    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::lowest();

    for (const auto &row : values) {
        for (const auto &val : row) {
            if (val < min_val) min_val = val;
            if (val > max_val) max_val = val;
        }
    }

    for (auto &row : values) {
        for (auto &val : row) {
            val = (val - min_val) / (max_val - min_val);  // Нормализуем в диапазон [0, 1]
        }
    }
}

// Интерполяция цвета от синего до красного
QColor interpolate_color(double value) {
    // Значения от 0 (синий) до 1 (красный)
    int red = static_cast<int>(value * 255);
    int blue = static_cast<int>((1.0 - value) * 255);
    return QColor(red, 0, blue);
}

// Генерация QImage для отображения
QImage generate_image(const std::vector<std::vector<double>> &values) {
    int height = values.size();
    int width = values[0].size();
    QImage image(width, height, QImage::Format_RGB32);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double normalized_value = values[i][j];
            QColor color = interpolate_color(normalized_value);  // Плавный переход цвета
            image.setPixel(j, i, color.rgb());
        }
    }

    return image;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Подключаем слот к кнопке
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::showGraph);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Реализация слота для генерации графика
void MainWindow::showGraph() {
    int width = 500;  // Ширина изображения
    int height = 500; // Высота изображения
    double x_min = -10, x_max = 10;
    double y_min = -10, y_max = 10;

    // Генерация значений функции
    auto values = generate_values(width, height, x_min, x_max, y_min, y_max);

    // Нормализация значений
    normalize_values(values);

    // Генерация изображения
    QImage image = generate_image(values);

    // Устанавливаем изображение на QLabel
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}
