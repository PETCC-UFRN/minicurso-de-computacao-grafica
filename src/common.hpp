#ifndef COMMON_HPP
#define COMMON_HPP

#include <ssmath/ssmath.hpp>
#include <array>
#include <cstdio>
#include <optional>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

using byte = uint16_t;

using std::cout;
using std::endl;

using std::array;
using std::vector;
using std::string;
using Point3 = vec3<double>;
using Mat2 = mat2<double>;
using Vec3 = vec3<double>;
using Mat3 = mat3<double>;
using Point2 = vec2<int>;
using Pixel = Point2;

// Corner indexes.
enum Corners_e {
  BOTTOM_LEFT = 0,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_RIGHT,
};

namespace pet {

struct RunningOptions {
	std::optional<array<Pixel,2>> crop_region;
	bool quick{false};
	bool verbose{false};
	string outfile = "";
	string scene;
};

struct RGBColor {

  /*
  atributos...
  constructors...
  destructors...
  */


  RGBColor operator+(const double& c) const {return RGBColor(/*TODO*/);}
  RGBColor operator*(const double& t) const {return RGBColor(/*TODO*/);}
  RGBColor operator/(const double& t) const {return RGBColor(/*TODO*/);}

  RGBColor operator+(const RGBColor& c) const {return RGBColor(/*TODO*/);}
  RGBColor operator*(const RGBColor& c) const {return RGBColor(/*TODO*/);}
  RGBColor operator/(const RGBColor& c) const {return RGBColor(/*TODO*/);}

  bool operator==(const RGBColor& c) const {return /*TODO*/};

  double&    operator[](const size_t index){
    if(index == 0)return /*TODO*/;
    if(index == 1)return /*TODO*/;
    return /*TODO*/;
  };

};


inline std::istream& operator>>(std::istream& is, pet::RGBColor& color) {
    /*TODO*/
    /*Essa função serve para executar coisas como
      
      cin >> color;
    
    sem precisar preencher cada campo individualmente*/
}
inline std::ostream& operator<<(std::ostream& os, const pet::RGBColor& color) {
    /*TODO*/
    /*Essa função serve para executar coisas como
      
      cout << color;
    
    sem precisar printar cada canal de cor individualmente*/
}

} // namespace rt

#endif //COMMON_HPP
