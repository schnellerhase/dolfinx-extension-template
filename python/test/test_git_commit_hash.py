import extension_template


def test_dolfinx_git_commit_hash():
    assert len(extension_template.dolfinx_git_commit_hash()) > 0
