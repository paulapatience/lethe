#include <core/ib_particle.h>
#include <core/shape_parsing.h>

#include <cfloat>


template <int dim>
void
IBParticle<dim>::initialize_all()
{
  // initialize all the variables associated to an immersed boundary particle.
  radius      = 1;
  particle_id = 0;

  inertia[0][0] = 1;
  inertia[1][1] = 1;
  inertia[2][2] = 1;

  fluid_forces[0] = 0;
  fluid_forces[1] = 0;

  velocity[0] = 0;
  velocity[1] = 0;

  position[0] = 0;
  position[1] = 0;

  fluid_torque[0] = 0;
  fluid_torque[1] = 0;
  fluid_torque[2] = 0;

  orientation[0] = 0;
  orientation[1] = 0;
  orientation[2] = 0;

  omega[0] = 0;
  omega[1] = 0;
  omega[2] = 0;

  if (dim == 3)
    {
      fluid_forces[2] = 0;
      velocity[2]     = 0;
      position[2]     = 0;
    }

  // Fill the vectors with default value
  previous_fluid_forces = fluid_forces;
  previous_fluid_torque = fluid_torque;
  velocity_iter         = velocity;

  omega_iter           = omega;
  omega_impulsion      = 0;
  omega_impulsion_iter = 0;
  impulsion            = 0;
  impulsion_iter       = 0;
  contact_impulsion    = 0;

  previous_positions.resize(3);
  previous_velocity.resize(3);
  previous_orientation.resize(3);
  previous_omega.resize(3);

  for (unsigned int i = 0; i < 3; ++i)
    {
      previous_positions[i]   = position;
      previous_velocity[i]    = velocity;
      previous_orientation[i] = orientation;
      previous_omega[i]       = omega;
    }
  residual_velocity = DBL_MAX;
  residual_omega    = DBL_MAX;

  f_position    = std::make_shared<Functions::ParsedFunction<dim>>(dim);
  f_velocity    = std::make_shared<Functions::ParsedFunction<dim>>(dim);
  f_omega       = std::make_shared<Functions::ParsedFunction<dim>>(3);
  f_orientation = std::make_shared<Functions::ParsedFunction<dim>>(3);
}

template <int dim>
void
IBParticle<dim>::initialize_previous_solution()
{
  // initialize all the variables associated to an immersed boundary particle
  previous_fluid_forces = fluid_forces;
  velocity_iter         = velocity;
  impulsion_iter        = impulsion;
  omega_iter            = omega;
  omega_impulsion_iter  = omega_impulsion;

  for (unsigned int i = 0; i < 3; ++i)
    {
      previous_positions[i]   = position;
      previous_velocity[i]    = velocity;
      previous_orientation[i] = orientation;
      previous_omega[i]       = omega;
    }
}

template <int dim>
std::vector<std::pair<std::string, int>>
IBParticle<dim>::get_properties_name()
{
  std::vector<std::pair<std::string, int>> properties(
    PropertiesIndex::n_properties);
  properties[PropertiesIndex::id] = std::make_pair("ID", 1);
  properties[PropertiesIndex::dp] = std::make_pair("Diameter", 1);
  properties[PropertiesIndex::vx] = std::make_pair("Velocity", 3);
  properties[PropertiesIndex::vy] = std::make_pair("Velocity", 1);
  properties[PropertiesIndex::vz] = std::make_pair("Velocity", 1);
  properties[PropertiesIndex::fx] = std::make_pair("Force", 3);
  properties[PropertiesIndex::fy] = std::make_pair("Force", 1);
  properties[PropertiesIndex::fz] = std::make_pair("Force", 1);
  properties[PropertiesIndex::m]  = std::make_pair("Mass", 1);
  properties[PropertiesIndex::ox] = std::make_pair("Omega", 3);
  properties[PropertiesIndex::oy] = std::make_pair("Omega", 1);
  properties[PropertiesIndex::oz] = std::make_pair("Omega", 1);
  properties[PropertiesIndex::tx] = std::make_pair("Torques", 3);
  properties[PropertiesIndex::ty] = std::make_pair("Torques", 1);
  properties[PropertiesIndex::tz] = std::make_pair("Torques", 1);

  return properties;
}

template <int dim>
std::vector<double>
IBParticle<dim>::get_properties()
{
  std::vector<double> properties(get_number_properties());
  properties[0]  = particle_id;
  properties[1]  = radius * 2.0;
  properties[2]  = velocity[0];
  properties[3]  = velocity[1];
  properties[5]  = fluid_forces[0];
  properties[6]  = fluid_forces[1];
  properties[8]  = omega[0];
  properties[9]  = omega[1];
  properties[10] = omega[2];
  properties[11] = mass;
  properties[12] = fluid_torque[0];
  properties[13] = fluid_torque[1];
  properties[14] = fluid_torque[2];
  if (dim == 2)
    {
      properties[4] = 0;
      properties[7] = 0;
    }
  if (dim == 3)
    {
      properties[4] = velocity[2];
      properties[7] = fluid_forces[2];
    }

  return properties;
}

template <int dim>
void
IBParticle<dim>::clear_shape_cache()
{
  this->shape->clear_cache();
}

template <int dim>
void
IBParticle<dim>::initialize_shape(const std::string         type,
                                  const std::vector<double> shape_arguments)
{
  shape = ShapeGenerator::initialize_shape_from_vector(type,
                                                       shape_arguments,
                                                       position,
                                                       orientation);
}

template <int dim>
void
IBParticle<dim>::initialize_shape(const std::string type,
                                  const std::string raw_arguments)
{
  shape = ShapeGenerator::initialize_shape(type,
                                           raw_arguments,
                                           position,
                                           orientation);
}

template <int dim>
void
IBParticle<dim>::closest_surface_point(
  const Point<dim> &                                    p,
  Point<dim> &                                          closest_point,
  const typename DoFHandler<dim>::active_cell_iterator &cell_guess)
{
  shape->closest_surface_point(p, closest_point, cell_guess);
}

template <int dim>
void
IBParticle<dim>::closest_surface_point(const Point<dim> &p,
                                       Point<dim> &      closest_point)
{
  shape->closest_surface_point(p, closest_point);
}

template <int dim>
bool
IBParticle<dim>::is_inside_crown(
  const Point<dim> &                                    evaluation_point,
  const double                                          outer_radius,
  const double                                          inside_radius,
  const typename DoFHandler<dim>::active_cell_iterator &cell_guess)
{
  const double radius = shape->effective_radius;

  double distance = shape->value_with_cell_guess(evaluation_point, cell_guess);
  bool   is_inside_outer_ring  = distance <= radius * (outer_radius - 1);
  bool   is_outside_inner_ring = distance >= radius * (inside_radius - 1);

  return is_inside_outer_ring && is_outside_inner_ring;
}

template <int dim>
bool
IBParticle<dim>::is_inside_crown(const Point<dim> &evaluation_point,
                                 const double      outer_radius,
                                 const double      inside_radius)
{
  const double radius = shape->effective_radius;

  double distance              = shape->value(evaluation_point);
  bool   is_inside_outer_ring  = distance <= radius * (outer_radius - 1);
  bool   is_outside_inner_ring = distance >= radius * (inside_radius - 1);

  return is_inside_outer_ring && is_outside_inner_ring;
}

template <int dim>
void
IBParticle<dim>::set_orientation(const Tensor<1, 3> new_orientation)
{
  this->orientation = new_orientation;
  this->shape->set_orientation(new_orientation);
}

template <int dim>
void
IBParticle<dim>::update_precalculations(
  DoFHandler<dim> &  updated_dof_handler,
  const unsigned int levels_not_precalculated)
{
  if (typeid(*shape) == typeid(RBFShape<dim>))
    {
      std::static_pointer_cast<RBFShape<dim>>(shape)->update_precalculations(
        updated_dof_handler, levels_not_precalculated);
    }
  else if (typeid(*shape) == typeid(CompositeShape<dim>))
    {
      std::static_pointer_cast<CompositeShape<dim>>(shape)
        ->update_precalculations(updated_dof_handler, levels_not_precalculated);
    }
}

template class IBParticle<2>;
template class IBParticle<3>;
