#ifndef SPICY_PARTICLE_DATA_H
#define SPICY_PARTICLE_DATA_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/variant/packed_float32_array.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot
{
	class ParticleData : public RefCounted
	{	GDCLASS(ParticleData, RefCounted)		
	private:
		bool initialized;
	public:	
		//particle data
		Vector3* position;
		Vector3* rotation;
		Vector3* scale;
		Vector3* velocity;
		Vector3* current_velocity;
		Vector3* current_scale;
		Vector3* current_rotation;
		Vector3* acceleration;
		Color* current_color;
		Color* color;
		float* lifetime;
		float* life_remaining;
		float* normalized_lifetime;
		bool* alive;

		//simulation data
		const Node3D* particle_node;
		Transform3D emitter_transform;

		size_t particle_count;
		size_t count_alive;

		Ref<RandomNumberGenerator> rng;

		double max_duration;
		double current_duration_normalized;
	protected:
		static void _bind_methods() {}
	public:
		inline ParticleData() : initialized(false),
			particle_count(0),
			count_alive(0), 
			max_duration(0),
			current_duration_normalized(0),
			particle_node(nullptr),
			emitter_transform(Transform3D()) { }

		~ParticleData();
		void delete_data();

		void generate(size_t max_size, const Ref<RandomNumberGenerator>& p_rng);
		void kill(size_t id);
		void wake(size_t id);
		void swap(size_t id1, size_t id2);

		void get_transform(size_t id, Transform3D& p_transform) const;

		void get_color(size_t id, Color& p_color) const;

		size_t get_last_alive_index();
	};

}
#endif // !SPICY_PARTICLE_DATA_H