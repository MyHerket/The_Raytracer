#pragma once
#ifndef MATERIALH
#define MATERIALH

#include "rtweekend.h"
#include "hitable_list.h"

struct hit_record; 
class material {
public: 
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0; 
};


//This gives us attenuation albedo/p with probability p.
class lambertian : public material {
public: 
	lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	)const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		//Catch degenerate scatter direction;
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction); 
		attenuation = albedo; 
		return true;
	}

public: 
	color albedo;
};

class metal : public material {
public: 
	metal(const color& a, double f) : albedo(a), fuzz(f<1 ? f:1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal); 
		scattered = ray(rec.p, reflected+ fuzz*random_in_unit_sphere()); 
		attenuation = albedo; 
		return(dot(scattered.direction(), rec.normal) > 0);
	}

public: 
	color albedo; //Color del material
	double fuzz; //Hace que la refleccion sea borrosa, entre más cerca de 1 más borrosa.
};

#endif // !MATERIALH
