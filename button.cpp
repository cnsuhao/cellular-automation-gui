#include <QPainter>
#include <QBrush>
#include <QPen>
#include "button.h"

Button::Button(QWidget *parent) :
    QPushButton(parent), mycolor(0), inputFlag(false)
{
    connect(this, SIGNAL(clicked()), SLOT(onClick()));
}

void Button::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter;
    painter.begin(this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if (mycolor == 1)    // white
        brush.setColor(Qt::white);
    else    // black
        brush.setColor(Qt::black);

    painter.setBrush(brush);
    painter.drawRect(this->rect());
    painter.end();
}

void Button::setColor(int c)
{
    mycolor = c;
}

int Button::getColor() const
{
    return mycolor;
}

bool Button::isInputColor()
{
    bool tmp = inputFlag;
    inputFlag = false;     // reset
    return tmp;
}

void Button::onClick()
{
    if (mycolor == 1)
        mycolor = 0;
    else
        mycolor = 1;

    inputFlag = true;
}
