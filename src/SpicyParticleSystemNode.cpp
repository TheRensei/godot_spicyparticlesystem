#include "SpicyParticleSystemNode.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

#include <godot_cpp/classes/rendering_server.hpp>

using namespace godot;

void godot::SpicyParticleSystemNode::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("initialize", "max_count"), &SpicyParticleSystemNode::initialize);
	ClassDB::bind_method(D_METHOD("emit_burst", "count"), &SpicyParticleSystemNode::emit_burst);
	ClassDB::bind_method(D_METHOD("pause", "include_children"), &SpicyParticleSystemNode::pause, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("play", "include_children"), &SpicyParticleSystemNode::play, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("stop", "include_children"), &SpicyParticleSystemNode::stop, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("restart", "include_children"), &SpicyParticleSystemNode::restart, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("is_playing"), &SpicyParticleSystemNode::get_is_playing);

	ClassDB::bind_method(D_METHOD("set_emit_rate", "p_emit_rate"), &SpicyParticleSystemNode::set_emit_rate);
	ClassDB::bind_method(D_METHOD("get_emit_rate"), &SpicyParticleSystemNode::get_emit_rate);
	ClassDB::bind_method(D_METHOD("set_emit_rate_over_distance", "p_emit_rate_over_distance"), &SpicyParticleSystemNode::set_emit_rate_over_distance);
	ClassDB::bind_method(D_METHOD("get_emit_rate_over_distance"), &SpicyParticleSystemNode::get_emit_rate_over_distance);
	ClassDB::bind_method(D_METHOD("set_emit_bursts", "p_emit_bursts"), &SpicyParticleSystemNode::set_emit_bursts);
	ClassDB::bind_method(D_METHOD("get_emit_bursts"), &SpicyParticleSystemNode::get_emit_bursts);
	ClassDB::bind_method(D_METHOD("set_simulation_time", "p_simulation_time"), &SpicyParticleSystemNode::set_simulation_time);
	ClassDB::bind_method(D_METHOD("get_simulation_time"), &SpicyParticleSystemNode::get_simulation_time);
	ClassDB::bind_method(D_METHOD("set_simulation_speed", "p_simulation_speed"), &SpicyParticleSystemNode::set_simulation_speed);
	ClassDB::bind_method(D_METHOD("get_simulation_speed"), &SpicyParticleSystemNode::get_simulation_speed);
	ClassDB::bind_method(D_METHOD("get_particle_count"), &SpicyParticleSystemNode::get_particle_count);
	ClassDB::bind_method(D_METHOD("set_seed", "p_seed"), &SpicyParticleSystemNode::set_seed);
	ClassDB::bind_method(D_METHOD("get_seed"), &SpicyParticleSystemNode::get_seed);
	ClassDB::bind_method(D_METHOD("set_duration", "p_duration"), &SpicyParticleSystemNode::set_duration);
	ClassDB::bind_method(D_METHOD("get_duration"), &SpicyParticleSystemNode::get_duration);
	ClassDB::bind_method(D_METHOD("set_delay", "p_delay"), &SpicyParticleSystemNode::set_delay);
	ClassDB::bind_method(D_METHOD("get_delay"), &SpicyParticleSystemNode::get_delay);
	ClassDB::bind_method(D_METHOD("set_looping", "p_looping"), &SpicyParticleSystemNode::set_looping);
	ClassDB::bind_method(D_METHOD("get_looping"), &SpicyParticleSystemNode::get_looping);
	ClassDB::bind_method(D_METHOD("set_world_space", "p_world_space"), &SpicyParticleSystemNode::set_world_space);
	ClassDB::bind_method(D_METHOD("get_world_space"), &SpicyParticleSystemNode::get_world_space);
	ClassDB::bind_method(D_METHOD("set_play_on_start", "p_play_on_start"), &SpicyParticleSystemNode::set_play_on_start);
	ClassDB::bind_method(D_METHOD("get_play_on_start"), &SpicyParticleSystemNode::get_play_on_start);
	ClassDB::bind_method(D_METHOD("set_max_particle_count", "p_max_particle_count"), &SpicyParticleSystemNode::set_max_particle_count);
	ClassDB::bind_method(D_METHOD("get_max_particle_count"), &SpicyParticleSystemNode::get_max_particle_count);
	ClassDB::bind_method(D_METHOD("set_randomize_seed", "p_randomize_seed"), &SpicyParticleSystemNode::set_randomize_seed);
	ClassDB::bind_method(D_METHOD("get_randomize_seed"), &SpicyParticleSystemNode::get_randomize_seed);

	ClassDB::bind_method(D_METHOD("set_lifetime_generator", "p_lifetime_generator"), &SpicyParticleSystemNode::set_lifetime_generator);
	ClassDB::bind_method(D_METHOD("get_lifetime_generator"), &SpicyParticleSystemNode::get_lifetime_generator);
	ClassDB::bind_method(D_METHOD("set_position_generator", "p_position_generator"), &SpicyParticleSystemNode::set_position_generator);
	ClassDB::bind_method(D_METHOD("get_position_generator"), &SpicyParticleSystemNode::get_position_generator);
	ClassDB::bind_method(D_METHOD("set_color_generator", "p_color_generator"), &SpicyParticleSystemNode::set_color_generator);
	ClassDB::bind_method(D_METHOD("get_color_generator"), &SpicyParticleSystemNode::get_color_generator);
	ClassDB::bind_method(D_METHOD("set_size_generator", "p_size_generator"), &SpicyParticleSystemNode::set_size_generator);
	ClassDB::bind_method(D_METHOD("get_size_generator"), &SpicyParticleSystemNode::get_size_generator);
	ClassDB::bind_method(D_METHOD("set_velocity_generator", "p_velocity_generator"), &SpicyParticleSystemNode::set_velocity_generator);
	ClassDB::bind_method(D_METHOD("get_velocity_generator"), &SpicyParticleSystemNode::get_velocity_generator);
	ClassDB::bind_method(D_METHOD("set_rotation_generator", "p_rotation_generator"), &SpicyParticleSystemNode::set_rotation_generator);
	ClassDB::bind_method(D_METHOD("get_rotation_generator"), &SpicyParticleSystemNode::get_rotation_generator);

	ClassDB::bind_method(D_METHOD("set_color_updater", "p_color_updater"), &SpicyParticleSystemNode::set_color_updater);
	ClassDB::bind_method(D_METHOD("get_color_updater"), &SpicyParticleSystemNode::get_color_updater);
	ClassDB::bind_method(D_METHOD("set_velocity_updater", "p_velocity_updater"), &SpicyParticleSystemNode::set_velocity_updater);
	ClassDB::bind_method(D_METHOD("get_velocity_updater"), &SpicyParticleSystemNode::get_velocity_updater);
	ClassDB::bind_method(D_METHOD("set_acceleration_updater", "p_acceleration_updater"), &SpicyParticleSystemNode::set_acceleration_updater);
	ClassDB::bind_method(D_METHOD("get_acceleration_updater"), &SpicyParticleSystemNode::get_acceleration_updater);
	ClassDB::bind_method(D_METHOD("set_rotation_updater", "p_rotation_updater"), &SpicyParticleSystemNode::set_rotation_updater);
	ClassDB::bind_method(D_METHOD("get_rotation_updater"), &SpicyParticleSystemNode::get_rotation_updater);
	ClassDB::bind_method(D_METHOD("set_size_updater", "p_size_updater"), &SpicyParticleSystemNode::set_size_updater);
	ClassDB::bind_method(D_METHOD("get_size_updater"), &SpicyParticleSystemNode::get_size_updater);

	ClassDB::bind_method(D_METHOD("set_mesh", "p_mesh"), &SpicyParticleSystemNode::set_mesh);
	ClassDB::bind_method(D_METHOD("get_mesh"), &SpicyParticleSystemNode::get_mesh);
	ClassDB::bind_method(D_METHOD("set_particle_alignment", "p_particle_alignment"), &SpicyParticleSystemNode::set_particle_alignment);
	ClassDB::bind_method(D_METHOD("get_particle_alignment"), &SpicyParticleSystemNode::get_particle_alignment);
	ClassDB::bind_method(D_METHOD("set_alignment_target", "p_alignment_target"), &SpicyParticleSystemNode::set_alignment_target);
	ClassDB::bind_method(D_METHOD("get_alignment_target"), &SpicyParticleSystemNode::get_alignment_target);
	//ClassDB::bind_method(D_METHOD("set_custom_renderer_data", "p_custom_renderer_data"), &SpicyParticleSystemNode::set_custom_renderer_data);
	//ClassDB::bind_method(D_METHOD("get_custom_renderer_data"), &SpicyParticleSystemNode::get_custom_renderer_data);

	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::BOOL, "looping"), "set_looping", "get_looping");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::FLOAT, "duration", PROPERTY_HINT_RANGE, "0.01,1000000.0,0.01,or_greater,exp,suffix:s"), "set_duration", "get_duration");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::FLOAT, "delay", PROPERTY_HINT_RANGE, "0,1000000.0,0.01,or_greater,exp,suffix:s"), "set_delay", "get_delay");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::FLOAT, "simulation_speed", PROPERTY_HINT_RANGE, "0.0,1000000.0,0.01,exp"), "set_simulation_speed", "get_simulation_speed");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::BOOL, "world_space"), "set_world_space", "get_world_space");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::BOOL, "play_on_start"), "set_play_on_start", "get_play_on_start");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::BOOL, "randomize_seed"), "set_randomize_seed", "get_randomize_seed");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::INT, "max_particles", PROPERTY_HINT_RANGE, "2,1000000,1,exp"), "set_max_particle_count", "get_max_particle_count");
	ClassDB::add_property_group("SpicyParticleSystemNode", "Emitter", "");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::FLOAT, "emit_rate", PROPERTY_HINT_RANGE, "0.0,1000000.0,0.01,exp"), "set_emit_rate", "get_emit_rate");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::FLOAT, "emit_rate_over_distance", PROPERTY_HINT_RANGE, "0.0,1000000.0,0.01,exp"), "set_emit_rate_over_distance", "get_emit_rate_over_distance");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "emit_bursts", PROPERTY_HINT_ARRAY_TYPE, MAKE_RESOURCE_TYPE_HINT(SpicyParticleBurst::get_class_static()), PROPERTY_USAGE_DEFAULT), "set_emit_bursts", "get_emit_bursts"); //| PROPERTY_USAGE_NEVER_DUPLICATE

	ClassDB::add_property_group("SpicyParticleSystemNode", "Generators", "");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "lifetime_generator", PROPERTY_HINT_RESOURCE_TYPE, "LifetimeGenerator"), "set_lifetime_generator", "get_lifetime_generator");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "position_generator", PROPERTY_HINT_RESOURCE_TYPE, "PositionGenerator"), "set_position_generator", "get_position_generator");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "color_generator", PROPERTY_HINT_RESOURCE_TYPE, "ColorGenerator"), "set_color_generator", "get_color_generator");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "size_generator", PROPERTY_HINT_RESOURCE_TYPE, "SizeGenerator"), "set_size_generator", "get_size_generator");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "velocity_generator", PROPERTY_HINT_RESOURCE_TYPE, "VelocityGenerator"), "set_velocity_generator", "get_velocity_generator");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "rotation_generator", PROPERTY_HINT_RESOURCE_TYPE, "RotationGenerator"), "set_rotation_generator", "get_rotation_generator");
	ClassDB::add_property_group("SpicyParticleSystemNode", "Updaters", "");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "color_updater", PROPERTY_HINT_RESOURCE_TYPE, "ColorUpdater"), "set_color_updater", "get_color_updater");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "size_updater", PROPERTY_HINT_RESOURCE_TYPE, "SizeUpdater"), "set_size_updater", "get_size_updater");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "velocity_updater", PROPERTY_HINT_RESOURCE_TYPE, "VelocityUpdater"), "set_velocity_updater", "get_velocity_updater");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "rotation_updater", PROPERTY_HINT_RESOURCE_TYPE, "RotationUpdater"), "set_rotation_updater", "get_rotation_updater");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "acceleration_updater", PROPERTY_HINT_RESOURCE_TYPE, "AccelerationUpdater"), "set_acceleration_updater", "get_acceleration_updater");

	ClassDB::add_property_group("SpicyParticleSystemNode", "Renderer", "");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::INT, "particle_alignment", PROPERTY_HINT_ENUM, "Local,World,Screen,Camera,Velocity,Look At"), "set_particle_alignment", "get_particle_alignment");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::NODE_PATH, "alignment_target", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "Node3D"), "set_alignment_target", "get_alignment_target");
	//ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::BOOL, "custom_renderer_data"), "set_custom_renderer_data", "get_custom_renderer_data");

	ClassDB::add_property_group("SpicyParticleSystemNode", "Debug", "");
	ClassDB::add_property("SpicyParticleSystemNode", PropertyInfo(Variant::FLOAT, "simulation_time"), "set_simulation_time", "get_simulation_time");

	ADD_SIGNAL(MethodInfo("play"));
	ADD_SIGNAL(MethodInfo("stop"));
}

godot::SpicyParticleSystemNode::SpicyParticleSystemNode() : max_particles(100),
initialized(false),
is_paused(false),
is_playing(false),
is_stopped(true),
looping(true),
randomize_seed(true),
seed(0),
world_space(false),
play_on_start(true),
emit_rate(8),
emit_rate_over_distance(0),
duration(1.0),
delay(0.0),
simulation_speed(1),
simulation_time(0.0),
normalized_duration_time(0.0),
m_particle_alignment(MultiMeshParticleRenderer::ALIGNMENT_LOCAL)
{
	if (initialized)
		return;
	initialize(max_particles);
}

godot::SpicyParticleSystemNode::~SpicyParticleSystemNode()
{
}

double frame_remainder = 0;

void godot::SpicyParticleSystemNode::_internal_process(double delta)
{
	if (is_paused && !is_playing || is_stopped || !is_visible_in_tree() || !is_inside_tree())
	{
		frame_remainder = 0;
		internal_process_time = 0;
		return;
	}

	if (is_playing)
	{
		if (simulation_time > duration && !looping)
		{
			m_particle_system->set_emitting(false);
			if (m_particle_system->num_alive_particles() == 0)
			{
				stop(false);
				return;
			}
		}
	}

	if (initialized)
	{
		normalized_duration_time = std::fmod(simulation_time, duration) / duration;

		double local_delta = delta * simulation_speed;
		simulation_time += local_delta;
		if (simulation_time <= delay)
			return;

		m_particle_system->update(local_delta, normalized_duration_time, node_transform);
		m_renderer->update();
		m_renderer->render();
	}
}

void godot::SpicyParticleSystemNode::_validate_property(PropertyInfo& p_property) const
{
	//Main
	if (p_property.name == (String)"seed" && randomize_seed) {
		p_property.usage = PROPERTY_USAGE_NONE;
	}

	if (p_property.name == (String)"alignment_target" && m_particle_alignment != MultiMeshParticleRenderer::ALIGNMENT_LOOK_AT)
	{
		p_property.usage = PROPERTY_USAGE_NONE;
	}
}

void godot::SpicyParticleSystemNode::_update_null_properties()
{
	if (m_lifetime_generator.is_null()) {
		Ref<LifetimeGenerator> lifetime_generator(memnew(LifetimeGenerator));
		set_lifetime_generator(lifetime_generator);
	}

	if (m_position_generator.is_null()) {
		Ref<PointEmissionShape> shape(memnew(PointEmissionShape));
		Ref<PositionGenerator> position_generator(memnew(PositionGenerator));
		position_generator->set_emission_shape(shape);
		set_position_generator(position_generator);
	}
}

void godot::SpicyParticleSystemNode::_notification(int p_what)
{
	switch (p_what) {

	case NOTIFICATION_ENTER_TREE: {
		_update_null_properties();
	} break;

	case NOTIFICATION_INTERNAL_PROCESS: {
		_internal_process(get_process_delta_time());
	} break;

	case NOTIFICATION_READY: {
		if (!Engine::get_singleton()->is_editor_hint() && initialized && play_on_start) {
			play();
			set_particle_alignment(m_particle_alignment);
		}
	} break;

	case NOTIFICATION_TRANSFORM_CHANGED:
	{
		if (world_space && is_inside_tree())
			node_transform = get_global_transform();
		else
			node_transform = Transform3D();

	} break;

	case NOTIFICATION_EXIT_TREE: {
		_stop_no_signal();
	} break;
	}

}

void godot::SpicyParticleSystemNode::_stop_no_signal()
{
	set_process_internal(false);
	is_playing = false;
	is_paused = false;
	is_stopped = true;


	m_particle_system->reset();
	m_particle_system->set_emitting(false);
	m_renderer->reset();

	rng->set_state(rng_state);
	simulation_time = 0.0f;
	internal_process_time = 0.0f;
	normalized_duration_time = 0.0f;
}

PackedStringArray godot::SpicyParticleSystemNode::_get_configuration_warnings() const
{
	PackedStringArray warnings = GeometryInstance3D::_get_configuration_warnings();

	bool mesh_found = false;

	if (get_mesh().is_valid()) {
		mesh_found = true;
	}

	if (!mesh_found) {
		warnings.push_back("Nothing is visible because no mesh has been assigned.");
	}


	//bool mesh_found = false;
	//bool anim_material_found = false;

	//if (get_mesh().is_valid()) {
	//	mesh_found = true;
	//	for (int j = 0; j < get_mesh()->get_surface_count(); j++) {
	//		anim_material_found = Object::cast_to<ShaderMaterial>(get_mesh()->surface_get_material(j).ptr()) != nullptr;
	//		StandardMaterial3D* spat = Object::cast_to<StandardMaterial3D>(get_mesh()->surface_get_material(j).ptr());
	//		anim_material_found = anim_material_found || (spat && spat->get_billboard_mode() == StandardMaterial3D::BILLBOARD_PARTICLES);
	//	}
	//}

	//anim_material_found = anim_material_found || Object::cast_to<ShaderMaterial>(get_material_override().ptr()) != nullptr;
	//StandardMaterial3D* spat = Object::cast_to<StandardMaterial3D>(get_material_override().ptr());
	//anim_material_found = anim_material_found || (spat && spat->get_billboard_mode() == StandardMaterial3D::BILLBOARD_PARTICLES);

	//if (!mesh_found) {
	//	warnings.push_back("Nothing is visible because no mesh has been assigned.");
	//}

	//if (!anim_material_found && m_custom_renderer_data) {
	//	warnings.push_back("CPUParticles3D animation requires the usage of a StandardMaterial3D whose Billboard Mode is set to \"Particle Billboard\".");
	//}

	return warnings;
}

void godot::SpicyParticleSystemNode::initialize(size_t max_count)
{
	if (initialized)
		return;

	rng.instantiate();
	if (randomize_seed)
		seed = Time::get_singleton()->get_ticks_msec() + get_name().to_int();
	set_seed(seed);

	Ref<SpicyParticleSystem> particle_system(memnew(SpicyParticleSystem));
	m_particle_system = particle_system;
	m_particle_system->initialize(max_count, rng, *this);

	Ref<MultiMeshParticleRenderer> renderer(memnew(MultiMeshParticleRenderer));
	m_renderer = renderer;
	m_renderer->generate(m_particle_system, mesh);

	set_base(m_renderer->get_multimesh());

	Ref<SpicyParticleEmitter> emitter(memnew(SpicyParticleEmitter));
	m_emitter = emitter;
	m_emitter->set_emit_rate(emit_rate);

	m_particle_system->add_emitter(m_emitter);

	Ref<SpicyParticleGenerator> default_generator(memnew(SpicyParticleGenerator));
	m_emitter->add_generator(default_generator);

	//Add base updaters
	Ref<LifetimeUpdater> lifetime_updater(memnew(LifetimeUpdater));
	m_particle_system->add_updater(lifetime_updater);

	Ref<PositionUpdater> position_updater(memnew(PositionUpdater));
	m_particle_system->add_updater(position_updater);

	m_particle_system->final_data()->max_duration = duration;

	//m_particle_system->update(0.01667);
	//m_renderer->update();
	//m_renderer->render();

	initialized = true;
}

void godot::SpicyParticleSystemNode::set_mesh(const Ref<Mesh>& p_mesh)
{
	mesh = p_mesh;

	if (mesh.is_valid() && initialized) {
		m_renderer->set_mesh(mesh);
	}

	update_configuration_warnings();
}

Ref<Mesh> godot::SpicyParticleSystemNode::get_mesh() const
{
	return mesh;
}

void godot::SpicyParticleSystemNode::set_particle_alignment(MultiMeshParticleRenderer::Alignment p_particle_alignment)
{
	m_particle_alignment = p_particle_alignment;

	if (m_renderer.is_valid())
	{
		m_renderer->set_alignment(m_particle_alignment);

		if (m_particle_alignment == MultiMeshParticleRenderer::ALIGNMENT_CAMERA || m_particle_alignment == MultiMeshParticleRenderer::ALIGNMENT_SCREEN)
		{
			Camera3D* camera = nullptr;
			if (Engine::get_singleton()->is_editor_hint()) {
				camera = EditorInterface::get_singleton()->get_editor_viewport_3d()->get_camera_3d();
			}
			else {
				if (get_viewport() != nullptr)
				{
					camera = get_viewport()->get_camera_3d();
				}
			}


			m_renderer->set_alignment_target(camera);
		}
		else
		{
			set_alignment_target(m_alignment_target);
		}
	}

	notify_property_list_changed();
}

MultiMeshParticleRenderer::Alignment godot::SpicyParticleSystemNode::get_particle_alignment() const
{
	return m_particle_alignment;
}

void godot::SpicyParticleSystemNode::set_alignment_target(const NodePath& p_path)
{
	m_alignment_target = p_path;

	if (m_renderer.is_valid()) {
		m_renderer->set_alignment_target(NULL);
		if (!p_path.is_empty())
			m_renderer->set_alignment_target(get_node<Node3D>(m_alignment_target));
	}
}

NodePath godot::SpicyParticleSystemNode::get_alignment_target() const
{
	return m_alignment_target;
}

void godot::SpicyParticleSystemNode::emit_burst(int count)
{
	if (initialized)
		m_particle_system->emit(0.0f, count);
}

void godot::SpicyParticleSystemNode::set_emit_rate(float p_emit_rate)
{
	emit_rate = p_emit_rate;

	if (initialized)
		m_emitter->set_emit_rate(emit_rate);
}

float godot::SpicyParticleSystemNode::get_emit_rate() const
{
	return emit_rate;
}

void godot::SpicyParticleSystemNode::set_emit_rate_over_distance(float p_emit_rate_over_distance)
{
	emit_rate_over_distance = p_emit_rate_over_distance;

	if (initialized)
		m_emitter->set_emit_rate_over_distance(emit_rate_over_distance);
}


float godot::SpicyParticleSystemNode::get_emit_rate_over_distance() const
{
	return emit_rate_over_distance;
}

void godot::SpicyParticleSystemNode::set_duration(float p_duration)
{
	duration = p_duration;
	m_particle_system->final_data()->max_duration = duration;

	_update_burst_times();
}

float godot::SpicyParticleSystemNode::get_duration() const
{
	return duration;
}

void godot::SpicyParticleSystemNode::set_emit_bursts(const TypedArray<SpicyParticleBurst>& p_emit_bursts)
{
	m_emit_bursts = p_emit_bursts;

	for (int i = 0; i < m_emit_bursts.size(); ++i)
	{
		Ref<SpicyParticleBurst> burst = m_emit_bursts[i];
		if (!burst.is_valid())
		{
			burst = Ref<SpicyParticleBurst>(memnew(SpicyParticleBurst));
			m_emit_bursts[i] = burst;
		}
	}

	if (m_emitter.is_valid())
		m_emitter->set_bursts(m_emit_bursts);
}

TypedArray<SpicyParticleBurst> godot::SpicyParticleSystemNode::get_emit_bursts() const
{
	return m_emit_bursts;
}

void godot::SpicyParticleSystemNode::_update_burst_times()
{
	for (int i = 0; i < m_emit_bursts.size(); ++i)
	{
		Ref<SpicyParticleBurst> burst = m_emit_bursts[i];
		if (burst.is_valid())
			burst->time = CLAMP(burst->time, 0, duration);
	}
}

void godot::SpicyParticleSystemNode::set_delay(float p_preprocess_time)
{
	delay = p_preprocess_time;
}

float godot::SpicyParticleSystemNode::get_delay() const
{
	return delay;
}

void godot::SpicyParticleSystemNode::set_looping(bool p_looping)
{
	looping = p_looping;
}

bool godot::SpicyParticleSystemNode::get_looping() const
{
	return looping;
}

void godot::SpicyParticleSystemNode::set_world_space(bool p_world_space)
{
	world_space = p_world_space;
}

bool godot::SpicyParticleSystemNode::get_world_space() const
{
	return world_space;
}

void godot::SpicyParticleSystemNode::set_play_on_start(bool p_play_on_start)
{
	play_on_start = p_play_on_start;
}

bool godot::SpicyParticleSystemNode::get_play_on_start() const
{
	return play_on_start;
}

void godot::SpicyParticleSystemNode::set_randomize_seed(bool p_randomize_seed)
{
	randomize_seed = p_randomize_seed;
	notify_property_list_changed();
}

bool godot::SpicyParticleSystemNode::get_randomize_seed() const
{
	return randomize_seed;
}

void godot::SpicyParticleSystemNode::seek(double sim_time)
{
	if (!initialized || !is_visible_in_tree() || !is_inside_tree())
		return;

	_stop_no_signal();
	pause();
	m_particle_system->set_emitting(true);


	simulation_time = sim_time;

	//simulate until that point with 60 fps delta
	//could probably be optimized
	float temp = 0;
	while (temp < simulation_time)
	{
		normalized_duration_time = std::fmod(temp, duration) / duration;
		m_particle_system->update(simulation_delta, normalized_duration_time, node_transform);
		temp += simulation_delta;

		if (temp >= duration && !looping)
		{
			m_particle_system->set_emitting(false);
			if (m_particle_system->num_alive_particles() == 0)
			{
				_stop_no_signal();
			}
		}
	}

	m_renderer->update();
	m_renderer->render();

}

void godot::SpicyParticleSystemNode::step(double delta)
{
	if (!initialized || !is_visible_in_tree() || !is_inside_tree())
		return;

	pause();
	m_particle_system->set_emitting(true);

	normalized_duration_time = std::fmod(delta, duration) / duration;
	m_particle_system->update(delta, normalized_duration_time, node_transform);
	simulation_time += delta;

	if (simulation_time >= duration && !looping)
	{
		m_particle_system->set_emitting(false);
		if (m_particle_system->num_alive_particles() == 0)
		{
			_stop_no_signal();
		}
	}

	m_renderer->update();
	m_renderer->render();

}

void godot::SpicyParticleSystemNode::pause(bool include_children)
{
	if (include_children) {
		SpicyParticleSystemNode* parent = Object::cast_to<SpicyParticleSystemNode>(get_parent());
		if (parent != nullptr)
		{
			parent->pause();
			return;
		}

		TypedArray<SpicyParticleSystemNode> children = find_children("*", SpicyParticleSystemNode::get_class_static(), true, false);
		for (int i = 0; i < children.size(); ++i)
		{
			SpicyParticleSystemNode* child = cast_to<SpicyParticleSystemNode>(children[i]);
			child->pause(false);
		}
	}

	is_playing = false;
	is_paused = true;
	is_stopped = false;
}

void godot::SpicyParticleSystemNode::play(bool include_children)
{
	if (is_playing)
		return;

	if (include_children) {
		SpicyParticleSystemNode* parent = Object::cast_to<SpicyParticleSystemNode>(get_parent());
		if (parent != nullptr)
		{
			parent->play();
			return;
		}

		TypedArray<SpicyParticleSystemNode> children = find_children("*", SpicyParticleSystemNode::get_class_static(), true, false);
		for (int i = 0; i < children.size(); ++i)
		{
			SpicyParticleSystemNode* child = cast_to<SpicyParticleSystemNode>(children[i]);
			child->play(false);
		}
	}

	emit_signal("play");

	set_process_internal(true);

	if (!is_paused) {
		if (randomize_seed)
			set_seed(Time::get_singleton()->get_ticks_msec() + get_name().to_int());

		rng->set_state(rng_state);
	}

	is_playing = true;
	is_paused = false;
	is_stopped = false;

	m_particle_system->set_emitting(true);
}

void godot::SpicyParticleSystemNode::stop(bool include_children)
{
	//if(is_stopped)
	//	return;

	if (include_children) {
		SpicyParticleSystemNode* parent = Object::cast_to<SpicyParticleSystemNode>(get_parent());
		if (parent != nullptr)
		{
			parent->stop();
			return;
		}

		TypedArray<SpicyParticleSystemNode> children = find_children("*", SpicyParticleSystemNode::get_class_static(), true, false);
		for (int i = 0; i < children.size(); ++i)
		{
			SpicyParticleSystemNode* child = cast_to<SpicyParticleSystemNode>(children[i]);
			child->stop(false);
		}
	}

	emit_signal("stop");

	_stop_no_signal();
}

void godot::SpicyParticleSystemNode::restart(bool include_children)
{
	if (include_children) {
		SpicyParticleSystemNode* parent = Object::cast_to<SpicyParticleSystemNode>(get_parent());
		if (parent != nullptr)
		{
			parent->restart();
			return;
		}

		TypedArray<SpicyParticleSystemNode> children = find_children("*", SpicyParticleSystemNode::get_class_static(), true, false);
		for (int i = 0; i < children.size(); ++i)
		{
			SpicyParticleSystemNode* child = cast_to<SpicyParticleSystemNode>(children[i]);
			child->restart(false);
		}
	}

	stop(include_children);
	play(include_children);
}

bool godot::SpicyParticleSystemNode::get_is_playing()
{
	return is_playing;
}

void godot::SpicyParticleSystemNode::set_simulation_time(double p_simulation_time)
{
	seek(p_simulation_time);

	//double d = p_simulation_time - simulation_time;
	//if (d > 0)
	//{
	//	step(d);
	//}
	//else
	//{
	//	seek(p_simulation_time);
	//}

}

double godot::SpicyParticleSystemNode::get_simulation_time() const
{
	return simulation_time;
}

void godot::SpicyParticleSystemNode::set_seed(unsigned int p_seed)
{
	seed = p_seed;
	rng->set_seed(seed);
	rng_state = rng->get_state();
}

unsigned int godot::SpicyParticleSystemNode::get_seed() const
{
	return seed;
}

void godot::SpicyParticleSystemNode::set_lifetime_generator(const Ref<LifetimeGenerator>& p_lifetime_generator)
{
	m_emitter->remove_generator(m_lifetime_generator);
	m_lifetime_generator = p_lifetime_generator;

	if (m_lifetime_generator.is_valid())
		m_emitter->add_generator(m_lifetime_generator);

	_update_null_properties();
}

Ref<LifetimeGenerator> godot::SpicyParticleSystemNode::get_lifetime_generator() const
{
	return m_lifetime_generator;
}

void godot::SpicyParticleSystemNode::set_position_generator(const Ref<PositionGenerator>& p_position_generator)
{
	m_emitter->remove_generator(m_position_generator);
	m_position_generator = p_position_generator;

	if (m_position_generator.is_valid())
		m_emitter->add_generator(m_position_generator);

	_update_null_properties();
}

Ref<PositionGenerator> godot::SpicyParticleSystemNode::get_position_generator() const
{
	return m_position_generator;
}

void godot::SpicyParticleSystemNode::set_color_generator(const Ref<ColorGenerator>& p_color_generator)
{
	m_emitter->remove_generator(m_color_generator);
	m_color_generator = p_color_generator;

	if (m_color_generator.is_valid())
		m_emitter->add_generator(m_color_generator);
}

Ref<ColorGenerator> godot::SpicyParticleSystemNode::get_color_generator() const
{
	return m_color_generator;
}
void godot::SpicyParticleSystemNode::set_size_generator(const Ref<SizeGenerator>& p_size_generator)
{
	m_emitter->remove_generator(m_size_generator);
	m_size_generator = p_size_generator;

	if (m_size_generator.is_valid())
		m_emitter->add_generator(m_size_generator);
}
Ref<SizeGenerator> godot::SpicyParticleSystemNode::get_size_generator() const
{
	return m_size_generator;
}

void godot::SpicyParticleSystemNode::set_simulation_speed(double p_simulation_speed)
{
	simulation_speed = p_simulation_speed;
}

double godot::SpicyParticleSystemNode::get_simulation_speed() const
{
	return simulation_speed;
}

void godot::SpicyParticleSystemNode::set_rotation_generator(const Ref<RotationGenerator>& p_rotation_generator)
{
	m_emitter->remove_generator(m_rotation_generator);
	m_rotation_generator = p_rotation_generator;

	if (m_rotation_generator.is_valid())
		m_emitter->add_generator(m_rotation_generator);
}

Ref<RotationGenerator> godot::SpicyParticleSystemNode::get_rotation_generator() const
{
	return m_rotation_generator;
}

void godot::SpicyParticleSystemNode::set_velocity_generator(const Ref<VelocityGenerator>& p_velocity_generator)
{
	m_emitter->remove_generator(m_velocity_generator);
	m_velocity_generator = p_velocity_generator;

	if (m_velocity_generator.is_valid())
		m_emitter->add_generator(m_velocity_generator);
}

Ref<VelocityGenerator> godot::SpicyParticleSystemNode::get_velocity_generator() const
{
	return m_velocity_generator;
}

void godot::SpicyParticleSystemNode::set_color_updater(const Ref<ColorUpdater>& p_color_updater)
{
	m_particle_system->remove_updater(m_color_updater);
	m_color_updater = p_color_updater;

	if (m_color_updater.is_valid())
		m_particle_system->add_updater(m_color_updater);
}

Ref<ColorUpdater> godot::SpicyParticleSystemNode::get_color_updater() const
{
	return m_color_updater;
}

void godot::SpicyParticleSystemNode::set_velocity_updater(const Ref<VelocityUpdater>& p_velocity_updater)
{
	m_particle_system->remove_updater(m_velocity_updater);
	m_velocity_updater = p_velocity_updater;

	if (m_velocity_updater.is_valid())
		m_particle_system->add_updater(m_velocity_updater);
}

Ref<VelocityUpdater> godot::SpicyParticleSystemNode::get_velocity_updater() const
{
	return m_velocity_updater;
}

void godot::SpicyParticleSystemNode::set_acceleration_updater(const Ref<AccelerationUpdater>& p_acceleration_updater)
{
	m_particle_system->remove_updater(m_acceleration_updater);
	m_acceleration_updater = p_acceleration_updater;

	if (m_acceleration_updater.is_valid())
		m_particle_system->add_updater(m_acceleration_updater);
}

Ref<AccelerationUpdater> godot::SpicyParticleSystemNode::get_acceleration_updater() const
{
	return m_acceleration_updater;
}

void godot::SpicyParticleSystemNode::set_rotation_updater(const Ref<RotationUpdater>& p_rotation_updater)
{
	m_particle_system->remove_updater(m_rotation_updater);
	m_rotation_updater = p_rotation_updater;

	if (m_rotation_updater.is_valid())
		m_particle_system->add_updater(m_rotation_updater);
}

Ref<RotationUpdater> godot::SpicyParticleSystemNode::get_rotation_updater() const
{
	return m_rotation_updater;
}

void godot::SpicyParticleSystemNode::set_size_updater(const Ref<SizeUpdater>& p_size_updater)
{
	m_particle_system->remove_updater(m_size_updater);
	m_size_updater = p_size_updater;

	if (m_size_updater.is_valid())
		m_particle_system->add_updater(m_size_updater);
}

Ref<SizeUpdater> godot::SpicyParticleSystemNode::get_size_updater() const
{
	return m_size_updater;
}

void godot::SpicyParticleSystemNode::set_max_particle_count(int p_max_particle_count)
{
	max_particles = p_max_particle_count;

	if (!initialized)
		return;

	m_particle_system->set_max_particle_count(max_particles, rng);
	m_renderer->resize_buffer();

	stop();
}

int godot::SpicyParticleSystemNode::get_max_particle_count() const
{
	return max_particles;
}

int godot::SpicyParticleSystemNode::get_particle_count() const
{
	if (initialized)
		return m_particle_system->num_alive_particles();

	return -1;
}

//void godot::SpicyParticleSystemNode::set_custom_renderer_data(bool p_custom_renderer_data)
//{
//	m_custom_renderer_data = p_custom_renderer_data;
//
//	update_configuration_warnings();
//}
//
//bool godot::SpicyParticleSystemNode::get_custom_renderer_data() const
//{
//	return m_custom_renderer_data;
//}

