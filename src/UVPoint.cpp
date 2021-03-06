#include "UVPoint.h"

UVPoint::UVPoint() {
  this->u = 0.0;
  this->v = 0.0;
  this->w = 0.0;
}

/// Scales the UV point by a wavenumber.
void UVPoint::Scale(double wavenumber) {
  this->u *= wavenumber;
  this->v *= wavenumber;
  this->w *= wavenumber;
}

string UVPoint::HashString(void) {
  ostringstream sstream;
  sstream << this->u << '-' << this->v << '-' << this->w;
  string str = sstream.str();
  return str;
}

UVPoint UVPoint::operator+(UVPoint other) {
  UVPoint temp;
  temp.u = this->u + other.u;
  temp.v = this->v + other.v;
  temp.w = this->w + other.w;

  return temp;
}
