#include "SpicyParticleUpdater.hpp"

using namespace godot;

void godot::LifetimeUpdater::_bind_methods()
{
}

void godot::LifetimeUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;
	const float localDT = static_cast<float>(dt);

	if (endId == 0) return;

	for (size_t i = 0; i < endId; ++i)
	{
		p_data->life_remaining[i] -= localDT;
		p_data->normalized_lifetime[i] = 1.0f - (p_data->life_remaining[i] / p_data->lifetime[i]);
		if (p_data->life_remaining[i] <= 0.0f)
		{
			p_data->kill(i);
			endId = p_data->get_last_alive_index();
		}
	}
}

void godot::PositionUpdater::_bind_methods()
{
}

void godot::PositionUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;
	const float localDT = static_cast<float>(dt);

	for (size_t i = 0; i < endId; ++i)
	{
		p_data->position[i] += ((p_data->velocity[i] + p_data->current_velocity[i] + p_data->acceleration[i]) * localDT);
	}
}

//////////////////////////////////////////////////////////////////////////

void godot::VelocityUpdater::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_velocity_over_life_property", "velocity_over_life"), &VelocityUpdater::set_velocity_over_life_property);
	ClassDB::bind_method(D_METHOD("get_velocity_over_life_property"), &VelocityUpdater::get_velocity_over_life_property);
	ClassDB::bind_method(D_METHOD("set_radial_velocity_property", "radial_velocity"), &VelocityUpdater::set_radial_velocity_property);
	ClassDB::bind_method(D_METHOD("get_radial_velocity_property"), &VelocityUpdater::get_radial_velocity_property);
	ClassDB::bind_method(D_METHOD("set_orbital_velocity_property", "orbital_velocity"), &VelocityUpdater::set_orbital_velocity_property);
	ClassDB::bind_method(D_METHOD("get_orbital_velocity_property"), &VelocityUpdater::get_orbital_velocity_property);
}

godot::VelocityUpdater::VelocityUpdater()
{
	Ref<SpicyVector3Property> vel_of_life(memnew(SpicyVector3Property));
	velocity_over_life = vel_of_life;
	vel_of_life->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
	vel_of_life->set_property_name("velocity_over_life");
	vel_of_life->set_default_uniform(Vector3(0, 0, 0));
	vel_of_life->set_default_type(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);

	Ref<SpicyFloatProperty> rad_vel(memnew(SpicyFloatProperty));
	radial_velocity = rad_vel;
	rad_vel->set_available_types(SpicyFloatProperty::SpicyFloatType::SPICY_FLOAT_TYPE_UNIFORM | SpicyFloatProperty::SpicyFloatType::SPICY_FLOAT_TYPE_CURVE);
	rad_vel->set_property_name("radial_velocity");
	rad_vel->set_default_uniform(0);

	Ref<SpicyVector3Property> orb_vel(memnew(SpicyVector3Property));
	orbital_velocity = orb_vel;
	orb_vel->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_UNIFORM | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
	orb_vel->set_property_name("orbital_velocity");
	orb_vel->set_default_uniform(Vector3(0, 0, 0));
}

void godot::VelocityUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;
	const float localDT = static_cast<float>(dt);

	for (size_t i = 0; i < endId; ++i)
	{
		Vector3 new_velocity = Vector3(0, 0, 0);

		if(velocity_over_life.is_valid())
			new_velocity += velocity_over_life->get_value(p_data->rng, p_data->normalized_lifetime[i]);

		if(radial_velocity.is_valid())
			new_velocity += (p_data->position[i] - p_data->emitter_transform.origin).normalized() * radial_velocity->get_value(p_data->rng, p_data->normalized_lifetime[i]);

		if (orbital_velocity.is_valid())
		{
			Vector3 rotationAngles = orbital_velocity->get_value(p_data->rng, p_data->normalized_lifetime[i]);
			Vector3 radians(Math::deg_to_rad(rotationAngles.x), Math::deg_to_rad(rotationAngles.y), Math::deg_to_rad(rotationAngles.z));
			rotationAngles = radians * localDT;

			double translatedX = p_data->position[i].x - p_data->emitter_transform.origin.x;
			double translatedY = p_data->position[i].y - p_data->emitter_transform.origin.y;
			double translatedZ = p_data->position[i].z - p_data->emitter_transform.origin.z;

			// Perform the rotation around the X-axis
			double rotatedYX = translatedY * cos(rotationAngles.x) - translatedZ * sin(rotationAngles.x);
			double rotatedZX = translatedY * sin(rotationAngles.x) + translatedZ * cos(rotationAngles.x);

			// Perform the rotation around the Y-axis
			double rotatedXY = translatedX * cos(rotationAngles.y) + rotatedZX * sin(rotationAngles.y);
			double rotatedZY = -translatedX * sin(rotationAngles.y) + rotatedZX * cos(rotationAngles.y);

			// Perform the rotation around the Z-axis
			double rotatedXZ = rotatedXY * cos(rotationAngles.z) - rotatedYX * sin(rotationAngles.z);
			double rotatedYZ = rotatedXY * sin(rotationAngles.z) + rotatedYX * cos(rotationAngles.z);

			// Translate the point back to its original position
			double finalX = rotatedXZ + p_data->emitter_transform.origin.x;
			double finalY = rotatedYZ + p_data->emitter_transform.origin.y;
			double finalZ = rotatedZY + p_data->emitter_transform.origin.z;

			Vector3 v = Vector3(finalX, finalY, finalZ);

			//new_velocity += (v - p_data->position[i]);
			p_data->position[i] = v;
		}

		p_data->current_velocity[i] = new_velocity;
	}
}

void godot::VelocityUpdater::set_velocity_over_life_property(const Ref<SpicyVector3Property>& p_velocity_property)
{
	velocity_over_life = p_velocity_property;

	if (velocity_over_life.is_null()) {
		Ref<SpicyVector3Property> vel_of_life(memnew(SpicyVector3Property));
		velocity_over_life = vel_of_life;
		vel_of_life->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
		vel_of_life->set_property_name("velocity_over_life");
		vel_of_life->set_default_uniform(Vector3(0, 0, 0));
	}
}

Ref<SpicyVector3Property> godot::VelocityUpdater::get_velocity_over_life_property() const
{
	return velocity_over_life;
}

void godot::VelocityUpdater::set_radial_velocity_property(const Ref<SpicyFloatProperty>& p_radial_velocity_property)
{
	radial_velocity = p_radial_velocity_property;

	if (radial_velocity.is_null()) {
		Ref<SpicyFloatProperty> rad_vel(memnew(SpicyFloatProperty));
		radial_velocity = rad_vel;
		rad_vel->set_available_types(SpicyFloatProperty::SpicyFloatType::SPICY_FLOAT_TYPE_UNIFORM | SpicyFloatProperty::SpicyFloatType::SPICY_FLOAT_TYPE_CURVE);
		rad_vel->set_property_name("radial_velocity");
		rad_vel->set_default_uniform(0);
	}
}

Ref<SpicyFloatProperty> godot::VelocityUpdater::get_radial_velocity_property() const
{
	return radial_velocity;
}

void godot::VelocityUpdater::set_orbital_velocity_property(const Ref<SpicyVector3Property>& p_orbital_velocity_property)
{
	orbital_velocity = p_orbital_velocity_property;

	if (orbital_velocity.is_null()) {
		Ref<SpicyVector3Property> orb_vel(memnew(SpicyVector3Property));
		orbital_velocity = orb_vel;
		orb_vel->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_UNIFORM | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
		orb_vel->set_property_name("orbital_velocity");
		orb_vel->set_default_uniform(Vector3(0, 0, 0));
	}
}

Ref<SpicyVector3Property> godot::VelocityUpdater::get_orbital_velocity_property() const
{
	return orbital_velocity;
}

void godot::VelocityUpdater::_get_property_list(List<PropertyInfo>* r_props) const
{
	if (velocity_over_life.is_valid()) {
		velocity_over_life->get_property_list(r_props, false);
	}
	if (radial_velocity.is_valid()) {
		radial_velocity->get_property_list(r_props);
	}
	if (orbital_velocity.is_valid()) {
		orbital_velocity->get_property_list(r_props);
	}
}

bool godot::VelocityUpdater::_get(const StringName& p_property, Variant& r_value) const
{
	bool result = false;

	if (velocity_over_life.is_valid()) {
		result = result || velocity_over_life->get_property(p_property, r_value);
	}
	if (radial_velocity.is_valid()) {
		result = result || radial_velocity->get_property(p_property, r_value);
	}
	if (orbital_velocity.is_valid()) {
		result = result || orbital_velocity->get_property(p_property, r_value);
	}
	return result;
}

bool godot::VelocityUpdater::_set(const StringName& p_property, const Variant& p_value)
{
	bool result = false;

	if (velocity_over_life.is_valid()) {
		result = result || velocity_over_life->set_property(p_property, p_value, Ref<Resource>(this));
	}
	if (radial_velocity.is_valid()) {
		result = result || radial_velocity->set_property(p_property, p_value, Ref<Resource>(this));
	}
	if (orbital_velocity.is_valid()) {
		result = result || orbital_velocity->set_property(p_property, p_value, Ref<Resource>(this));
	}
	return result;
}

bool godot::VelocityUpdater::_property_can_revert(const StringName& p_name) const
{
	bool result = false;

	if (velocity_over_life.is_valid()) {
		result = result || velocity_over_life->can_property_revert(p_name);
	}
	if (radial_velocity.is_valid()) {
		result = result || radial_velocity->can_property_revert(p_name);
	}
	if (orbital_velocity.is_valid()) {
		result = result || orbital_velocity->can_property_revert(p_name);
	}
	return result;
}

bool godot::VelocityUpdater::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	bool result = false;

	if (velocity_over_life.is_valid()) {
		result = result || velocity_over_life->get_property_revert(p_name, r_property);
	}
	if (radial_velocity.is_valid()) {
		result = result || radial_velocity->get_property_revert(p_name, r_property);
	}
	if (orbital_velocity.is_valid()) {
		result = result || orbital_velocity->get_property_revert(p_name, r_property);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////

void godot::AccelerationUpdater::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_acceleration_property", "acceleration_property"), &AccelerationUpdater::set_acceleration_property);
	ClassDB::bind_method(D_METHOD("get_acceleration_property"), &AccelerationUpdater::get_acceleration_property);
}

godot::AccelerationUpdater::AccelerationUpdater()
{
	Ref<SpicyVector3Property> acc_prop(memnew(SpicyVector3Property));
	acceleration_property = acc_prop;
	acc_prop->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_UNIFORM | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
	acc_prop->set_property_name("acceleration");
	acc_prop->set_default_uniform(Vector3(0, 0, 0));
}

void godot::AccelerationUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;

	if(!acceleration_property.is_valid()) return;

	for (size_t i = 0; i < endId; ++i)
	{
		p_data->acceleration[i] += acceleration_property->get_value(p_data->rng, p_data->normalized_lifetime[i]) * dt;
	}
}

void godot::AccelerationUpdater::set_acceleration_property(const Ref<SpicyVector3Property>& p_acceleration_property)
{
	acceleration_property = p_acceleration_property;

	if (acceleration_property.is_null()) {
		Ref<SpicyVector3Property> acc_prop(memnew(SpicyVector3Property));
		acceleration_property = acc_prop;
		acc_prop->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_UNIFORM | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
		acc_prop->set_property_name("acceleration");
		acc_prop->set_default_uniform(Vector3(0, 0, 0));
	}
}

Ref<SpicyVector3Property> godot::AccelerationUpdater::get_acceleration_property() const
{
	return acceleration_property;
}

void godot::AccelerationUpdater::_get_property_list(List<PropertyInfo>* r_props) const
{
	if (acceleration_property.is_valid()) {
		acceleration_property->get_property_list(r_props);
	}
}

bool godot::AccelerationUpdater::_get(const StringName& p_property, Variant& r_value) const
{
	if (acceleration_property.is_valid()) {
		return acceleration_property->get_property(p_property, r_value);
	}
	return false;
}

bool godot::AccelerationUpdater::_set(const StringName& p_property, const Variant& p_value)
{
	if (acceleration_property.is_valid()) {
		return acceleration_property->set_property(p_property, p_value, Ref<Resource>(this));
	}
	return false;
}

bool godot::AccelerationUpdater::_property_can_revert(const StringName& p_name) const
{
	if (acceleration_property.is_valid()) {
		return acceleration_property->can_property_revert(p_name);
	}
	return false;
}

bool godot::AccelerationUpdater::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	if (acceleration_property.is_valid()) {
		return acceleration_property->get_property_revert(p_name, r_property);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void godot::ColorUpdater::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_color_over_lifetime", "color_over_lifetime"), &ColorUpdater::set_color_over_lifetime);
	ClassDB::bind_method(D_METHOD("get_color_over_lifetime"), &ColorUpdater::get_color_over_lifetime);
	ClassDB::bind_method(D_METHOD("set_color_over_velocity", "color_over_velocity"), &ColorUpdater::set_color_over_velocity);
	ClassDB::bind_method(D_METHOD("get_color_over_velocity"), &ColorUpdater::get_color_over_velocity);
	ClassDB::bind_method(D_METHOD("set_speed_range", "velocity_range"), &ColorUpdater::set_speed_range);
	ClassDB::bind_method(D_METHOD("get_speed_range"), &ColorUpdater::get_speed_range);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_over_lifetime", PROPERTY_HINT_RESOURCE_TYPE, "Gradient"), "set_color_over_lifetime", "get_color_over_lifetime");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "color_over_velocity", PROPERTY_HINT_RESOURCE_TYPE, "Gradient"), "set_color_over_velocity", "get_color_over_velocity");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity_range"), "set_speed_range", "get_speed_range");
}

void godot::ColorUpdater::_update_color(double dt, const Ref<ParticleData> p_data)
{
}

void godot::ColorUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;

	if (color_over_lifetime.is_valid() && color_over_velocity.is_valid())
	{
		for (size_t i = 0; i < endId; ++i)
		{
			float vel = p_data->acceleration[i].length() + (((p_data->velocity[i] + p_data->current_velocity[i]) * p_data->normalized_lifetime[i]).length());
			vel = Math::clamp(vel, speed_range.x, speed_range.y);
			vel = Math::remap(vel, speed_range.x, speed_range.y, 0.0f, 1.0f);
			p_data->current_color[i] = color_over_lifetime->sample(p_data->normalized_lifetime[i]) * color_over_velocity->sample(vel);
		}
		return;
	}

	if (color_over_lifetime.is_valid())
	{
		for (size_t i = 0; i < endId; ++i)
		{
			p_data->current_color[i] = color_over_lifetime->sample(p_data->normalized_lifetime[i]);
		}
		return;
	}

	if (color_over_velocity.is_valid())
	{
		for (size_t i = 0; i < endId; ++i)
		{
			float vel = p_data->acceleration[i].length() + ((p_data->velocity[i] * p_data->normalized_lifetime[i]).length());
			vel = Math::clamp(vel, speed_range.x, speed_range.y);
			vel = Math::remap(vel, speed_range.x, speed_range.y, 0.0f, 1.0f);
			p_data->current_color[i] = color_over_velocity->sample(vel);
		}
		return;
	}
}

void godot::ColorUpdater::set_color_over_lifetime(const Ref<Gradient>& p_color_over_lifetime)
{
	color_over_lifetime = p_color_over_lifetime;
}

Ref<Gradient> godot::ColorUpdater::get_color_over_lifetime() const
{
	return color_over_lifetime;
}

void godot::ColorUpdater::set_color_over_velocity(const Ref<Gradient>& p_color_over_velocity)
{
	color_over_velocity = p_color_over_velocity;
}

Ref<Gradient> godot::ColorUpdater::get_color_over_velocity() const
{
	return color_over_velocity;
}

void godot::ColorUpdater::set_speed_range(const Vector2& p_velocity_range)
{
	speed_range = p_velocity_range;
}

Vector2 godot::ColorUpdater::get_speed_range() const
{
	return speed_range;
}

//////////////////////////////////////////////////////////////////////////

void godot::RotationUpdater::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_rotation_property", "rotation_property"), &RotationUpdater::set_rotation_property);
	ClassDB::bind_method(D_METHOD("get_rotation_property"), &RotationUpdater::get_rotation_property);
}

godot::RotationUpdater::RotationUpdater()
{
	Ref<SpicyVector3Property> rot_prop(memnew(SpicyVector3Property));
	rotation_property = rot_prop;
	rot_prop->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_UNIFORM | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
	rot_prop->set_property_name("rotation");
	rot_prop->set_default_uniform(Vector3(0, 0, 0));
}

void godot::RotationUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;

	if(!rotation_property.is_valid()) return;

	for (size_t i = 0; i < endId; ++i)
	{
		p_data->current_rotation[i] += rotation_property->get_value(p_data->rng, p_data->normalized_lifetime[i]) * dt;
	}
}

void godot::RotationUpdater::set_rotation_property(const Ref<SpicyVector3Property>& p_rotation_property)
{
	rotation_property = p_rotation_property;

	if (rotation_property.is_null()) {
		Ref<SpicyVector3Property> rot_prop(memnew(SpicyVector3Property));
		rotation_property = rot_prop;
		rot_prop->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_UNIFORM | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
		rot_prop->set_property_name("rotation");
		rot_prop->set_default_uniform(Vector3(0, 0, 0));
	}
}

Ref<SpicyVector3Property> godot::RotationUpdater::get_rotation_property() const
{
	return rotation_property;
}

void godot::RotationUpdater::_get_property_list(List<PropertyInfo>* r_props) const
{
	if (rotation_property.is_valid()) {
		rotation_property->get_property_list(r_props);
	}
}

bool godot::RotationUpdater::_get(const StringName& p_property, Variant& r_value) const
{
	if (rotation_property.is_valid()) {
		return rotation_property->get_property(p_property, r_value);
	}
	return false;
}

bool godot::RotationUpdater::_set(const StringName& p_property, const Variant& p_value)
{
	if (rotation_property.is_valid()) {
		return rotation_property->set_property(p_property, p_value, Ref<Resource>(this));
	}
	return false;
}

bool godot::RotationUpdater::_property_can_revert(const StringName& p_name) const
{
	if (rotation_property.is_valid()) {
		return rotation_property->can_property_revert(p_name);
	}
	return false;
}

bool godot::RotationUpdater::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	if (rotation_property.is_valid()) {
		return rotation_property->get_property_revert(p_name, r_property);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void godot::SizeUpdater::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_size_property", "size_property"), &SizeUpdater::set_size_property);
	ClassDB::bind_method(D_METHOD("get_size_property"), &SizeUpdater::get_size_property);
}

godot::SizeUpdater::SizeUpdater()
{
	Ref<SpicyVector3Property> size_prop(memnew(SpicyVector3Property));
	size_property = size_prop;
	size_prop->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
	size_prop->set_property_name("size");
	size_prop->set_default_uniform(Vector3(1, 1, 1));
	size_prop->set_default_type(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE);
}

void godot::SizeUpdater::update(double dt, const Ref<ParticleData> p_data)
{
	unsigned int endId = p_data->count_alive;

	if(!size_property.is_valid()) return;

	for (size_t i = 0; i < endId; ++i)
	{
		p_data->current_scale[i] = size_property->get_value(p_data->rng, p_data->normalized_lifetime[i]);
	}
}

void godot::SizeUpdater::set_size_property(const Ref<SpicyVector3Property>& p_size_property)
{
	size_property = p_size_property;
	
	if (size_property.is_null()) {
		Ref<SpicyVector3Property> size_prop(memnew(SpicyVector3Property));
		size_property = size_prop;
		size_prop->set_available_types(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE | SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE_XYZ);
		size_prop->set_property_name("size");
		size_prop->set_default_uniform(Vector3(1, 1, 1));
		size_prop->set_default_type(SpicyVector3Property::SpicyVector3Type::SPICY_VECTOR_TYPE_CURVE);
	}
}

Ref<SpicyVector3Property> godot::SizeUpdater::get_size_property() const
{
	return size_property;
}

void godot::SizeUpdater::_get_property_list(List<PropertyInfo>* r_props) const
{
	if (size_property.is_valid()) {
		size_property->get_property_list(r_props);
	}
}

bool godot::SizeUpdater::_get(const StringName& p_property, Variant& r_value) const
{
	if (size_property.is_valid()) {
		return size_property->get_property(p_property, r_value);
	}
	return false;
}

bool godot::SizeUpdater::_set(const StringName& p_property, const Variant& p_value)
{
	if (size_property.is_valid()) {
		return size_property->set_property(p_property, p_value, Ref<Resource>(this));
	}
	return false;
}

bool godot::SizeUpdater::_property_can_revert(const StringName& p_name) const
{
	if (size_property.is_valid()) {
		return size_property->can_property_revert(p_name);
	}
	return false;
}

bool godot::SizeUpdater::_property_get_revert(const StringName& p_name, Variant& r_property) const
{
	if (size_property.is_valid()) {
		return size_property->get_property_revert(p_name, r_property);
	}
	return false;
}