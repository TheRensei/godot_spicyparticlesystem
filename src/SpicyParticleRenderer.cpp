#include "SpicyParticleRenderer.hpp"
#include <godot_cpp/classes/rendering_server.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>


using namespace godot;

void godot::MultiMeshParticleRenderer::_bind_methods()
{
	BIND_ENUM_CONSTANT(ALIGNMENT_LOCAL);
	BIND_ENUM_CONSTANT(ALIGNMENT_WORLD);
	BIND_ENUM_CONSTANT(ALIGNMENT_SCREEN);
	BIND_ENUM_CONSTANT(ALIGNMENT_CAMERA);
	BIND_ENUM_CONSTANT(ALIGNMENT_VELOCITY);
	BIND_ENUM_CONSTANT(ALIGNMENT_LOOK_AT);
	BIND_ENUM_CONSTANT(ALIGNMENT_MAX);
}

godot::MultiMeshParticleRenderer::~MultiMeshParticleRenderer()
{
	destroy();
}

void godot::MultiMeshParticleRenderer::set_mesh(const Ref<Mesh>& p_mesh)
{
	RID meshRID = RID();

	if (p_mesh.is_valid()) {
		meshRID = p_mesh->get_rid();
	}

	RenderingServer::get_singleton()->multimesh_set_mesh(multimesh, meshRID);
}

Ref<Mesh> godot::MultiMeshParticleRenderer::get_mesh() const
{
	return RenderingServer::get_singleton()->multimesh_get_mesh(multimesh);
}

RID godot::MultiMeshParticleRenderer::get_multimesh() const
{
	return multimesh;
}

void godot::MultiMeshParticleRenderer::apply_alignment(const Ref<ParticleData> p_data, size_t p_id, Transform3D& out_transform)
{
	switch (m_alignment) {
	case ALIGNMENT_LOCAL:
	{
		//Should do this by default
		return;
	}
	break;
	case ALIGNMENT_WORLD:
	{
		out_transform.basis = p_data->particle_node->get_global_transform().affine_inverse().basis * out_transform.basis;
	}
	break;
	case ALIGNMENT_SCREEN:
	{
		if (m_alignment_target_node != NULL) {
			Basis p_node_basis = p_data->particle_node->get_global_transform().affine_inverse().basis;
			Basis target_basis = m_alignment_target_node->get_global_basis() * flip_xz;

			out_transform.basis = target_basis * out_transform.basis;
			out_transform.basis = p_node_basis * out_transform.basis;
		}
	}
	break;
	case ALIGNMENT_CAMERA:
	{

		if (m_alignment_target_node != NULL) {
			Basis p_node_basis = p_data->particle_node->get_global_transform().affine_inverse().basis;
			Basis target_basis = p_node_basis.looking_at(m_alignment_target_node->get_global_position(), Vector3(0, 1, 0));

			out_transform.basis = target_basis * out_transform.basis;
			out_transform.basis = p_node_basis * out_transform.basis;

		}
	}
	break;
	case ALIGNMENT_VELOCITY:
	{
		Vector3 v = p_data->velocity[p_id] + p_data->current_velocity[p_id] + p_data->acceleration[p_id];
		v.normalize();

		Basis p_node_basis = p_data->particle_node->get_global_transform().affine_inverse().basis;

		if (v.length_squared() > 0.0001 && !v.is_equal_approx(Vector3(0, 1, 0)))
			out_transform.basis = out_transform.basis.looking_at(v, out_transform.basis.rows[2]) * out_transform.basis;
		out_transform.basis = p_node_basis * out_transform.basis;

	}
	break;
	case ALIGNMENT_LOOK_AT:
	{
		if (m_alignment_target_node != NULL) {
			Basis p_node_basis = p_data->particle_node->get_global_transform().affine_inverse().basis;
			Basis target_basis = p_node_basis.looking_at(m_alignment_target_node->get_global_position(), Vector3(0, 1, 0));

			out_transform.basis = target_basis * out_transform.basis;
			out_transform.basis = p_node_basis * out_transform.basis;
		}
	}
	break;
	}
}

void godot::MultiMeshParticleRenderer::set_alignment(Alignment p_alignment)
{
	m_alignment = p_alignment;
}

void godot::MultiMeshParticleRenderer::set_alignment_target(const Node3D* p_alignment_target_node)
{
	m_alignment_target_node = p_alignment_target_node;
}

void MultiMeshParticleRenderer::generate(const Ref<SpicyParticleSystem> p_system, const Ref<Mesh>& p_mesh)
{
	m_system = p_system;

	mesh_data.resize(static_cast<int64_t>(mesh_data_size) * p_system->num_all_particles());
	mesh_data.fill(0);

	multimesh = RenderingServer::get_singleton()->multimesh_create();
	RenderingServer::get_singleton()->multimesh_set_visible_instances(multimesh, -1);
	RenderingServer::get_singleton()->multimesh_allocate_data(multimesh, p_system->num_all_particles(), RenderingServer::MULTIMESH_TRANSFORM_3D, true, true);

	set_mesh(p_mesh);
}

void godot::MultiMeshParticleRenderer::destroy()
{
	ERR_FAIL_NULL(RenderingServer::get_singleton());
	RenderingServer::get_singleton()->free_rid(multimesh);
}

void godot::MultiMeshParticleRenderer::update()
{
	float* ptr = mesh_data.ptrw();
	const Ref<ParticleData> p = m_system->final_data();

	Transform3D t;
	Color c;
	Basis b;

	for (size_t i = 0; i < m_system->num_alive_particles(); i++) {

		t.basis = b; //reset basis
		p->get_transform(i, t);
		p->get_color(i, c);
		apply_alignment(p, i, t);

		//t.translate_local(Vector3(0, 1, 0)); //maybe this to add pivot offset

		ptr[0] = t.basis.rows[0][0];
		ptr[1] = t.basis.rows[0][1];
		ptr[2] = t.basis.rows[0][2];
		ptr[3] = t.origin.x;
		ptr[4] = t.basis.rows[1][0];
		ptr[5] = t.basis.rows[1][1];
		ptr[6] = t.basis.rows[1][2];
		ptr[7] = t.origin.y;
		ptr[8] = t.basis.rows[2][0];
		ptr[9] = t.basis.rows[2][1];
		ptr[10] = t.basis.rows[2][2];
		ptr[11] = t.origin.z;

		////Color
		ptr[12] = c.r;
		ptr[13] = c.g;
		ptr[14] = c.b;
		ptr[15] = c.a;

		float rot = p->rotation[i].z + p->current_rotation[i].z; //that should be calculated before //optimize
		ptr[16] = Math::deg_to_rad(rot);
		ptr[17] = p->normalized_lifetime[i];
		ptr[18] = ptr[17];
		ptr[19] = 0;

		ptr += mesh_data_size;
	}
}

void godot::MultiMeshParticleRenderer::render()
{
	RenderingServer::get_singleton()->multimesh_set_buffer(multimesh, mesh_data);
	RenderingServer::get_singleton()->multimesh_set_visible_instances(multimesh, m_system->num_alive_particles());
}

void godot::MultiMeshParticleRenderer::reset()
{
	mesh_data.fill(0);
	render();
}

void godot::MultiMeshParticleRenderer::resize_buffer()
{
	mesh_data.resize(static_cast<int64_t>(mesh_data_size) * m_system->num_all_particles());
	mesh_data.fill(0);

	RenderingServer::get_singleton()->multimesh_set_visible_instances(multimesh, -1);
	RenderingServer::get_singleton()->multimesh_allocate_data(multimesh, m_system->num_all_particles(), RenderingServer::MULTIMESH_TRANSFORM_3D, true, true);
}
