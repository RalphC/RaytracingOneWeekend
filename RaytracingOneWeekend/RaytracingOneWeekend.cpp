// RaytracingOneWeekend.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>

#include <float.h>

#include "ray.hpp"
#include "hitablelist.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"

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

vec3 color(const ray& r, hitable *world) {
	hit_record _rec;
	if (world->hit(r, 0.0001, FLT_MAX, _rec)) {
		vec3 target = _rec.p + _rec.normal + random_in_unit_sphere();
		return 0.5 * color(ray(_rec.p, target - _rec.p), world);
		//return 0.5 * vec3(_rec.normal.x() + 1.0, _rec.normal.y() + 1.0, _rec.normal.z() + 1.0);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record _rec;
	if (world->hit(r, 0.0001, FLT_MAX, _rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && _rec.p_mat->scatter(r, _rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0.0, 0.0, 0.0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = drand();
			vec3 center(a + 0.9*drand(), 0.2, b + 0.9*drand());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand()*drand(), drand()*drand(), drand()*drand())));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5*(1 + drand()), 0.5*(1 + drand()), 0.5*(1 + drand())), 0.5*drand()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main()
{
	int nx = 1200;
	int ny = 800;
	int ns = 256;

	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	//vec3 lower_left(-2.0, -1.0, -1.0);
	//vec3 horizontal(4.0, 0.0, 0.0);
	//vec3 vertical(0.0, 2.0, 0.0);
	//vec3 origin(0.0, 0.0, 0.0);

	/*hitable* list[2];
	float R = cos(M_PI_4);
	list[0] = new sphere(vec3(-R, 0.0, -1.0), R, new lambertian(vec3(0.0, 0.0, 1.0)));
	list[1] = new sphere(vec3(R, 0.0, -1.0), R, new lambertian(vec3(1.0, 0.0, 0.0)));
	hitable* world = new hitable_list(list, 2);*/

	/*hitable* list[5];
	list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2), 1));
	list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1.0, 0.0, -1.0), -0.45, new dielectric(1.5));
	hitable* world = new hitable_list(list, 5);*/

	hitable* world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), 2.0, (lookfrom - lookat).length());

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {

			vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < ns; s++) {

				float u = float(i + drand()) / float(nx);
				float v = float(j + drand()) / float(ny);

				ray r = cam.get_ray(u, v);
				//ray r(origin, lower_left + u * horizontal + v * vertical);
				r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}


	return 0;
}

