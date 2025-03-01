#include "extension_template.hpp"

#include <dolfinx/common/defines.h>

auto
extension_template::dolfinx_git_commit_hash() -> std::string
{
  return dolfinx::git_commit_hash();
}
