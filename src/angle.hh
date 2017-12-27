// Copyright 2017 Neverware

#ifndef SRC_ANGLE_HH_
#define SRC_ANGLE_HH_

namespace mgrid {

class Angle {
 public:
  static Angle from_degrees(const float value);

  static Angle from_radians(const float value);

  float in_degrees() const;

  float in_radians() const;

  Angle operator+(const Angle& other) const;
  Angle operator-(const Angle& other) const;

 private:
  Angle(const float radians);

  float radians_ = 0;
};

}

#endif  // SRC_ANGLE_HH_
