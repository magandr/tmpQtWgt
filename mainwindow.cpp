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
    painter.setPen( QPen(Qt::black, 2) );

    //входные параметры
    float X1 = 200; //начальная точка (текущая позиция)
    float Y1 = 250;
    float X2 = 250;  //конечная точка (X,Y в G2 G3)
    float Y2 = 200;
    float XI = 250;  //центр поворота (I,J в G2 G3)
    float YJ = 250;
    bool clockwise = true; //G2 - по часовой, G3 - против часовой
    float angle_step = 5;

    QVector3D P1(X1-XI,Y1-YJ,0); //создаем 2 вектора
    QVector3D P2(X2-XI,Y2-YJ,0);

    float angle = 0;    //угол между векторами

    if(P1.length() && P2.length()){ //если вектора имеют длинну, смотрим угол
        if(P1 == P2){ //если начальная и конечная точка одна и таже, то это окружность
            (clockwise? angle = -360 : angle = 360);
        }else{
            P1.normalize(); //нормализуем их (приводим к единичной длинне, нам от векторов нужен только угол)
            P2.normalize();

            angle = qRadiansToDegrees( qAcos(QVector3D::dotProduct(P1,P2)) ); //из скалярного произведения векторов вытаскиваем угол

            if((QVector3D::crossProduct(P1,P2)).z() < 0){     //если при этом поменялся знак детерминанта, то угол был больше 180, делаем поправку
                angle = 360 - angle;
            }

            if(clockwise){ //рассчитанный угол был для движения против часовой стрелки, если это не так, то меняем направление (угол будет отрицательным)
                angle = angle - 360;
            }
        }
    }else{
        angle = 0; //если вектора нулевые, то просто точка будет
    }


    QVector4D PR(X1-XI,Y1-YJ,0,1); //Координаты первой точки от центра вращения

    float cur_angle = 0;

    while( ( (!clockwise) && (cur_angle <= angle) ) ||
           ( (clockwise) && (cur_angle >= angle) ) ){
        QMatrix4x4 matrix; //матрицу разварота формируем для каждого угла, чтобы не потерять точность при накомплении точек

        matrix.translate(XI,YJ,0); //смещаем центр вращения в координаты центра дуги
        matrix.rotate(clockwise? cur_angle -= angle_step : cur_angle += angle_step, 0.0, 0.0, 1.0); //расчет текущего угла поворота

        QVector4D P = matrix * PR; //получаем новую точку из первой с учетом угла

        painter.drawPoint(P.toPointF()); //На экране Y координата перевернута
    }

}
