#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>

#include <mpi.h>
#include <petscsys.h>
#include <petscsystypes.h>

#include <basix/cell.h>
#include <basix/element-families.h>
#include <basix/finite-element.h>

#include <dolfinx/common/types.h>
#include <dolfinx/fem/FiniteElement.h>
#include <dolfinx/fem/FunctionSpace.h>
#include <dolfinx/fem/assembler.h>
#include <dolfinx/fem/utils.h>
#include <dolfinx/la/Vector.h>
#include <dolfinx/mesh/Mesh.h>
#include <dolfinx/mesh/cell_types.h>
#include <dolfinx/mesh/generation.h>

#include "form.h"

void
assemble_form()
{
  using T = dolfinx::scalar_value_t<PetscScalar>;

  const std::int64_t n = 8;
  auto mesh = std::make_shared<dolfinx::mesh::Mesh<T>>(
    dolfinx::mesh::create_rectangle<T>(MPI_COMM_WORLD,
                                       { { { 0.0, 0.0 }, { 1.0, 1.0 } } },
                                       { n, n },
                                       dolfinx::mesh::CellType::triangle));

  auto element =
    basix::create_element<T>(basix::element::family::P,
                             basix::cell::type::triangle,
                             1,
                             basix::element::lagrange_variant::unset,
                             basix::element::dpc_variant::unset,
                             false);

  auto V = std::make_shared<dolfinx::fem::FunctionSpace<T>>(
    fem::create_functionspace<T>(
      mesh, std::make_shared<dolfinx::fem::FiniteElement<T>>(element)));

  const auto L = fem::create_form<T>(*form_form_L, { V }, {}, {}, {}, {});

  la::Vector<T> b(L.function_spaces()[0]->dofmap()->index_map,
                  L.function_spaces()[0]->dofmap()->index_map_bs());

  b.set(0.0);
  fem::assemble_vector(b.mutable_array(), L);
  b.scatter_rev(std::plus<>());
}

auto
main() -> int
{
  PetscInitialize(nullptr, nullptr, nullptr, nullptr);

  try {
    assemble_form();
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
  }

  PetscFinalize();
}
