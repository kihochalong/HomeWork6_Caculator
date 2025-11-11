#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void onButtonClicked(const QString &text);

private:
    QLineEdit *display;
    double calculate(const QString &expr);
};

#endif
