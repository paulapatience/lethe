#include "core/parameters_lagrangian.h"

namespace Parameters
{
  namespace Lagrangian
  {
    void
    PhysicalProperties::declare_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("physical properties");
      {
        prm.declare_entry("gx",
                          "1.",
                          Patterns::Double(),
                          "Gravitational acceleration in x direction");
        prm.declare_entry("gy",
                          "1.",
                          Patterns::Double(),
                          "Gravitational acceleration in y direction");
        prm.declare_entry("gz",
                          "1.",
                          Patterns::Double(),
                          "Gravitational acceleration in z direction");
        prm.declare_entry("diameter",
                          "1.",
                          Patterns::Double(),
                          "Particle diameter");
        prm.declare_entry("density",
                          "1.",
                          Patterns::Double(),
                          "Particle density");
        prm.declare_entry("young modulus particle",
                          "1.",
                          Patterns::Double(),
                          "Young's modulus of particle");
        prm.declare_entry("young modulus wall",
                          "1.",
                          Patterns::Double(),
                          "Young's modulus of wall");
        prm.declare_entry("poisson ratio particle",
                          "1.",
                          Patterns::Double(),
                          "Poisson's ratio of particle");
        prm.declare_entry("poisson ratio wall",
                          "1.",
                          Patterns::Double(),
                          "Poisson's ratio of wall");
        prm.declare_entry("restitution coefficient particle",
                          "1.",
                          Patterns::Double(),
                          "Coefficient of restitution of particle");
        prm.declare_entry("restitution coefficient wall",
                          "1.",
                          Patterns::Double(),
                          "Coefficient of restitution of wall");
        prm.declare_entry("friction coefficient particle",
                          "1.",
                          Patterns::Double(),
                          "Friction coefficient of particle");
        prm.declare_entry("friction coefficient wall",
                          "1.",
                          Patterns::Double(),
                          "Friction coefficient of wall");
        prm.declare_entry("rolling friction particle",
                          "1.",
                          Patterns::Double(),
                          "Rolling friction coefficient of particle");
        prm.declare_entry("rolling friction wall",
                          "1.",
                          Patterns::Double(),
                          "Rolling friction coefficient of wall");
      }
      prm.leave_subsection();
    }

    void
    PhysicalProperties::parse_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("physical properties");
      {
        gx                      = prm.get_double("gx");
        gy                      = prm.get_double("gy");
        gz                      = prm.get_double("gz");
        diameter                = prm.get_double("diameter");
        density                 = prm.get_double("density");
        youngs_modulus_particle = prm.get_integer("young modulus particle");
        youngs_modulus_wall     = prm.get_integer("young modulus wall");
        poisson_ratio_particle  = prm.get_double("poisson ratio particle");
        poisson_ratio_wall      = prm.get_double("poisson ratio wall");
        restitution_coefficient_particle =
          prm.get_double("restitution coefficient particle");
        restitution_coefficient_wall =
          prm.get_double("restitution coefficient wall");
        friction_coefficient_particle =
          prm.get_double("friction coefficient particle");
        friction_coefficient_wall = prm.get_double("friction coefficient wall");
        rolling_friction_particle = prm.get_double("rolling friction particle");
        rolling_friction_wall     = prm.get_double("rolling friction wall");
      }
      prm.leave_subsection();
    }

    void
    InsertionInfo::declare_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("insertion info");
      {
        prm.declare_entry("insertion method",
                          "non_uniform",
                          Patterns::Selection("uniform|non_uniform"),
                          "Choosing insertion method. "
                          "Choices are <uniform|non_uniform>.");
        prm.declare_entry("n total",
                          "1",
                          Patterns::Integer(),
                          "Total number of particles");
        prm.declare_entry("inserted number of particles at each time step",
                          "1",
                          Patterns::Integer(),
                          "Inserted number of particles at each time step");
        prm.declare_entry("insertion frequency",
                          "1",
                          Patterns::Integer(),
                          "Insertion frequncy");
        prm.declare_entry("insertion box minimum x",
                          "1",
                          Patterns::Double(),
                          "Insertion x min");
        prm.declare_entry("insertion box minimum y",
                          "1",
                          Patterns::Double(),
                          "Insertion y min");
        prm.declare_entry("insertion box minimum z",
                          "1",
                          Patterns::Double(),
                          "Insertion z min");
        prm.declare_entry("insertion box maximum x",
                          "1",
                          Patterns::Double(),
                          "Insertion x max");
        prm.declare_entry("insertion box maximum y",
                          "1",
                          Patterns::Double(),
                          "Insertion y max");
        prm.declare_entry("insertion box maximum z",
                          "1",
                          Patterns::Double(),
                          "Insertion z max");
        prm.declare_entry("insertion distance threshold",
                          "1",
                          Patterns::Double(),
                          "Distance threshold");
        prm.declare_entry("insertion random number range",
                          "1",
                          Patterns::Double(),
                          "Random number range");
        prm.declare_entry("insertion random number seed",
                          "1",
                          Patterns::Integer(),
                          "Random number seed");
      }
      prm.leave_subsection();
    }

    void
    InsertionInfo::parse_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("insertion info");
      {
        const std::string insertion = prm.get("insertion method");
        if (insertion == "uniform")
          insertion_method = InsertionMethod::uniform;
        else if (insertion == "non_uniform")
          insertion_method = InsertionMethod::non_uniform;
        else
          {
            std::runtime_error("Invalid insertion method");
          }
        total_particle_number = prm.get_integer("n total");
        inserted_this_step =
          prm.get_integer("inserted number of particles at each time step");
        insertion_frequency = prm.get_integer("insertion frequency");
        x_min               = prm.get_double("insertion box minimum x");
        y_min               = prm.get_double("insertion box minimum y");
        z_min               = prm.get_double("insertion box minimum z");
        x_max               = prm.get_double("insertion box maximum x");
        y_max               = prm.get_double("insertion box maximum y");
        z_max               = prm.get_double("insertion box maximum z");
        distance_threshold  = prm.get_double("insertion distance threshold");
        random_number_range = prm.get_double("insertion random number range");
        random_number_seed  = prm.get_double("insertion random number seed");
      }
      prm.leave_subsection();
    }

    void
    ModelParameters::declare_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("model parameters");
      {
        prm.declare_entry(
          "repartition frequency",
          "1000000000",
          Patterns::Integer(),
          "Frequency at which the triangulation is repartitioned "
          "and load is balanced");

        prm.declare_entry("contact detection method",
                          "dynamic",
                          Patterns::Selection("constant|dynamic"),
                          "Choosing contact detection method"
                          "Choices are <constant|dynamic>.");

        prm.declare_entry("contact detection frequency",
                          "1",
                          Patterns::Integer(),
                          "Particle-particle contact list");

        prm.declare_entry("dynamic contact search size coefficient",
                          "0.8",
                          Patterns::Double(),
                          "Security coefficient for dynamic contact detection");

        prm.declare_entry(
          "neighborhood threshold",
          "1",
          Patterns::Double(),
          "Contact search zone diameter to particle diameter ratio");

        prm.declare_entry("particle particle contact force method",
                          "pp_nonlinear",
                          Patterns::Selection("pp_linear|pp_nonlinear"),
                          "Choosing particle-particle contact force model"
                          "Choices are <pp_linear|pp_nonlinear>.");

        prm.declare_entry("particle wall contact force method",
                          "pw_nonlinear",
                          Patterns::Selection("pw_linear|pw_nonlinear"),
                          "Choosing particle-wall contact force model"
                          "Choices are <pw_linear|pw_nonlinear>.");

        prm.declare_entry("integration method",
                          "velocity_verlet",
                          Patterns::Selection("velocity_verlet|explicit_euler"),
                          "Choosing integration method"
                          "Choices are <velocity_verlet|explicit_euler>.");
      }
      prm.leave_subsection();
    }

    void
    ModelParameters::parse_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("model parameters");
      {
        repartition_frequency = prm.get_integer("repartition frequency");

        const std::string contact_search = prm.get("contact detection method");
        if (contact_search == "constant")
          {
            contact_detection_method = ContactDetectionMethod::constant;
            contact_detection_frequency =
              prm.get_integer("contact detection frequency");
          }
        else if (contact_search == "dynamic")
          {
            contact_detection_method = ContactDetectionMethod::dynamic;
            dynamic_contact_search_factor =
              prm.get_double("dynamic contact search size coefficient");
          }
        else
          {
            std::runtime_error("Invalid insertion method");
          }
        neighborhood_threshold = prm.get_double("neighborhood threshold");

        const std::string ppcf =
          prm.get("particle particle contact force method");
        if (ppcf == "pp_linear")
          pp_contact_force_method = PPContactForceModel::pp_linear;
        else if (ppcf == "pp_nonlinear")
          pp_contact_force_method = PPContactForceModel::pp_nonlinear;
        else
          {
            std::runtime_error("Invalid particle-particle contact force model");
          }

        const std::string pwcf = prm.get("particle wall contact force method");
        if (pwcf == "pw_linear")
          pw_contact_force_method = PWContactForceModel::pw_linear;
        else if (pwcf == "pw_nonlinear")
          pw_contact_force_method = PWContactForceModel::pw_nonlinear;
        else
          {
            std::runtime_error("Invalid particle-wall contact force model");
          }

        const std::string integration = prm.get("integration method");
        if (integration == "velocity_verlet")
          integration_method = IntegrationMethod::velocity_verlet;
        else if (integration == "explicit_euler")
          integration_method = IntegrationMethod::explicit_euler;
        else
          {
            std::runtime_error("Invalid integration method");
          }
      }
      prm.leave_subsection();
    }

    template <int dim>
    void
    FloatingWalls<dim>::declareDefaultEntry(ParameterHandler &prm)
    {
      prm.enter_subsection("point on wall");
      prm.declare_entry("x", "0.", Patterns::Double(), "X Point on wall");
      prm.declare_entry("y", "0.", Patterns::Double(), "Y Point on wall");
      prm.declare_entry("z", "0.", Patterns::Double(), "Z Point on wall");
      prm.leave_subsection();

      prm.enter_subsection("normal vector");
      prm.declare_entry("nx", "0.", Patterns::Double(), "X Normal vector wall");
      prm.declare_entry("ny", "0.", Patterns::Double(), "Y Normal vector wall");
      prm.declare_entry("nz", "0.", Patterns::Double(), "Z Normal vector wall");
      prm.leave_subsection();

      prm.declare_entry("start time", "0.", Patterns::Double(), "Start time");

      prm.declare_entry("end time", "0.", Patterns::Double(), "End time");
    }

    template <int dim>
    void
    FloatingWalls<dim>::parse_floating_wall(ParameterHandler &prm)
    {
      prm.enter_subsection("point on wall");
      Point<dim> wall_point;
      wall_point[0] = prm.get_double("x");
      wall_point[1] = prm.get_double("y");
      if (dim == 3)
        wall_point[2] = prm.get_double("z");
      this->points_on_walls.push_back(wall_point);
      prm.leave_subsection();

      prm.enter_subsection("normal vector");
      Tensor<1, dim> wall_normal;
      wall_normal[0] = prm.get_double("nx");
      wall_normal[1] = prm.get_double("ny");
      if (dim == 3)
        wall_normal[2] = prm.get_double("nz");
      this->floating_walls_normal_vectors.push_back(wall_normal);
      prm.leave_subsection();

      time_start.push_back(prm.get_double("start time"));
      time_end.push_back(prm.get_double("end time"));
    }

    template <int dim>
    void
    FloatingWalls<dim>::declare_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("floating walls");
      {
        prm.declare_entry("number of floating walls",
                          "0",
                          Patterns::Integer(),
                          "Number of floating walls");

        prm.enter_subsection("wall 0");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 1");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 2");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 3");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 4");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 5");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 6");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 7");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 8");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("wall 9");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();
      }
      prm.leave_subsection();
    }

    template <int dim>
    void
    FloatingWalls<dim>::parse_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("floating walls");
      {
        floating_walls_number = prm.get_integer("number of floating walls");

        if (floating_walls_number >= 1)
          {
            prm.enter_subsection("wall 0");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 2)
          {
            prm.enter_subsection("wall 1");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 3)
          {
            prm.enter_subsection("wall 2");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 4)
          {
            prm.enter_subsection("wall 3");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 5)
          {
            prm.enter_subsection("wall 4");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 6)
          {
            prm.enter_subsection("wall 5");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 7)
          {
            prm.enter_subsection("wall 6");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 8)
          {
            prm.enter_subsection("wall 7");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 9)
          {
            prm.enter_subsection("wall 8");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
        if (floating_walls_number >= 10)
          {
            prm.enter_subsection("wall 9");
            {
              parse_floating_wall(prm);
            }
            prm.leave_subsection();
          }
      }
      prm.leave_subsection();
    }

    template <int dim>
    void
    BoundaryMotion<dim>::declareDefaultEntry(ParameterHandler &prm)
    {
      prm.declare_entry("boundary id",
                        "0",
                        Patterns::Integer(),
                        "Moving boundary ID");
      prm.declare_entry("type",
                        "none",
                        Patterns::Selection("none|translational|rotational"),
                        "Type of boundary rotation"
                        "Choices are <none|translational|rotational>.");

      prm.declare_entry("speed x",
                        "0.",
                        Patterns::Double(),
                        "Translational boundary speed in x direction");
      prm.declare_entry("speed y",
                        "0.",
                        Patterns::Double(),
                        "Translational boundary speed in y direction");
      prm.declare_entry("speed z",
                        "0.",
                        Patterns::Double(),
                        "Translational boundary speed in z direction");

      prm.declare_entry("rotational speed",
                        "0.",
                        Patterns::Double(),
                        "Rotational boundary speed");
      prm.declare_entry("rotational vector x",
                        "0.",
                        Patterns::Double(),
                        "Rotational vector element in x direction");
      prm.declare_entry("rotational vector y",
                        "0.",
                        Patterns::Double(),
                        "Rotational vector element in y direction");
      prm.declare_entry("rotational vector z",
                        "0.",
                        Patterns::Double(),
                        "Rotational vector element in z direction");
    }

    template <int dim>
    void
    BoundaryMotion<dim>::parse_boundary_motion(ParameterHandler &prm)
    {
      const unsigned int boundary_id = prm.get_integer("boundary id");
      const std::string  motion_type = prm.get("type");

      if (motion_type == "translational")
        {
          Tensor<1, dim> translational_velocity;
          translational_velocity[0] = prm.get_double("speed x");
          translational_velocity[1] = prm.get_double("speed y");
          if (dim == 3)
            translational_velocity[2] = prm.get_double("speed z");

          this->boundary_translational_velocity.at(boundary_id) =
            translational_velocity;
        }

      if (motion_type == "rotational")
        {
          double         rotational_speed = prm.get_double("rotational speed");
          Tensor<1, dim> rotational_vector;
          if (dim == 3)
            {
              rotational_vector[0] = prm.get_double("rotational vector x");
              rotational_vector[1] = prm.get_double("rotational vector y");
              rotational_vector[2] = prm.get_double("rotational vector z");
            }

          this->boundary_rotational_speed.at(boundary_id)  = rotational_speed;
          this->boundary_rotational_vector.at(boundary_id) = rotational_vector;
        }
    }

    template <int dim>
    void
    BoundaryMotion<dim>::declare_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("boundary motion");
      {
        prm.declare_entry("number of boundary motion",
                          "0",
                          Patterns::Integer(),
                          "Number of boundary motion");

        prm.enter_subsection("moving boundary 0");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("moving boundary 1");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("moving boundary 2");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("moving boundary 3");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("moving boundary 4");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("moving boundary 5");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();

        prm.enter_subsection("moving boundary 6");
        {
          declareDefaultEntry(prm);
        }
        prm.leave_subsection();
      }
      prm.leave_subsection();
    }

    template <int dim>
    void
    BoundaryMotion<dim>::parse_parameters(ParameterHandler &prm)
    {
      prm.enter_subsection("boundary motion");
      initialize_containers(boundary_translational_velocity,
                            boundary_rotational_speed,
                            boundary_rotational_vector);
      {
        moving_boundary_number = prm.get_integer("number of boundary motion");

        if (moving_boundary_number >= 1)
          {
            prm.enter_subsection("moving boundary 0");
            {
              parse_boundary_motion(prm);
            }
            prm.leave_subsection();
          }
        if (moving_boundary_number >= 2)
          {
            prm.enter_subsection("moving boundary 1");
            {
              parse_boundary_motion(prm);
            }
            prm.leave_subsection();
          }
        if (moving_boundary_number >= 3)
          {
            prm.enter_subsection("moving boundary 2");
            {
              parse_boundary_motion(prm);
            }
            prm.leave_subsection();
          }
        if (moving_boundary_number >= 4)
          {
            prm.enter_subsection("moving boundary 3");
            {
              parse_boundary_motion(prm);
            }
            prm.leave_subsection();
          }
        if (moving_boundary_number >= 5)
          {
            prm.enter_subsection("moving boundary 4");
            {
              parse_boundary_motion(prm);
            }
            prm.leave_subsection();
          }
        if (moving_boundary_number >= 6)
          {
            prm.enter_subsection("moving boundary 5");
            {
              parse_boundary_motion(prm);
            }
            prm.leave_subsection();
          }
      }
      prm.leave_subsection();
    }

    template <int dim>
    void
    BoundaryMotion<dim>::initialize_containers(
      std::unordered_map<int, Tensor<1, dim>> &boundary_translational_velocity,
      std::unordered_map<int, double> &        boundary_rotational_speed,
      std::unordered_map<int, Tensor<1, dim>> &boundary_rotational_vector)
    {
      Tensor<1, dim> zero_tensor;
      for (unsigned int d = 0; d < dim; ++d)
        {
          zero_tensor[d] = 0;
        }

      for (unsigned int counter = 0; counter < moving_boundary_maximum_number;
           ++counter)
        {
          boundary_translational_velocity.insert({counter, zero_tensor});
          boundary_rotational_speed.insert({counter, 0});
          boundary_rotational_vector.insert({counter, zero_tensor});
        }
    }

    template class FloatingWalls<2>;
    template class FloatingWalls<3>;
    template class BoundaryMotion<2>;
    template class BoundaryMotion<3>;

  } // namespace Lagrangian

} // namespace Parameters