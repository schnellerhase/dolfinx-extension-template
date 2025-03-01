#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <extension_template/extension_template.hpp>

namespace nb = nanobind;

using namespace nb::literals;

NB_MODULE(extension_template_mod, extension_template)
{

  extension_template.def("dolfinx_git_commit_hash",
                         &extension_template::dolfinx_git_commit_hash);
}
