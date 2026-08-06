#include "square.hpp"
#include "common.hpp"

namespace pet{

	Square Square::fromCorner(const Point2 &top_left, unsigned int size, Point2 scale, double thick){
		Point2 bottom_right = Point2(top_left.x() + size, top_left.y() + size);

		return Square(top_left, bottom_right, scale, thick);
	}

	Square Square::fromCenter(const Point2 &center, unsigned int size, Point2 scale, double thick){
		Point2 top_left = Point2(center.x() - size/2, center.y() - size/2);

		return fromCorner(top_left, size, scale, thick);
	}

}
