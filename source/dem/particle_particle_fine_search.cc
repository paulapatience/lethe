#include <dem/dem_container_manager.h>
#include <dem/particle_particle_fine_search.h>

using namespace dealii;

template <int dim>
ParticleParticleFineSearch<dim>::ParticleParticleFineSearch()
{}

template <int dim>
void
ParticleParticleFineSearch<dim>::particle_particle_fine_search(
  typename DEM::dem_data_structures<dim>::particle_index_iterator_map
    &particle_container,
  typename DEM::dem_data_structures<dim>::adjacent_particle_pairs
    &adjacent_particles,
  const typename DEM::dem_data_structures<dim>::particle_particle_candidates
    &                  contact_pair_candidates,
  const double         neighborhood_threshold,
  const Tensor<1, dim> periodic_offset)
{
  // First iterating over adjacent_particles
  for (auto &&adjacent_particles_list :
       adjacent_particles | boost::adaptors::map_values)
    {
      // Iterating over each map which contains the contact information
      // including particles I and II
      for (auto adjacent_particles_list_iterator =
             adjacent_particles_list.begin();
           adjacent_particles_list_iterator != adjacent_particles_list.end();)
        {
          // Getting contact information and particles I and II as local
          // variables
          auto adjacent_pair_information =
            adjacent_particles_list_iterator->second;
          auto particle_one = adjacent_pair_information.particle_one;
          auto particle_two = adjacent_pair_information.particle_two;

          // Finding the properties of the particles in contact
          Point<dim, double> particle_one_location =
            particle_one->get_location();
          Point<dim, double> particle_two_location =
            particle_two->get_location() - periodic_offset;

          // Finding distance
          const double square_distance =
            particle_one_location.distance_square(particle_two_location);
          if (square_distance > neighborhood_threshold)
            {
              adjacent_particles_list.erase(adjacent_particles_list_iterator++);
            }
          else
            {
              ++adjacent_particles_list_iterator;
            }
        }
    }

  // Now iterating over contact_pair_candidates (maps of pairs), which
  // is the output of broad search. If a pair is in vicinity (distance <
  // threshold), it is added to the adjacent_particles
  for (auto const &[particle_one_id, second_particle_container] :
       contact_pair_candidates)
    {
      if (!second_particle_container.empty())
        {
          auto               particle_one = particle_container[particle_one_id];
          Point<dim, double> particle_one_location =
            particle_one->get_location();

          for (const unsigned int &particle_two_id : second_particle_container)
            {
              auto particle_two = particle_container[particle_two_id];
              Point<dim, double> particle_two_location =
                particle_two->get_location() - periodic_offset;

              // Finding distance
              const double square_distance =
                particle_one_location.distance_square(particle_two_location);

              // If the particles distance is less than the threshold
              if (square_distance < neighborhood_threshold)
                {
                  // Getting the particle one contact list and particle two id
                  auto particle_one_contact_list =
                    &adjacent_particles[particle_one_id];

                  particle_one_contact_list->emplace(
                    particle_two_id,
                    particle_particle_contact_info<dim>(particle_one,
                                                        particle_two));
                }
            }
        }
    }
}

template class ParticleParticleFineSearch<2>;
template class ParticleParticleFineSearch<3>;
