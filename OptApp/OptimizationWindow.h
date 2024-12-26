#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include "dimensionselector.h"
#include "../OptimizationMethods/allHeaders.h"
#include "qcustomplot/qcustomplot.h"


class OptimizationWindow : public QWidget {
    Q_OBJECT  // Это нужно для использования сигналов и слотов

public:
    OptimizationWindow(QWidget *parent = nullptr)
        : QWidget(parent), optimizeButton(new QPushButton("Запустить оптимизацию", this)) {

        // Инициализация выборки функций для каждой размерности
        functions.resize(5);
        functions[2].emplace_back(std::make_shared<FunctionDefault>());
        functions[2].emplace_back(std::make_shared<FunctionAbs>());
        functions[2].emplace_back(std::make_shared<FunctionRosenbrock>());
        functions[3].emplace_back(std::make_shared<FunctionHarmonic>());
        functions[4].emplace_back(std::make_shared<FunctionEasom>());

        // Создаем левую панель с прокручиваемой областью
        leftPanel = new QWidget();
        QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

        // Добавляем виджет выбора размерности в левую панель
        dimensionSelector = new DimensionSelector(leftPanel);
        leftLayout->addWidget(dimensionSelector);

        // Добавляем выбор функции
        functionComboBox = new QComboBox(this);
        leftLayout->addWidget(new QLabel("Выберите функцию:"));
        leftLayout->addWidget(functionComboBox);

        // Подключаем сигнал изменения размерности к слоту обновления функций
        connect(dimensionSelector, &DimensionSelector::dimensionChanged, this, &OptimizationWindow::updateFunctionSelection);

        // Подключаем сигнал выбора функции к слоту
        connect(functionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &OptimizationWindow::onFunctionSelected);

        // Поля для ввода максимальных итераций и epsilon
        maxIterationsSpinBox = new QSpinBox(this);
        maxIterationsSpinBox->setRange(10, 100000);
        maxIterationsSpinBox->setValue(1000);
        leftLayout->addWidget(new QLabel("Макс. итераций:"));
        leftLayout->addWidget(maxIterationsSpinBox);

        // Используем QFormLayout для компактного размещения текста и поля ввода
        QFormLayout *formLayout = new QFormLayout();

        // Создаем QDoubleSpinBox для epsilon
        epsilonSpinBox = new QDoubleSpinBox(this);
        epsilonSpinBox->setRange(1e-10, 1e+10);
        epsilonSpinBox->setValue(1e-5);
        epsilonSpinBox->setDecimals(10); // Устанавливаем два знака после запятой

        // Добавляем метку и поле ввода в форму
        formLayout->addRow("Epsilon:", epsilonSpinBox);

        // Добавляем форму в левый layout
        leftLayout->addLayout(formLayout);

        // Добавляем выбор критерия остановки
        stopCriterionComboBox = new QComboBox(this);
        stopCriterionComboBox->addItem("StopCriterion (max iterations)");
        stopCriterionComboBox->addItem("StopCriterionFEps (max iterations & epsilon)");
        stopCriterionComboBox->addItem("StopCriterionFEpsUnnormalized (max iterations & epsilon)");
        leftLayout->addWidget(new QLabel("Выберите критерий остановки:"));
        leftLayout->addWidget(stopCriterionComboBox);

        // Добавляем выбор метода оптимизации
        optMethodComboBox = new QComboBox(this);
        optMethodComboBox->addItem("Newton");
        optMethodComboBox->addItem("Random Search");
        leftLayout->addWidget(new QLabel("Выберите метод оптимизации:"));
        leftLayout->addWidget(optMethodComboBox);

        // Добавляем поля для ввода начальной точки
        initialPointWidget = new QWidget(this);
        initialPointLayout = new QVBoxLayout(initialPointWidget);
        leftLayout->addWidget(new QLabel("Начальная точка:"));
        leftLayout->addWidget(initialPointWidget);

        // Подключаем сигнал изменения размерности к слоту обновления начальной точки
        connect(dimensionSelector, &DimensionSelector::dimensionChanged, this, &OptimizationWindow::setupInitialPointInputs);

        // Создаем правую панель
        rightPanel = new QWidget();
        QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->addWidget(optimizeButton);
        connect(optimizeButton, &QPushButton::clicked, this, &OptimizationWindow::onOptimizeButtonClicked);

        // Create QCustomPlot instance
        customPlot = new QCustomPlot(rightPanel);

        // Modify rightLayout to include the plot
        rightLayout->addWidget(customPlot); // Add the plot widget

        // ...





        rightPanel->setLayout(rightLayout);


        // Создаем вертикальную линию
        QFrame *verticalLine = new QFrame();
        verticalLine->setFrameShape(QFrame::VLine);  // Устанавливаем вертикальную линию
        verticalLine->setFrameShadow(QFrame::Sunken);  // Добавляем эффект "утопленности"
        verticalLine->setLineWidth(2);  // Устанавливаем толщину линии

        // Создаем основной layout
        QSplitter *splitter = new QSplitter(Qt::Horizontal);

        // Добавляем левую и правую панели в splitter
        splitter->addWidget(leftPanel);
        splitter->addWidget(verticalLine);
        splitter->addWidget(rightPanel);

        // Устанавливаем размеры для каждой панели
        splitter->setSizes(QList<int>() << 198 << 2 << 800);  // Левая панель 200px, правая — 800px

        // Устанавливаем основной layout
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(splitter);
        setLayout(mainLayout);

        // Устанавливаем минимальные размеры окна
        setMinimumSize(1000, 600);
    }

    // Добавляем метод для создания объекта Area из DimensionSelector
    Area createAreaFromUI() {
        QVector<QPair<int, int>> bounds = dimensionSelector->getBounds();

        int dim = bounds.size();
        Eigen::MatrixXd areaBounds(dim, 2);

        for (int i = 0; i < dim; ++i) {
            areaBounds(i, 0) = bounds[i].first;  // Минимум
            areaBounds(i, 1) = bounds[i].second; // Максимум
        }

        return Area(areaBounds);
    }

private slots:
    void updateFunctionSelection(int dimension) {
        functionComboBox->clear();
        if (!functions[dimension].empty()) {
            for (const auto& func : functions[dimension]) {
                std::string funcName = func->getName();  // Это std::string
                functionComboBox->addItem(QString::fromStdString(funcName));  // Преобразуем в QString и добавляем в comboBox
            }
            functionComboBox->setEnabled(true);
        } else {
            QMessageBox::information(this, "Информация", "Для выбранной размерности функции не доступны.");
            functionComboBox->setEnabled(false);
        }
    }

    void onFunctionSelected(int index) {
        if (index >= 0) {
            int dimension = dimensionSelector->getBounds().length();
            selectedFunction = functions[dimension][index];
            // Enable optimizeButton or perform other actions
        }
    }

    void setupInitialPointInputs(int dimension) {
        for (auto spinbox : initialPointSpinboxes) {
            initialPointLayout->removeWidget(spinbox);
            spinbox->deleteLater();
        }
        initialPointSpinboxes.clear();

        QVector<QPair<int, int>> bounds = dimensionSelector->getBounds();
        for (int i = 0; i < dimension; ++i) {
            QDoubleSpinBox* spinbox = new QDoubleSpinBox(this);
            spinbox->setMinimum(bounds[i].first);
            spinbox->setMaximum(bounds[i].second);
            spinbox->setValue((bounds[i].first + bounds[i].second) / 2.0);
            initialPointLayout->addWidget(spinbox);
            initialPointSpinboxes.push_back(spinbox);
        }
    }


    void generateContourPlot() {
        QVector<QPair<int, int>> bounds = dimensionSelector->getBounds();
        int dim = bounds.size();

        if (dim != 2) return;

        int xSteps = 1000;
        int ySteps = 1000;

        double xmin = bounds[0].first;
        double xmax = bounds[0].second;
        double ymin = bounds[1].first;
        double ymax = bounds[1].second;

        // Prepare matrix for function values
        QCPColorMapData *valueMatrix = new QCPColorMapData(xSteps, ySteps, QCPRange(xmin, xmax), QCPRange(ymin, ymax));

        for (int i = 0; i < xSteps; ++i) {
            double x = xmin + i * (xmax - xmin) / (xSteps - 1);
            for (int j = 0; j < ySteps; ++j) {
                double y = ymin + j * (ymax - ymin) / (ySteps - 1);
                VectorXd point(2);
                point << x, y;
                double value = (*selectedFunction)(point);
                valueMatrix->setCell(i, j, value);
            }
        }

        // Clear previous plot
        customPlot->clearPlottables();

        // Add color map
        QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
        colorMap->setData(valueMatrix, true); // true to rescale the data range

        // Set color gradient
        colorMap->setGradient(QCPColorGradient::gpJet);

        // Add color scale
        QCPColorScale *colorScale = new QCPColorScale(customPlot);
        customPlot->plotLayout()->addElement(0, 1, colorScale); // Add color scale to the right of the plot
        colorScale->setType(QCPAxis::atRight); // Set color scale orientation
        colorMap->setColorScale(colorScale); // Link color map to color scale

        // Set axis labels
        customPlot->xAxis->setLabel(QString("X (%1 to %2)").arg(xmin).arg(xmax));
        customPlot->yAxis->setLabel(QString("Y (%1 to %2)").arg(ymin).arg(ymax));

        // Rescale axes
        customPlot->rescaleAxes();

        // Replot
        customPlot->replot();
    }

    QVector<QPointF> readOptimizationPoints(const QString& filename) {
        QVector<QPointF> points;
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file:" << filename;
            return points;
        }
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(' ');
            if (parts.size() != 2) {
                qWarning() << "Invalid line in file:" << line;
                continue;
            }
            bool okX, okY;
            double x = parts[0].toDouble(&okX);
            double y = parts[1].toDouble(&okY);
            if (okX && okY) {
                points.append(QPointF(x, y));
            } else {
                qWarning() << "Invalid values in line:" << line;
            }
        }
        file.close();
        return points;
    }


    void onOptimizeButtonClicked() {
        if (!selectedFunction) {
            QMessageBox::warning(this, "Ошибка", "Выберите функцию.");
            return;
        }

        int dimension = dimensionSelector->getBounds().length();
        Area area = createAreaFromUI();
        int maxIter = maxIterationsSpinBox->value();
        double eps = epsilonSpinBox->value();
        int stopCriterionIndex = stopCriterionComboBox->currentIndex();
        int optMethodIndex = optMethodComboBox->currentIndex();

        if (dimension == 2){
             generateContourPlot();
        }
        std::shared_ptr<StopCriterion> crit;
        switch (stopCriterionIndex) {
        case 0:
            crit = std::make_shared<StopCriterion>(maxIter);
            break;
        case 1:
            crit = std::make_shared<StopCriterionFEps>(maxIter, eps);
            break;
        case 2:
            crit = std::make_shared<StopCriterionFEpsUnnormalized>(maxIter, eps);
            break;
        default:
            crit = std::make_shared<StopCriterion>(maxIter);
            break;
        }

        std::shared_ptr<OptMethod> opt;
        switch (optMethodIndex) {
        case 0:
            opt = std::make_shared<OptMethodNewton>();
            break;
        case 1:
            opt = std::make_shared<OptMethodRandomSearch>();
            break;
        default:
            opt = std::make_shared<OptMethodNewton>();
            break;
        }

        VectorXd startPoint(dimension);
        for (int i = 0; i < dimension; ++i) {
            startPoint(i) = initialPointSpinboxes[i]->value();
        }

        opt->optimize(area, *selectedFunction, *crit, startPoint);

        // Добавляем точки оптимизации на график
        QVector<QPointF> points = readOptimizationPoints("currOpt.txt"); // Замените на путь к вашему файлу
        if (!points.isEmpty()) {
            QCPGraph *pointGraph = customPlot->addGraph();
            QVector<double> xData, yData;
            for (const QPointF &point : points) {
                xData.append(point.x());
                yData.append(point.y());
            }
            pointGraph->setData(xData, yData);
            pointGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, 10)); // Красные круги
            pointGraph->setLineStyle(QCPGraph::lsNone); // Не соединять точки линиями
        }

        // Replot
        customPlot->replot();

        QMessageBox::information(this, "Оптимизация завершена", "Оптимизация выполнена.");
    }

private:
    QPushButton *optimizeButton;
    QWidget *leftPanel;
    QWidget *rightPanel;
    DimensionSelector *dimensionSelector;
    QSpinBox* maxIterationsSpinBox;
    QDoubleSpinBox* epsilonSpinBox;
    QComboBox* functionComboBox;
    QComboBox* stopCriterionComboBox;
    QComboBox* optMethodComboBox;
    QWidget* initialPointWidget;
    QVBoxLayout* initialPointLayout;
    std::vector<QDoubleSpinBox*> initialPointSpinboxes;

    QCustomPlot *customPlot;

    // Функции и другие параметры для каждой размерности
    std::vector<std::vector<std::shared_ptr<Function>>> functions;
    std::shared_ptr<Function> selectedFunction;
    std::shared_ptr<Area> area;
    VectorXd startPoint;
};
