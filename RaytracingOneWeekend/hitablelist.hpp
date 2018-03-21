#pragma once

#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.hpp"

class hitable_list : public hitable {
public:
	hitable_list() {}
	hitable_list(hitable** l, int n) {
		list = l;
		list_size = n;
	}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

	hitable** list;
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record _rec;
	bool b_hit = false;
	double closest = t_max;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest, _rec)) {
			b_hit = true;
			closest = _rec.t;
			rec = _rec;
		}
	}
	return b_hit;
}


#endif // !HITABLELISTH
