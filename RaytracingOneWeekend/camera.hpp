#ifndef CAMERAH
#define CAMERAH

#define _USE_MATH_DEFINES
#include <math.h>
#include "ray.hpp"

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand(), drand(), 0.0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_distance) {
		lens_radius = aperture / 2;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		//lower_left = vec3(-half_width, -half_height, -1.0);
		lower_left = origin - half_width * u * focus_distance - half_height * v* focus_distance - w * focus_distance;
		/*horizontal = vec3(2 * half_width, 0.0, 0.0);
		vertical = vec3(0.0, 2 * half_height, 0.0);
		origin = vec3(0.0, 0.0, 0.0);*/
		horizontal = 2 * half_width * u * focus_distance;
		vertical = 2 * half_height * v * focus_distance;

	};

	ray get_ray(float s, float t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left + s * horizontal + t * vertical - origin - offset);
	}

	vec3 lower_left;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	vec3 u, v, w;
	float lens_radius;
};



#endif // !CAMERAH
