// RaytracingOneWeekend.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include "ray.hpp"

bool hit_sphereb(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminat = b * b - 4 * a * c;
	return discriminat > 0;
}

float hit_spheref(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminat = b * b - 4 * a * c;

	if (discriminat < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminat)) / (2.0 * a);
	}
}

vec3 color(const ray& r) {
	float f = hit_spheref(vec3(0.0, 0.0, -1.0), 0.5, r);
	if (f > 0) {
		vec3 n = unit_vector(r.point_at_parameter(f) - vec3(0.0, 0.0, -1.0));
		return 0.5 * vec3(n.x() + 1.0, n.y() + 1.0, n.z() + 1.0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	int nx = 200;
	int ny = 100;

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lower_left(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			
			ray r(origin, lower_left + u * horizontal + v * vertical);
			vec3 col = color(r);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}


	return 0;
}

