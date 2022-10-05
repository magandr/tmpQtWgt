#include "mainwindow.h"
#include <QMatrix4x4>

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
    QPointF  point;
    painter.setPen( QPen(Qt::black, 2) );

    //входные параметры
    float X1 = -5; //начальная точка
    float Y1 = 0;
    float X2 = 5;  //конечная точка
    float Y2 = 0;
    float XI = 0;  //центр поворота
    float YJ = 0;
    bool clockwise = true;
    float angle_step = 5;

    QVector3D P1(X1-XI,Y1-YJ,0); //создаем 2 вектора
    QVector3D P2(X2-XI,Y2-YJ,0);
    QVector4D PR = P1.toVector4D(); //точка, которую будем вращать (начало совпадает с первой точкой) 4D для одной размерности с матрицей преобразований
    float angle = 0;
    float current_angle = 0;
    //добавить проверку на нулевые вектора

    if(P1 == P2){ //если начальная и конечная точка одна и таже, то это окружность
        (clockwise? angle = 360 : angle = -360);
    }else{
        P1.normalize(); //нормализуем их (приводим к единичной длинне, нам от векторов нужен только угол)
        P2.normalize();

        angle = qRadiansToDegrees( qAcos(QVector3D::dotProduct(P2,P1)) ); //из скалярного произведения векторов вытаскиваем угол

        if((QVector3D::crossProduct(P2,P1)).z() < 0){     //если при этом поменялся знак детерминанта, то угол был больше 180, делаем поправку
            angle = 360 - angle;
        }

        if(!clockwise){ //рассчитанный угол был для движения по часовой стрелке, если это не так, то меняем направление (угол будет отрицательным)
            angle = angle - 360;
        }
    }

    while( ( (clockwise) && (current_angle <= angle) ) ||
           ( (!clockwise) && (current_angle >= angle) ) ){
        QMatrix4x4 m;
        m.rotate(current_angle, 0.0, 0.0, 1.0); //матрицу разварота формируем для каждого угла, чтобы не потерять точность при накомплении точек
        QVector4D P = PR * m; //получаем новую точку из первой с учетом угла

        (clockwise? current_angle += angle_step : current_angle -= angle_step);

        point = P.toPointF();
        point.setX(point.x()*10 + 300);
        point.setY(300-point.y()*10);
        painter.drawPoint(point); //На экране Y координата перевернута
    }

}
