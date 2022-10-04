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

    QVector3D P1(-5,0,0);
    QVector3D P2(-2,-5,0);
    QVector3D PC(0,0,0);

    QVector3D V1 = P1-PC;
    QVector3D V2 = P2-PC;

    V1.normalize();
    V2.normalize();

    float common_angle = qRadiansToDegrees( qAcos( QVector3D::dotProduct(V1,V2) ) );
    float step_angle = 5;
    int angle_steps = common_angle/step_angle;

    QMatrix4x4 m;
    m.rotate(step_angle, 0.0, 0.0, 1.0);

    QVector3D P = P1;
    for(int step = 0; step < angle_steps; step ++){
        P -= PC;
        P = (P.toVector4D() * m).toVector3D();
        P += PC;

        point = P.toPointF();
        point.setX(point.x()*10 + 300);
        point.setY(300-point.y()*10);
        painter.drawPoint(point); //На экране Y координата перевернута
    }

}
