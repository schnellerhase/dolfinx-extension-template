#include "extension_template.hpp"

#include <dolfinx/common/defines.h>

std::string
extension_template::dolfinx_git_commit_hash()
{
  return dolfinx::git_commit_hash();
}
