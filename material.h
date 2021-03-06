#pragma once
#ifndef MATERIALH
#define MATERIALH
#include "rtweekend.h"

#include "hitable.h"
#include "texture.h"
#include "pdf.h"


class material {
public:
    virtual color emitted(double u, double v, const point3& p) const {
        return color(0, 0, 0);
    }

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;

    virtual double specular_coef()const {
        return 0.0;
    }
    virtual double difuse_coef()const {
        return 0.0;
    }
    virtual double ambient_coef()const {
        return 0.0;
    }
};

class basic : public material {
public:
    basic(const color& a, double Krd, double Krs, double Kra) : albedo(make_shared<solid_color>(a)), K_rd(Krd < 1 ? Krd : 1), K_rs(Krs < 1 ? Krs : 1), K_ra(Kra < 1 ? Kra : 1) {}
    basic(shared_ptr<texture> a, double Krd, double Krs, double Kra) : albedo(a), K_rd(Krd < 1 ? Krd : 1), K_rs(Krs < 1 ? Krs : 1), K_ra(Kra < 1 ? Kra : 1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + K_rd * random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    virtual double specular_coef() const override {
        return K_rs;
    }
    virtual double difuse_coef() const override {
        return K_rd;
    }
    virtual double ambient_coef() const override {
        return K_ra;
    }


public:
    shared_ptr<texture> albedo;
    double K_rd;
    double K_rs;
    double K_ra;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<texture> albedo;
};


class metal : public material {
public:
    metal(const color& a, double f) : albedo(make_shared<solid_color>(a)), fuzz(f < 1 ? f : 1) {}
    metal(shared_ptr<texture> a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    shared_ptr<texture> albedo;
    double fuzz;
};



class Phong_material : public material {
public:
    Phong_material(const color& a, double Krd, double Krs) : albedo(make_shared<solid_color>(a)), K_rd(Krd < 1 ? Krd : 1), K_rs(Krs < 1 ? Krs : 1) {}
    Phong_material(shared_ptr<texture> a, double Krd, double Krs) : albedo(a), K_rd(Krd < 1 ? Krd : 1), K_rs(Krs < 1 ? Krs : 1) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + K_rd * random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    virtual double specular_coef() const override{
        return K_rs;
    }
    virtual double difuse_coef() const override {
        return K_rd;
    }

public:
    shared_ptr<texture> albedo;
    double K_rd;
    double K_rs;
};



class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};


class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        return false;
    }

    virtual color emitted(double u, double v, const point3& p) const override {
        return emit->value(u, v, p);
    }

public:
    shared_ptr<texture> emit;
};

class diffuse_light_s : public material {
public:
	diffuse_light_s(shared_ptr<texture> a) : emit(a) {}
	diffuse_light_s(color c, vec3 attenuation, double radius) : emit(make_shared<solid_color>(c)), c(attenuation), r(radius) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        return false;
    }

	virtual color emitted(double u, double v, const point3& p) const override {
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
