#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(640, 480);
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter{this};
    painter.setPen( QPen(Qt::black, 2) );

    double X1 = -50; //Точка начала дуги (текущая позиция)
    double Y1 = 0;
    double X2 = 0;   //Точка конца дуги (заданная позиция)
    double Y2 = -50;
    double I2 = 0;   //Центр дуги
    double J2 = 0;

    double angle1 = qAtan2 ( X1 - I2, Y1 - J2 ); //угол начала
    double angle2 = qAtan2 ( X2 - I2, Y2 - J2 ); //угол окончания
    double radius = qSqrt( qPow(X1 - I2,2) + qPow(Y1 - J2,2) ); //радиус дуги

    double delta_angle =  (M_PI*4)/radius; //угол, на который надо шагать при апроксимации отрезками (чем меньше радиус, тем меньше точек в окружности)

    //Работает только от меньшего угла к большему
    for( double CurAngle = angle1; CurAngle < angle2; CurAngle += delta_angle){
        qreal x = I2 + radius * qSin(CurAngle);
        qreal y = J2 + radius * qCos(CurAngle);

        painter.drawPoint(QPointF(300+x, 300-y)); //На экране Y координата перевернута
    }
}
