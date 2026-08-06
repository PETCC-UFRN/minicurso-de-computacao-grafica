#include "transform.hpp"
#include "common.hpp"


namespace pet {
    Transform::Transform() : mat() {}
    Transform::Transform(const Mat3& mat) : mat(mat) {}
    
    void Transform::rotate(double degrees, Point2 axis)
    {
		//TODO: missing
    }

    void Transform::translate(Point2 delta)
    {
		//TODO: missing
    }

    void Transform::scale(double deltax, double deltay, Point2 axis)
    {
		//TODO: missing
    }
    
    void Transform::shear(double s, Point2 axis)
    {
		//TODO: missing
    }

    void Transform::reflectX()
    {
		//TODO: missing
    }
    void Transform::reflectY()
    {
		//TODO: missing
    }

    Point2 Transform::operator*(const Point2& p) const
    {
		//TODO: missing
    }

    Transform Transform::operator*(const Transform& t) const
    {
		//TODO: missing
    }

}
