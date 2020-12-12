#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public: 
	vec3() : e{ 0,0,0 } {};
	vec3(double e0, double e1, double e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	  double x() const { return e[0]; }
	  double y() const { return e[1]; }
	  double z() const { return e[2]; }

	  const vec3& operator+() const { return *this; }
	  vec3 operator-()const { return vec3(-e[0], -e[1], -e[2]); }
	  double operator[](int i) const { return e[i]; }
	  double& operator[](int i) { return e[i]; }

	  vec3& operator+=(const vec3& v2);
	  vec3& operator-=(const vec3& v2);
	  vec3& operator*=(const vec3& v2);
	  vec3& operator/=(const vec3& v2);
	  vec3& operator*=(const double t);
	  vec3& operator/=(const double t);

	  double lenght() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);	}
	  double lenght_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];	}
	  void make_unit_vector();

	double e[3];
};

  std::istream& operator>>(std::istream& is, vec3& t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

  std::ostream& operator<<(std::ostream& os, vec3& t) {
	os << t.e[0]<< " " << t.e[1] << " " << t.e[2];
	return os;
}

  void vec3::make_unit_vector() {
	  if (e[0] * e[0] + e[1] * e[1] + e[2] * e[2] != 0) {
		  double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
		  e[0] *= k;
		  e[1] *= k;
		  e[2] *= k;
	}
}

  vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

  vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

  vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

  vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

  vec3 operator*(double t, const vec3& v2) {
	return vec3(t * v2.e[0], t * v2.e[1], t * v2.e[2]);
}


  vec3 operator*(const vec3& v2, double t) {
	return vec3(t * v2.e[0], t * v2.e[1], t * v2.e[2]);
}

  vec3 operator/(vec3 v2, double t) {
	return vec3(v2.e[0]/t, v2.e[1]/t, v2.e[2]/t);
}

  double dot(const vec3& v1, const vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

  vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

  vec3& vec3::operator+=(const vec3& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

  vec3& vec3::operator*=(const vec3& v) {
	e[0] *= v.e[0]; 
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

  vec3& vec3::operator/=(const vec3& v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

  vec3& vec3::operator-=(const vec3& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

  vec3& vec3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

  vec3& vec3::operator/=(const double t) {
	double k = 1.0 / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}


  bool operator==(const vec3& v1, const vec3& v2){
	return (v1.e[0] == v2.e[0] && v1.e[1] == v2.e[1] && v1.e[2] == v2.e[2]);
}

  vec3 unit_vector(vec3 v) {
	if (v.lenght() == 0.0) return vec3(1.0, 0.0, 0.0); 
	return v / v.lenght();
}

  using point3 = vec3; 
  using color = vec3;

#endif // !VEC3_H


