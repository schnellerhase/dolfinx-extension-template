#include <cstddef>

#include <mpi.h>
#include <petscmat.h>
#include <petscsys.h>
#include <petscsystypes.h>

#include <basix/finite-element.h>

#include <dolfinx/fem/petsc.h>
#include <dolfinx/fem/utils.h>
#include <dolfinx/la/petsc.h>
#include <dolfinx/mesh/generation.h>

#include "form.h"

using namespace dolfinx;

auto
main() -> int
{
  using T = dolfinx::scalar_value_t<PetscScalar>;

  MPI_Init(nullptr, nullptr);
  PetscInitialize(0, 0, nullptr, nullptr);

  auto mesh = std::make_shared<dolfinx::mesh::Mesh<T>>(
    dolfinx::mesh::create_rectangle<T>(MPI_COMM_WORLD,
                                       { { { 0.0, 0.0 }, { 1.0, 1.0 } } },
                                       { 8, 8 },
                                       dolfinx::mesh::CellType::triangle));

  auto element =
    basix::create_element<T>(basix::element::family::P,
                             basix::cell::type::triangle,
                             1,
                             basix::element::lagrange_variant::unset,
                             basix::element::dpc_variant::unset,
                             false);

  auto V = std::make_shared<fem::FunctionSpace<T>>(fem::create_functionspace<T>(
    mesh, std::make_shared<fem::FiniteElement<T>>(element)));

  fem::Form<T> L = fem::create_form<T>(*form_form_L, { V }, {}, {}, {}, {});

  la::Vector<T> b(L.function_spaces()[0]->dofmap()->index_map,
                  L.function_spaces()[0]->dofmap()->index_map_bs());

  b.set(0.0);
  fem::assemble_vector(b.mutable_array(), L);
  b.scatter_rev(std::plus<T>());

  // PetscFinalize();
  MPI_Finalize();
}