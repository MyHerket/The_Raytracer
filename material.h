#pragma once
#ifndef MATERIALH
#define MATERIALH

#include "rtweekend.h"
#include "hitable_list.h"
#include "Texture.h"
#include "pdf.h"

struct scatter_record {
	ray specular_ray; 
	bool is_specular; 
	color attenuation; 
	shared_ptr<pdf> pdf_ptr;
};

struct hit_record;


class material {
public: 
	virtual color emitted(
		const ray& r_in, const hit_record& rec, double u, double v, const point3& p)
		const {
		return color(0, 0, 0);
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec
	) const {
		return false;
	}
	virtual double scattering_pdf(
		const ray& r_in, const hit_record& rec, const ray& scattered
	) const {
		return 0;
	}
};


//This gives us attenuation albedo/p with probability p.
class lambertian : public material {
public: 
	lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
	lambertian(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, scatter_record& srec
	)const override {
		srec.is_specular = false; 
		srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
		srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
		return true;

	}

	double scattering_pdf(
		const ray& r_in, const hit_record& rec, const ray& scattered
	)const {
		auto cosine = dot(rec.normal, unit_vector(scattered.direction())); 
		return cosine < 0 ? 0 : cosine / pi;
	}

public: 
	shared_ptr<texture> albedo;
};

class metal : public material {
public: 
	metal(const color& a, double f) : albedo(a), fuzz(f<1 ? f:1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, scatter_record& srec
	) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal); 
		srec.specular_ray = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		srec.attenuation = albedo;
		srec.is_specular = true; 
		srec.pdf_ptr = 0;
		return true;
	}

public: 
	color albedo; //Color del material
	double fuzz; //Hace que la refleccion sea borrosa, entre m�s cerca de 1 m�s borrosa.
};


//Material with refraction index
class dielectric : public material {
public: 
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, scatter_record& srec
	)const override {
		srec.is_specular = true;
		srec.pdf_ptr = nullptr; 
		srec.attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = (rec.front_face ? (1.0 / ir) : ir);

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - (cos_theta * cos_theta));

		bool cannot_refract = (refraction_ratio * sin_theta > 1.0);
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		srec.specular_ray = ray(rec.p, direction, r_in.time());
		return true;
	}


	double ir; //Index of refraction

private:
	static double reflectance(double cosine, double ref_idx) {
		//Use Schlick's approximation for reflectanc
		double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1.0 - cosine), 5);
	}
};

class diffuse_light : public material {
public: 
	diffuse_light(shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, scatter_record& srec
	) const override {
		return false;
	}

	virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override {
		return emit->value(u, v, p);
	}

	shared_ptr<texture> emit;
};

class diffuse_light_s : public material {
public:
	diffuse_light_s(shared_ptr<texture> a) : emit(a) {}
	diffuse_light_s(color c, vec3 attenuation, double radius) : emit(make_shared<solid_color>(c)), c(attenuation), r(radius) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, scatter_record& srec
	) const override {
		return false;
	}

	virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override {
		double constant;
		if (c.lenght() == 0) 
			constant = 0;
		else 
			constant = 1 / (c[0] + (c[1] * r) + (c[2] * r * r));

		return constant * emit->value(u, v, p);
	}

	shared_ptr<texture> emit;
	vec3 c; 
	double r;
};
#endif // !MATERIALH
