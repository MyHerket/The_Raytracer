#pragma once
#ifndef PDF_H

#define PDF_H

#include "rtweekend.h"
#include "ONB.h"
#include "hitable.h"

class pdf {
public: 
	virtual ~pdf() {}
	virtual double value(const vec3& direction) const = 0; 
	virtual vec3 generate() const = 0;
};


inline vec3 random_cosine_direction() {
	auto r1 = random_double();
	auto r2 = random_double();
	auto z = sqrt(1 - r2);

	auto phi = 2 * pi * r1;
	auto x = cos(phi) * sqrt(r2);
	auto y = sin(phi) * sqrt(r2);
	return vec3(x, y, z);
}

class cosine_pdf : public pdf {
public: 
	cosine_pdf() {}
	cosine_pdf (const vec3& w) { uvw.build_from_w(w); }
	virtual double value(const vec3& direction) {
		auto cosine = dot(unit_vector(direction), uvw.w());
		return (cosine <= 0) ? 0 : cosine / pi;
	}
	virtual vec3 generate()const override {
		return uvw.local(random_cosine_direction());
	}

public: 
	onb uvw;
};

class hitable_pdf : public pdf {
public: 
	hitable_pdf(shared_ptr<hitable> p, const point3& origin) : ptr(p), o(origin) {}

	virtual double value(const vec3& direction)const override {
		return ptr->pdf_value(o, direction); 
	}

	virtual vec3 generate() const override {
		return ptr->random(o);
	}

public: 
	point3 o; 
	shared_ptr<hitable> ptr;
};
#endif // !PDF_H