
#include <iostream>

#include <extension_template/extension_template.hpp>

auto
main() -> int
{
  std::cout << "DOLFINx git commit hash (called through "
               "extension_template): "
            << extension_template::dolfinx_git_commit_hash() << std::endl;
}