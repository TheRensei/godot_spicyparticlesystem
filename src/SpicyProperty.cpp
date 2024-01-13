#include "SpicyProperty.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

#pragma region EmissionShapes

void godot::PointEmissionShape::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_point", "point"), &PointEmissionShape::set_point);
	ClassDB::bind_method(D_METHOD("get_point"), &PointEmissionShape::get_point);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "point"), "set_point", "get_point");
}

Vector3 godot::PointEmissionShape::get_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	return point;
}

void godot::PointEmissionShape::set_point(const Vector3& p_point)
{
	point = p_point;
}

Vector3 godot::PointEmissionShape::get_point() const
{
	return point;
}


void godot::BoxEmissionShape::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_box_extents", "box_extents"), &BoxEmissionShape::set_box_extents);
	ClassDB::bind_method(D_METHOD("get_box_extents"), &BoxEmissionShape::get_box_extents);

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "box_extents"), "set_box_extents", "get_box_extents");
}

Vector3 godot::BoxEmissionShape::get_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	return Vector3(
		rng->randf_range(-box_extents.x, box_extents.x),
		rng->randf_range(-box_extents.y, box_extents.y),
		rng->randf_range(-box_extents.z, box_extents.z)
	);
}

void godot::BoxEmissionShape::set_box_extents(const Vector3& p_box_extents)
{
	box_extents = p_box_extents;
}

Vector3 godot::BoxEmissionShape::get_box_extents() const
{
	return box_extents;
}

void godot::SphereEmissionShape::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_radius", "radius"), &SphereEmissionShape::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &SphereEmissionShape::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius_thickness", "radius_thickness"), &SphereEmissionShape::set_radius_thickness);
	ClassDB::bind_method(D_METHOD("get_radius_thickness"), &SphereEmissionShape::get_radius_thickness);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius_thickness", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_radius_thickness", "get_radius_thickness");
}

Vector3 godot::SphereEmissionShape::get_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	Vector3 v = Vector3(0, 0, 0);
	real_t l = 0.0;
	do {
		v = Vector3(
			rng->randf() * 2.0 - 1.0,
			rng->randf() * 2.0 - 1.0,
			rng->randf() * 2.0 - 1.0
		);

		l = v.length_squared();
	} while (l > 1.0);

	l = Math::lerp(l, radius_thickened, radius_thickened);
	v = v.normalized() * radius * l;
	return v;
}

void godot::SphereEmissionShape::set_radius(real_t p_radius)
{
	radius = p_radius;
}

real_t godot::SphereEmissionShape::get_radius() const
{
	return radius;
}

void godot::SphereEmissionShape::set_radius_thickness(real_t p_radius_thickness)
{
	radius_thickness = p_radius_thickness;
	radius_thickened = 1 - radius_thickness;;
}

real_t godot::SphereEmissionShape::get_radius_thickness() const
{
	return radius_thickness;
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////

void godot::SpicyFloatProperty::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_uniform", "uniform"), &SpicyFloatProperty::set_uniform);
	ClassDB::bind_method(D_METHOD("get_uniform"), &SpicyFloatProperty::get_uniform);

	ClassDB::bind_method(D_METHOD("set_random", "random"), &SpicyFloatProperty::set_random);
	ClassDB::bind_method(D_METHOD("get_random"), &SpicyFloatProperty::get_random);

	ClassDB::bind_method(D_METHOD("set_curve", "curve"), &SpicyFloatProperty::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &SpicyFloatProperty::get_curve);

	BIND_ENUM_CONSTANT(SPICY_FLOAT_TYPE_UNIFORM);
	BIND_ENUM_CONSTANT(SPICY_FLOAT_TYPE_RANDOM);
	BIND_ENUM_CONSTANT(SPICY_FLOAT_TYPE_CURVE);
}

real_t godot::SpicyFloatProperty::_get_uniform_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	return uniform;
}

real_t godot::SpicyFloatProperty::_get_random_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	return rng->randf_range(random.x, random.y);
}

real_t godot::SpicyFloatProperty::_get_curve_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	real_t value = curve_default;

	if (curve.is_valid())
		value = curve->sample(sample);

	return real_t(value);
}

real_t godot::SpicyFloatProperty::get_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	switch (prop_type)
	{
	case SPICY_FLOAT_TYPE_UNIFORM:
		return _get_uniform_value(rng, sample);
		break;
	case SPICY_FLOAT_TYPE_RANDOM:
		return _get_random_value(rng, sample);
		break;
	case SPICY_FLOAT_TYPE_CURVE:
		return _get_curve_value(rng, sample);
		break;
	}
	return 0;
}

void godot::SpicyFloatProperty::set_available_types(BitField<SpicyFloatType> p_types)
{
	display_prop_types = p_types;

	type_names = "";

	if (display_prop_types & SPICY_FLOAT_TYPE_UNIFORM)
		type_names += "Uniform:1,";
	if (display_prop_types & SPICY_FLOAT_TYPE_RANDOM)
		type_names += "Random:2,";
	if (display_prop_types & SPICY_FLOAT_TYPE_CURVE)
		type_names += "Curve:4";

	if (type_names.ends_with(","))
	{
		type_names = type_names.trim_suffix(",");
	}

}


void godot::SpicyFloatProperty::set_prop_type(SpicyFloatType p_prop_type)
{
	prop_type = p_prop_type;
}

godot::SpicyFloatProperty::SpicyFloatType godot::SpicyFloatProperty::get_prop_type() const
{
	return prop_type;
}

void godot::SpicyFloatProperty::set_property_name(const StringName& p_name)
{
	prop_name_str = p_name;
	prop_type_str = prop_name_str + (StringName)"_type";
}

void godot::SpicyFloatProperty::set_default_type(SpicyFloatType p_prop_type)
{
	prop_type = p_prop_type;
	this->set_meta("prop_type", p_prop_type);
}

void godot::SpicyFloatProperty::set_default_uniform(real_t p_uniform)
{
	uniform = p_uniform;
	this->set_meta("uniform", p_uniform);
	curve_default = p_uniform;
}

void godot::SpicyFloatProperty::set_default_random(const Vector2& p_random)
{
	random = p_random;
	this->set_meta("random", p_random);
}

bool godot::SpicyFloatProperty::get_property_list(List<PropertyInfo>* p_list, bool show_type_choice) const
{
	if (show_type_choice)
		p_list->push_back(PropertyInfo(Variant::INT, prop_type_str, PROPERTY_HINT_ENUM, (StringName)type_names));

	switch (prop_type)
	{
	case SPICY_FLOAT_TYPE_UNIFORM:
		p_list->push_back(PropertyInfo(Variant::FLOAT, prop_name_str));
		break;
	case SPICY_FLOAT_TYPE_RANDOM:
		p_list->push_back(PropertyInfo(Variant::VECTOR2, prop_name_str));
		break;
	case SPICY_FLOAT_TYPE_CURVE:
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_str, PROPERTY_HINT_RESOURCE_TYPE, "Curve"));
		break;
	}

	return true;
}

bool godot::SpicyFloatProperty::get_property(const StringName& p_property, Variant& r_value) const
{
	if (p_property == prop_type_str)
	{
		r_value = get_prop_type();
		return true;
	}

	switch (prop_type)
	{
	case SPICY_FLOAT_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			r_value = uniform;
			return true;
		}
		break;
	case SPICY_FLOAT_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			r_value = random;
			return true;
		};
		break;
	case SPICY_FLOAT_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			r_value = curve;
			return true;
		}
		break;
	}
	return false;
}


bool godot::SpicyFloatProperty::set_property(const StringName& p_property, const Variant& p_value, const Ref<Resource>& r)
{
	if (p_property == prop_type_str) {
		set_prop_type((SpicyFloatType)((int)(p_value)));
		r->notify_property_list_changed();
		return true;
	}

	switch (prop_type)
	{
	case SPICY_FLOAT_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			uniform = p_value;
			return true;
		}
		break;
	case SPICY_FLOAT_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			random = p_value;
			return true;
		};
		break;
	case SPICY_FLOAT_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			curve = p_value;
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyFloatProperty::can_property_revert(const StringName& p_property)
{
	if (p_property == prop_type_str)
	{
		return true;
	}

	switch (prop_type)
	{
	case SPICY_FLOAT_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			return true;
		}
		break;
	case SPICY_FLOAT_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			return true;
		};
		break;
	case SPICY_FLOAT_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyFloatProperty::get_property_revert(const StringName& p_property, Variant& r_value)
{
	if (p_property == prop_type_str)
	{
		r_value = this->has_meta("prop_type") ? this->get_meta("prop_type") : Variant(SPICY_FLOAT_TYPE_UNIFORM);
		return true;
	}

	switch (prop_type)
	{
	case SPICY_FLOAT_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			r_value = this->has_meta("uniform") ? this->get_meta("uniform") : Variant(0);
			return true;
		}
		break;
	case SPICY_FLOAT_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			r_value = this->has_meta("random") ? this->get_meta("random") : Variant((Vector2(0, 1));
			return true;
		};
		break;
	case SPICY_FLOAT_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			//r_value = curve;
			return true;
		}
		break;
	}
	return false;
}

void godot::SpicyFloatProperty::set_uniform(real_t p_uniform)
{
	set_prop_type(SPICY_FLOAT_TYPE_UNIFORM);
	uniform = p_uniform;
}

real_t godot::SpicyFloatProperty::get_uniform() const
{
	return uniform;
}

void godot::SpicyFloatProperty::set_random(const Vector2& p_random)
{
	set_prop_type(SPICY_FLOAT_TYPE_RANDOM);
	random = p_random;
}

Vector2 godot::SpicyFloatProperty::get_random() const
{
	return random;
}

void godot::SpicyFloatProperty::set_curve(const Ref<Curve>& p_curve)
{
	set_prop_type(SPICY_FLOAT_TYPE_CURVE);
	curve = p_curve;
}

Ref<Curve> godot::SpicyFloatProperty::get_curve() const
{
	return curve;
}

//////////////////////////////////////////////////////////////////////////

void godot::SpicyVector3Property::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_uniform", "uniform"), &SpicyVector3Property::set_uniform);
	ClassDB::bind_method(D_METHOD("get_uniform"), &SpicyVector3Property::get_uniform);

	ClassDB::bind_method(D_METHOD("set_random", "random_min", "random_min"), &SpicyVector3Property::set_random);
	ClassDB::bind_method(D_METHOD("get_random_min"), &SpicyVector3Property::get_random_min);
	ClassDB::bind_method(D_METHOD("get_random_max"), &SpicyVector3Property::get_random_max);

	ClassDB::bind_method(D_METHOD("set_curve", "curve"), &SpicyVector3Property::set_curve);
	ClassDB::bind_method(D_METHOD("get_curve"), &SpicyVector3Property::get_curve);

	ClassDB::bind_method(D_METHOD("set_curve_xyz", "curve_x", "curve_y", "curve_z"), &SpicyVector3Property::set_curve_xyz);
	ClassDB::bind_method(D_METHOD("get_curve_x"), &SpicyVector3Property::get_curve_x);
	ClassDB::bind_method(D_METHOD("get_curve_y"), &SpicyVector3Property::get_curve_y);
	ClassDB::bind_method(D_METHOD("get_curve_z"), &SpicyVector3Property::get_curve_z);

	BIND_ENUM_CONSTANT(SPICY_VECTOR_TYPE_UNIFORM);
	BIND_ENUM_CONSTANT(SPICY_VECTOR_TYPE_RANDOM);
	BIND_ENUM_CONSTANT(SPICY_VECTOR_TYPE_CURVE);
	BIND_ENUM_CONSTANT(SPICY_VECTOR_TYPE_CURVE_XYZ);
}

Vector3 godot::SpicyVector3Property::_get_uniform_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	return uniform;
}

Vector3 godot::SpicyVector3Property::_get_random_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	Vector3 v = Vector3(
		rng->randf_range(random_min.x, random_max.x),
		rng->randf_range(random_min.y, random_max.y),
		rng->randf_range(random_min.z, random_max.z));
	return v;
}


Vector3 godot::SpicyVector3Property::_get_curve_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	Vector3 v = curve_default;

	if (curve_x.is_valid()) {
		real_t s = curve_x->sample(sample);

		v = Vector3(s, s, s);
	}

	return v;
}

Vector3 godot::SpicyVector3Property::_get_curve_xyz_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	Vector3 v = curve_default;

	if (curve_x.is_valid())
		v.x = curve_x->sample(sample);
	if (curve_y.is_valid())
		v.y = curve_y->sample(sample);
	if (curve_z.is_valid())
		v.z = curve_z->sample(sample);

	return v;
}

Vector3 godot::SpicyVector3Property::get_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	switch (prop_type)
	{
	case SPICY_VECTOR_TYPE_UNIFORM:
		return _get_uniform_value(rng, sample);
		break;
	case SPICY_VECTOR_TYPE_RANDOM:
		return _get_random_value(rng, sample);
		break;
	case SPICY_VECTOR_TYPE_CURVE:
		return _get_curve_value(rng, sample);
		break;
	case SPICY_VECTOR_TYPE_CURVE_XYZ:
		return _get_curve_xyz_value(rng, sample);
		break;
	}
	return Vector3(0, 0, 0);
}

void godot::SpicyVector3Property::set_available_types(BitField<SpicyVector3Type> p_types)
{
	display_prop_types = p_types;

	type_names = "";

	if (display_prop_types & SPICY_VECTOR_TYPE_UNIFORM)
		type_names += "Uniform:1,";
	if (display_prop_types & SPICY_VECTOR_TYPE_RANDOM)
		type_names += "Random:2,";
	if (display_prop_types & SPICY_VECTOR_TYPE_CURVE)
		type_names += "Curve:4,";
	if (display_prop_types & SPICY_VECTOR_TYPE_CURVE_XYZ)
		type_names += "CurveXYZ:8";

	if (type_names.ends_with(","))
	{
		type_names = type_names.trim_suffix(",");
	}

}


void godot::SpicyVector3Property::set_prop_type(SpicyVector3Type p_prop_type)
{
	prop_type = p_prop_type;
}

godot::SpicyVector3Property::SpicyVector3Type godot::SpicyVector3Property::get_prop_type() const
{
	return prop_type;
}

void godot::SpicyVector3Property::set_property_name(const StringName& p_name)
{
	prop_name_str = p_name;
	prop_type_str = p_name + (StringName)"_type";
	prop_name_randmin_str = p_name + (StringName)"_min";
	prop_name_randmax_str = p_name + (StringName)"_max";
	prop_name_curve_x_str = p_name + (StringName)"_x";
	prop_name_curve_y_str = p_name + (StringName)"_y";
	prop_name_curve_z_str = p_name + (StringName)"_z";
}

void godot::SpicyVector3Property::set_default_type(SpicyVector3Type p_prop_type)
{
	prop_type = p_prop_type;
	this->set_meta("prop_type", p_prop_type);
}

void godot::SpicyVector3Property::set_default_uniform(Vector3 p_uniform)
{
	uniform = p_uniform;
	this->set_meta("uniform", p_uniform);
	curve_default = p_uniform;
}

void godot::SpicyVector3Property::set_default_random(const Vector3& p_random_min, const Vector3& p_random_max)
{
	random_min = p_random_min;
	random_max = p_random_max;
	this->set_meta("random_min", p_random_min);
	this->set_meta("random_max", p_random_max);
}

bool godot::SpicyVector3Property::get_property_list(List<PropertyInfo>* p_list, bool show_type_choice) const
{
	if (show_type_choice)
		p_list->push_back(PropertyInfo(Variant::INT, prop_type_str, PROPERTY_HINT_ENUM, (StringName)type_names));

	switch (prop_type)
	{
	case SPICY_VECTOR_TYPE_UNIFORM:
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prop_name_str));
		break;
	case SPICY_VECTOR_TYPE_RANDOM:
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prop_name_randmin_str));
		p_list->push_back(PropertyInfo(Variant::VECTOR3, prop_name_randmax_str));
		break;
	case SPICY_VECTOR_TYPE_CURVE:
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_str, PROPERTY_HINT_RESOURCE_TYPE, "Curve"));
		break;
	case SPICY_VECTOR_TYPE_CURVE_XYZ:
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_curve_x_str, PROPERTY_HINT_RESOURCE_TYPE, "Curve"));
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_curve_y_str, PROPERTY_HINT_RESOURCE_TYPE, "Curve"));
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_curve_z_str, PROPERTY_HINT_RESOURCE_TYPE, "Curve"));
		break;
	}

	return true;
}

bool godot::SpicyVector3Property::get_property(const StringName& p_property, Variant& r_value) const
{
	if (p_property == prop_type_str)
	{
		r_value = get_prop_type();
		return true;
	}

	switch (prop_type)
	{
	case SPICY_VECTOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			r_value = uniform;
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_RANDOM:
		if (p_property == prop_name_randmin_str)
		{
			r_value = random_min;
			return true;
		}
		else if (p_property == prop_name_randmax_str)
		{
			r_value = random_max;
			return true;
		};
		break;
	case SPICY_VECTOR_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			r_value = curve_x;
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_CURVE_XYZ:
		if (p_property == prop_name_curve_x_str)
		{
			r_value = curve_x;
			return true;
		}
		else if (p_property == prop_name_curve_y_str)
		{
			r_value = curve_y;
			return true;
		}
		else if (p_property == prop_name_curve_z_str)
		{
			r_value = curve_z;
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyVector3Property::set_property(const StringName& p_property, const Variant& p_value, const Ref<Resource>& r)
{
	if (p_property == prop_type_str) {
		set_prop_type((SpicyVector3Type)((int)(p_value)));
		r->notify_property_list_changed();
		return true;
	}

	switch (prop_type)
	{
	case SPICY_VECTOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			uniform = p_value;
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_RANDOM:
		if (p_property == prop_name_randmin_str)
		{
			random_min = p_value;
			return true;
		}
		else if (p_property == prop_name_randmax_str)
		{
			random_max = p_value;
			return true;
		};
		break;
	case SPICY_VECTOR_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			curve_x = p_value;
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_CURVE_XYZ:
		if (p_property == prop_name_curve_x_str)
		{
			curve_x = p_value;
			return true;
		}
		else if (p_property == prop_name_curve_y_str)
		{
			curve_y = p_value;
			return true;
		}
		else if (p_property == prop_name_curve_z_str)
		{
			curve_z = p_value;
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyVector3Property::can_property_revert(const StringName& p_property)
{
	if (p_property == prop_type_str) {
		return true;
	}

	switch (prop_type)
	{
	case SPICY_VECTOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_RANDOM:
		if (p_property == prop_name_randmin_str)
		{
			return true;
		}
		else if (p_property == prop_name_randmax_str)
		{
			return true;
		};
		break;
	case SPICY_VECTOR_TYPE_CURVE:
		if (p_property == prop_name_str)
		{
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_CURVE_XYZ:
		if (p_property == prop_name_curve_x_str)
		{
			return true;
		}
		else if (p_property == prop_name_curve_y_str)
		{
			return true;
		}
		else if (p_property == prop_name_curve_z_str)
		{
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyVector3Property::get_property_revert(const StringName& p_property, Variant& r_value)
{
	if (p_property == prop_type_str)
	{
		r_value = this->has_meta("prop_type") ? this->get_meta("prop_type") : Variant(SPICY_VECTOR_TYPE_UNIFORM);
		return true;
	}

	switch (prop_type)
	{
	case SPICY_VECTOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			r_value = this->has_meta("uniform") ? this->get_meta("uniform") : Variant(Vector3(0, 0, 0));
			return true;
		}
		break;
	case SPICY_VECTOR_TYPE_RANDOM:
		if (p_property == prop_name_randmin_str)
		{
			r_value = this->has_meta("random_min") ? this->get_meta("random_min") : Variant(Vector3(0, 0, 0));
			return true;
		}
		else if (p_property == prop_name_randmax_str)
		{
			r_value = this->has_meta("random_max") ? this->get_meta("random_max") : Variant(Vector3(1, 1, 1));
			return true;
		};
		break;
	case SPICY_VECTOR_TYPE_CURVE:
		if (p_property == prop_name_curve_x_str)
		{
			return true;
		}
		else if (p_property == prop_name_curve_y_str)
		{
			return true;
		}
		else if (p_property == prop_name_curve_z_str)
		{
			return true;
		}
		break;
	}
	return false;
}

void godot::SpicyVector3Property::set_uniform(Vector3 p_uniform)
{
	set_prop_type(SPICY_VECTOR_TYPE_UNIFORM);
	uniform = p_uniform;
}

Vector3 godot::SpicyVector3Property::get_uniform() const
{
	return uniform;
}

void godot::SpicyVector3Property::set_random(const Vector3& p_random_min, const Vector3& p_random_max)
{
	set_prop_type(SPICY_VECTOR_TYPE_RANDOM);
	random_min = p_random_min;
	random_max = p_random_max;
}

Vector3 godot::SpicyVector3Property::get_random_min() const
{
	return random_min;
}

Vector3 godot::SpicyVector3Property::get_random_max() const
{
	return random_max;
}

void godot::SpicyVector3Property::set_curve(const Ref<Curve>& p_curve)
{
	set_prop_type(SPICY_VECTOR_TYPE_CURVE);
	curve_x = p_curve;
}

Ref<Curve> godot::SpicyVector3Property::get_curve() const
{
	return curve_x;
}

void godot::SpicyVector3Property::set_curve_xyz(const Ref<Curve>& p_curve_x, const Ref<Curve>& p_curve_y, const Ref<Curve>& p_curve_z)
{
	set_prop_type(SPICY_VECTOR_TYPE_CURVE_XYZ);
	curve_x = p_curve_x;
	curve_y = p_curve_y;
	curve_z = p_curve_z;
}

Ref<Curve> godot::SpicyVector3Property::get_curve_x() const
{
	return curve_x;
}

Ref<Curve> godot::SpicyVector3Property::get_curve_y() const
{
	return curve_y;
}

Ref<Curve> godot::SpicyVector3Property::get_curve_z() const
{
	return curve_z;
}

//////////////////////////////////////////////////////////////////////////

void godot::SpicyColorProperty::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_uniform", "uniform"), &SpicyColorProperty::set_uniform);
	ClassDB::bind_method(D_METHOD("get_uniform"), &SpicyColorProperty::get_uniform);

	ClassDB::bind_method(D_METHOD("set_random", "random"), &SpicyColorProperty::set_random);
	ClassDB::bind_method(D_METHOD("get_random"), &SpicyColorProperty::get_random);

	ClassDB::bind_method(D_METHOD("set_gradient", "gradient"), &SpicyColorProperty::set_gradient);
	ClassDB::bind_method(D_METHOD("get_gradient"), &SpicyColorProperty::get_gradient);

	BIND_ENUM_CONSTANT(SPICY_COLOR_TYPE_UNIFORM);
	BIND_ENUM_CONSTANT(SPICY_COLOR_TYPE_RANDOM);
	BIND_ENUM_CONSTANT(SPICY_COLOR_TYPE_GRADIENT);
}

Color godot::SpicyColorProperty::_get_uniform_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	return uniform;
}

Color godot::SpicyColorProperty::_get_random_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	real_t d_time = rng->randf_range(0.0, 1.0);
	Color new_color = Color(1.0, 1.0, 1.0);

	if (random.is_valid())
		new_color = random->sample(d_time);

	return new_color;
}


Color godot::SpicyColorProperty::_get_gradient_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	Color new_color = Color(1.0, 1.0, 1.0);

	if (gradient.is_valid())
		new_color = gradient->sample(sample);

	return new_color;
}

Color godot::SpicyColorProperty::get_value(const Ref<RandomNumberGenerator>& rng, real_t sample) const
{
	switch (prop_type)
	{
	case SPICY_COLOR_TYPE_UNIFORM:
		return _get_uniform_value(rng, sample);
		break;
	case SPICY_COLOR_TYPE_RANDOM:
		return _get_random_value(rng, sample);
		break;
	case SPICY_COLOR_TYPE_GRADIENT:
		return _get_gradient_value(rng, sample);
		break;
	}
	return Color(1.0, 1.0, 1.0);
}

void godot::SpicyColorProperty::set_available_types(BitField<SpicyColorType> p_types)
{
	display_prop_types = p_types;

	type_names = "";

	if (display_prop_types & SPICY_COLOR_TYPE_UNIFORM)
		type_names += "Uniform:1,";
	if (display_prop_types & SPICY_COLOR_TYPE_RANDOM)
		type_names += "Random:2,";
	if (display_prop_types & SPICY_COLOR_TYPE_GRADIENT)
		type_names += "Gradient:4";

	if (type_names.ends_with(","))
	{
		type_names = type_names.trim_suffix(",");
	}

}


void godot::SpicyColorProperty::set_prop_type(SpicyColorType p_prop_type)
{
	prop_type = p_prop_type;
}

godot::SpicyColorProperty::SpicyColorType godot::SpicyColorProperty::get_prop_type() const
{
	return prop_type;
}

void godot::SpicyColorProperty::set_property_name(const StringName& p_name)
{
	prop_name_str = p_name;
	prop_type_str = p_name + (StringName)"_type";
}

void godot::SpicyColorProperty::set_default_type(SpicyColorType p_prop_type)
{
	prop_type = p_prop_type;
	this->set_meta("prop_type", p_prop_type);
}

void godot::SpicyColorProperty::set_default_uniform(Color p_uniform)
{
	uniform = p_uniform;
	this->set_meta("uniform", p_uniform);
}

bool godot::SpicyColorProperty::get_property_list(List<PropertyInfo>* p_list, bool show_type_choice) const
{
	if (show_type_choice)
		p_list->push_back(PropertyInfo(Variant::INT, prop_type_str, PROPERTY_HINT_ENUM, (StringName)type_names));

	switch (prop_type)
	{
	case SPICY_COLOR_TYPE_UNIFORM:
		p_list->push_back(PropertyInfo(Variant::COLOR, prop_name_str));
		break;
	case SPICY_COLOR_TYPE_RANDOM:
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_str, PROPERTY_HINT_RESOURCE_TYPE, "Gradient"));
		break;
	case SPICY_COLOR_TYPE_GRADIENT:
		p_list->push_back(PropertyInfo(Variant::OBJECT, prop_name_str, PROPERTY_HINT_RESOURCE_TYPE, "Gradient"));
		break;
	}

	return true;
}

bool godot::SpicyColorProperty::get_property(const StringName& p_property, Variant& r_value) const
{
	if (p_property == prop_type_str)
	{
		r_value = get_prop_type();
		return true;
	}

	switch (prop_type)
	{
	case SPICY_COLOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			r_value = uniform;
			return true;
		}
		break;
	case SPICY_COLOR_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			r_value = random;
			return true;
		};
		break;
	case SPICY_COLOR_TYPE_GRADIENT:
		if (p_property == prop_name_str)
		{
			r_value = gradient;
			return true;
		}
		break;
	}
	return false;
}


bool godot::SpicyColorProperty::set_property(const StringName& p_property, const Variant& p_value, const Ref<Resource>& r)
{
	if (p_property == prop_type_str) {
		set_prop_type((SpicyColorType)((int)(p_value)));
		r->notify_property_list_changed();
		return true;
	}

	switch (prop_type)
	{
	case SPICY_COLOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			uniform = p_value;
			return true;
		}
		break;
	case SPICY_COLOR_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			random = p_value;
			return true;
		};
		break;
	case SPICY_COLOR_TYPE_GRADIENT:
		if (p_property == prop_name_str)
		{
			gradient = p_value;
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyColorProperty::can_property_revert(const StringName& p_property)
{
	if (p_property == prop_type_str)
	{
		return true;
	}

	switch (prop_type)
	{
	case SPICY_COLOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			return true;
		}
		break;
	case SPICY_COLOR_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			return true;
		};
		break;
	case SPICY_COLOR_TYPE_GRADIENT:
		if (p_property == prop_name_str)
		{
			return true;
		}
		break;
	}
	return false;
}

bool godot::SpicyColorProperty::get_property_revert(const StringName& p_property, Variant& r_value)
{
	if (p_property == prop_type_str)
	{
		r_value = this->has_meta("prop_type") ? this->get_meta("prop_type") : Variant(SPICY_COLOR_TYPE_UNIFORM);
		return true;
	}

	switch (prop_type)
	{
	case SPICY_COLOR_TYPE_UNIFORM:
		if (p_property == prop_name_str)
		{
			r_value = this->has_meta("uniform") ? this->get_meta("uniform") : Variant(0);
			return true;
		}
		break;
	case SPICY_COLOR_TYPE_RANDOM:
		if (p_property == prop_name_str)
		{
			//r_value = this->has_meta("random") ? this->get_meta("random") : Vector2(0, 1);
			return true;
		};
		break;
	case SPICY_COLOR_TYPE_GRADIENT:
		if (p_property == prop_name_str)
		{
			//r_value = curve;
			return true;
		}
		break;
	}
	return false;
}

void godot::SpicyColorProperty::set_uniform(Color p_uniform)
{
	set_prop_type(SPICY_COLOR_TYPE_UNIFORM);
	uniform = p_uniform;
}

Color godot::SpicyColorProperty::get_uniform() const
{
	return uniform;
}

void godot::SpicyColorProperty::set_random(const Ref<Gradient>& p_random)
{
	set_prop_type(SPICY_COLOR_TYPE_RANDOM);
	random = p_random;
}

Ref<Gradient> godot::SpicyColorProperty::get_random() const
{
	return random;
}

void godot::SpicyColorProperty::set_gradient(const Ref<Gradient>& p_gradient)
{
	set_prop_type(SPICY_COLOR_TYPE_GRADIENT);
	gradient = p_gradient;
}

Ref<Gradient> godot::SpicyColorProperty::get_gradient() const
{
	return gradient;
}