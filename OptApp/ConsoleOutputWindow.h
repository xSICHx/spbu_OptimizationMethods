#ifndef CONSOLEOUTPUTWINDOW_H
#define CONSOLEOUTPUTWINDOW_H

#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QScrollBar> // Добавляем заголовочный файл для QScrollBar


class ConsoleOutputWindow : public QWidget {
    Q_OBJECT

public:
    ConsoleOutputWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Создаем текстовое поле с прокруткой
        textEdit = new QPlainTextEdit(this);
        textEdit->setReadOnly(true); // Запрещаем редактирование
        textEdit->setWordWrapMode(QTextOption::NoWrap); // Отключаем перенос слов

        // Создаем кнопку для добавления текста (для демонстрации)
        QPushButton *addTextButton = new QPushButton("Добавить текст", this);
        connect(addTextButton, &QPushButton::clicked, this, &ConsoleOutputWindow::appendText);

        // Создаем layout и добавляем виджеты
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(textEdit);
        layout->addWidget(addTextButton);

        setLayout(layout);
    }

public slots:
    // Метод для добавления текста в консоль
    void appendText(const QString &text) {
        textEdit->appendPlainText(text); // Добавляем текст
        textEdit->verticalScrollBar()->setValue(textEdit->verticalScrollBar()->maximum()); // Прокручиваем вниз
    }

private:
    QPlainTextEdit *textEdit;
};

#endif // CONSOLEOUTPUTWINDOW_H
