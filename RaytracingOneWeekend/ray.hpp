//#pragma once

#ifndef RAYH
#define RAYH

#include "vec3.hpp"

class ray {
public:
	ray() {};
	ray(const vec3 &a, const vec3 &b) { A = a; B = b; }
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};

inline double drand() { return rand() / (RAND_MAX + 1.0); }

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand(), drand(), drand()) - vec3(1.0, 1.0, 1.0);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}

// ref https://en.wikipedia.org/wiki/Schlick%27s_approximation
float schlick(float cosine, float ref_index) {
	float r0 = ((1 - ref_index) / (1 + ref_index)) * ((1 - ref_index) / (1 + ref_index));
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}

#endif // !RAYH

