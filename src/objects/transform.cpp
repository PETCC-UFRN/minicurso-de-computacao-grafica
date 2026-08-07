#include "transform.hpp"
#include "common.hpp"


namespace pet {
    Transform::Transform() : mat() {}
    Transform::Transform(const Mat3& mat) : mat(mat) {}
    
    void Transform::rotate(double degrees, Point2 axis)
    {
        double radians = degrees * (M_PI/180.0);
        Mat3 rotMat = {std::cos(radians),-std::sin(radians), 0.0,
                       std::sin(radians), std::cos(radians), 0.0,
                      0.0, 0.0, 1.0};
    
        Mat3 toOrigin = Mat3();
        toOrigin(0, 2) = -axis.x();
        toOrigin(1, 2) = -axis.y();
        Mat3 fromOrigin = Mat3();
        fromOrigin(0, 2) = axis.x();
        fromOrigin(1, 2) = axis.y();
        
        Mat3 pivot = fromOrigin * rotMat * toOrigin;

        this->mat = mat * pivot;
    }

    void Transform::translate(Point2 delta)
    {
        Mat3 transMat = Mat3();
        transMat(0, 2) = delta.x();
        transMat(1, 2) = delta.y();

        this->mat = mat * transMat;
    }

    void Transform::scale(double deltax, double deltay, Point2 axis)
    {
        Mat3 scaleMat = Mat3();

        scaleMat(0, 0) = deltax;
        scaleMat(1, 1) = deltay;

        Mat3 toOrigin = Mat3();
        toOrigin(0, 2) = -axis.x();
        toOrigin(1, 2) = -axis.y();

        Mat3 fromOrigin = Mat3();
        fromOrigin(0, 2) = axis.x();
        fromOrigin(1, 2) = axis.y();

        Mat3 pivot = fromOrigin * scaleMat * toOrigin;
        
        this->mat = mat * pivot;
    }
    
    void Transform::shear(double s, Point2 axis)
    {
        Mat3 shearMat = Mat3();
        shearMat(0, 1) = s;

        Mat3 toOrigin = Mat3();
        toOrigin(0, 2) = -axis.x();
        toOrigin(1, 2) = -axis.y();

        Mat3 fromOrigin = Mat3();
        fromOrigin(0, 2) = axis.x();
        fromOrigin(1, 2) = axis.y();

        Mat3 pivot = fromOrigin * shearMat * toOrigin;
        this->mat = mat * pivot;
    }

    void Transform::reflectX()
    {
        Mat3 reflectMat = Mat3();

        reflectMat(1, 1) = -1;

        this->mat = mat * reflectMat;
    }
    void Transform::reflectY()
    {
        Mat3 reflectMat = Mat3();

        reflectMat(0, 0) = -1;

        this->mat = mat * reflectMat;
    }

    Point2 Transform::operator*(const Point2& p) const
    {
        Point3 point = Point3(static_cast<double>(p[0]), static_cast<double>(p[1]), 1.0);
        point = this->mat * point;

        Point2 finalP = {static_cast<int>(point[0]), static_cast<int>(point[1])};
        return finalP;
    }

    Transform Transform::operator*(const Transform& t) const
    {
        return Transform(this->mat * t.mat);
    }

}