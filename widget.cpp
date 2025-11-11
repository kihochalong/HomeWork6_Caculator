#include "widget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Caculator");
    resize(300, 400);

    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMinimumHeight(70);

    QStringList buttons = {
        "7","8","9","/",
        "4","5","6","*",
        "1","2","3","-",
        "0","AC","=","+"
    };

    QGridLayout *grid = new QGridLayout;
    int row = 0, col = 0;
    for(const QString &text: buttons){
        QPushButton *btn = new QPushButton(text,this);
        btn->setMinimumSize(60,40);
        grid->addWidget(btn,row,col);
        connect(btn,&QPushButton::clicked,this,[=](){ onButtonClicked(text); });
        col++;
        if(col==4){ col=0; row++; }
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(display);
    mainLayout->addLayout(grid);
    setLayout(mainLayout);
}

Widget::~Widget(){}

void Widget::onButtonClicked(const QString &text)
{
    if(text=="AC"){
        display->clear();
    }
    else if(text=="="){
        QString expr = display->text();
        double result = calculate(expr);
        display->setText(QString::number(result,'g',12));
    }
    else{
        display->setText(display->text() + text);
    }
}


double Widget::calculate(const QString &expr)
{
    QString s = expr;
    s.replace(" ","");

    if(s.isEmpty()) return 0;

    for(int i=0;i<s.size();i++){
        if(s[i]=='+' || s[i]=='-'){
            double left = calculate(s.left(i));
            double right = calculate(s.mid(i+1));
            return s[i]=='+' ? left+right : left-right;
        }
    }

    for(int i=0;i<s.size();i++){
        if(s[i]=='*' || s[i]=='/'){
            double left = calculate(s.left(i));
            double right = calculate(s.mid(i+1));
            return s[i]=='*' ? left*right : left/right;
        }
    }

    return s.toDouble();
}
