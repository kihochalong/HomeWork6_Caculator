#include "widget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QtMath>

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
        "AC", "DEL", "(", ")",
        "7","8","9","/",
        "4","5","6","*",
        "1","2","3","-",
        "0",".","=","+"
    };

    QGridLayout *grid = new QGridLayout;
    int row = 0, col = 0;
    for(const QString &text: buttons){
        QPushButton *btn = new QPushButton(text, this);
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
    }else if(text=="DEL"){
        QString t = display->text();
        if(!t.isEmpty()) t.chop(1);
        display->setText(t);
    }else if(text=="="){
        QString expr = display->text();
        double result = calculate(expr);
        display->setText(QString::number(result,'g',12));
    }else{
        display->setText(display->text() + text);
    }
}

double Widget::calculate(const QString &expr)
{
    QString s = expr;
    s.replace(" ","");

    if(s.isEmpty()) return 0;

    int pos = s.indexOf('(');
    while(pos!=-1){
        int count=1;
        int i=pos+1;
        for(;i<s.size();i++){
            if(s[i]=='(') count++;
            else if(s[i]==')') count--;
            if(count==0) break;
        }
        if(count!=0) return 0;
        double val = calculate(s.mid(pos+1,i-pos-1));
        s.replace(pos,i-pos+1,QString::number(val,'g',12));
        pos = s.indexOf('(');
    }


    QStringList ops1 = {"*","/"};
    QStringList ops2 = {"+","-"};

    for(const QString &opList: {ops1.join(","), ops2.join(",")}){
        int idx = -1;
        for(int i=0;i<s.size();i++){
            if(opList.contains(s[i]) && i!=0){
                idx = i;
                break;
            }
        }
        if(idx!=-1){
            double left = calculate(s.left(idx));
            double right = calculate(s.mid(idx+1));
            QChar op = s[idx];
            if(op=='*') return left*right;
            if(op=='/') return right!=0 ? left/right : 0;
            if(op=='+') return left+right;
            if(op=='-') return left-right;
        }
    }


    return s.toDouble();
}
