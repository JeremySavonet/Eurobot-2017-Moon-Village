// Copyright (c) 2017 All Rights Reserved WestBot

#include <iostream>
#include <QCoreApplication>
#include <QDebug>

// 3rdParty library

#include  <3rdparty/Eigen/Dense>

using Eigen::MatrixXd;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);

    std::cout << m << std::endl;

    return a.exec();
}
