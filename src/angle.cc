#include "angle.hh"

#include <cmath>

namespace mgrid {

Angle Angle::from_degrees(const float degrees) {
  return Angle(degrees * (M_PI / 180.0f));
}

Angle Angle::from_radians(const float radians) {
  return Angle(radians);
}

float Angle::in_degrees() const {
  return radians_ * (180.0f / M_PI);
}

float Angle::in_radians() const {
  return radians_;
}

Angle Angle::operator+(const Angle& other) const {
  return Angle(radians_ + other.radians_);
}

Angle Angle::operator-(const Angle& other) const {
  return Angle(radians_ - other.radians_);
}

Angle::Angle(const float radians) : radians_(radians) {}

}
