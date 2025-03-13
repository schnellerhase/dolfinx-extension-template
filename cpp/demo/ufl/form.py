from basix.ufl import element
from ufl import (
    FunctionSpace,
    Mesh,
    TestFunction,
    dx,
    inner,
)

mesh = Mesh(element("Lagrange", "triangle", 1, shape=(2,)))
V = FunctionSpace(mesh, element("Lagrange", "triangle", 1))

v = TestFunction(V)
L = inner(1, v) * dx
