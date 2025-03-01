#include <gtest/gtest.h>

#include "extension_template.hpp"

TEST(ExtensionTemplate, dolfinx_git_commit_hash)
{
  std::string hash = extension_template::dolfinx_git_commit_hash();
  EXPECT_FALSE(hash.size() == 0);
}
