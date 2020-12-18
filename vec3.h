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

	  vec3 operator-()const { return vec3(-e[0], -e[1], -e[2]); }
	  double operator[](int i) const { return e[i]; }
	  double& operator[](int i) { return e[i]; }

	  //Sobrecarga de operadores
	  vec3& operator+=(const vec3& v2) {
		  e[0] += v2.e[0];
		  e[1] += v2.e[1];
		  e[2] += v2.e[2];
		  return *this;
	  }

	  vec3& operator*=(const double t) {
		  e[0] *= t; 
		  e[1] *= t;
		  e[2] *= t; 
		  return *this;
	  }

	  vec3& operator/=(const double t) {
		  return *this *= 1 / t;
	  }


	  double lenght() const {
		return sqrt(lenght_squared());	}
	  double lenght_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];	}
	  void make_unit_vector();

	  inline static vec3 random() {
		  return vec3(random_double(), random_double(), random_double());
	  }

	  inline static vec3 random(double min, double max) {
		  return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	  }

	  bool near_zero() {
		  //Return true if the vector is close to zero in all dimensions.
		  const auto s = 1e-8; 
		  return(fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	  }

	double e[3];
};


// vec3 Utility functions 

  inline std::istream& operator>>(std::istream& is, vec3& t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

 inline std::ostream& operator<<(std::ostream& os, vec3& t) {
	os << t.e[0]<< " " << t.e[1] << " " << t.e[2];
	return os;
}

  //revisar

  void vec3::make_unit_vector() {
	  if (e[0] + e[1] + e[2] != 0) {
		  double k = 1.0 / lenght();
		  e[0] *= k;
		  e[1] *= k;
		  e[2] *= k;
	}
}

  inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

  inline vec3 operator+(const vec3& v1, const double r) {
	  return vec3(v1.e[0] + r, v1.e[1] + r, v1.e[2] + r);
  }

  inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

  inline vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

  inline vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

  inline vec3 operator*(double t, const vec3& v2) {
	return vec3(t * v2.e[0], t * v2.e[1], t * v2.e[2]);
}


  inline vec3 operator*(const vec3& v2, double t) {
	  return t * v2;
}

  inline vec3 operator/(vec3 v2, double t) {
	  return (1 / t) * v2;
}

  inline double dot(const vec3& v1, const vec3& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

  vec3 cross(const vec3& v1, const vec3& v2) {
	  return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
				  -(v1.e[0] * v2.e[2]) + v1.e[2] * v2.e[0],
				  (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}


  bool operator==(const vec3& v1, const vec3& v2){
	return (v1.e[0] == v2.e[0] && v1.e[1] == v2.e[1] && v1.e[2] == v2.e[2]);
}

  vec3 unit_vector(vec3 v) {
	if (v.lenght() == 0.0) return v; 
	return v / v.lenght();
}


  vec3 random_in_unit_sphere() {
	  while (true) {
		  auto p = vec3::random(-1, 1);
		  if (p.lenght_squared() >= 1) continue; 
		  return p;
	  }
  }

  vec3 random_unit_vector() {
	  return unit_vector(random_in_unit_sphere());
  }

  vec3 random_in_hemisphere(const vec3& normal) {
	  vec3 in_unit_sphere = random_in_unit_sphere(); 
	  if (dot(in_unit_sphere, normal) > 0.0)
		  return in_unit_sphere;
	  else
		  return -in_unit_sphere;
  }

  vec3 reflect(const vec3& v, const vec3& n) {
	  return v - (2 * dot(v, n) * n);
  }

  vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	  double cos_theta = fmin(dot(-uv, n), 1.0); 
	  vec3 r_out_perp = etai_over_etat * (uv + (cos_theta * n)); 
	  vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lenght_squared())) * n;
	  return r_out_perp + r_out_parallel;
  }

  vec3 random_in_unit_disk() {
	  while (true) {
		  auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0); 
		  if (p.lenght_squared() >= 1) continue;
		  return p;
	  }
  }

  using point3 = vec3; 
  using color = vec3;

#endif // !VEC3_H


