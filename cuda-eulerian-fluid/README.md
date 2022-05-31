[https://github.com/hecbarcab/code-samples](⬅️ Press here to go back to the other code samples)

# CUDA-Accelerated Eulerian Fluid Solver

Implementation of a grid-based Eulerian simulator solving the incompressible Euler equations. The solver employs a semi-Lagrangian advection scheme and a Jacobi-based pressure solver for the divergence-free projection. It supports dynamic Dirichlet boundary conditions for simple geometric shapes, which is used for modelling user interaction. Used as basis for two of the works of my Ph.D. thesis.

## Features
* **Pure header implementation**. Slow on compilation, but achieves very nice inlining of GPU kernel code.
* **Single-source CPU and GPU solvers**. Code is shared across both simulator backends thanks to simple compute abstraction.
* **Parallelized advection-projection scheme**. Implements semi-Lagrangian scheme for both self and auxiliary field advection. Pressure projection is GPU-accelerated using a Jacobi relaxation scheme.
* **Support for arbitrary Dirichlet and Neumann boundary conditions**. Both on the domain faces and at arbitrary cell locations.
* **Staggered MAC grid scheme**. To prevent null-space in velocity derivatives.
* **Vorticity confinement**. Helps reintroduce energy into the system at locations where vortices have formed.

See the *Related Work* chapter of my [Doctoral Thesis](https://hecbarcab.github.io/uploads/thesis-hector-barreiro-cabrera.pdf) for a theoretical overview of the principles behind the simulator.

## Related publications
* [https://www.cs.cmu.edu/~kmcrane/Projects/GPUFluid/](Real Time Simulation and Rendering of 3D Fluids. Crane, Keenan and Llamas, Ignacio, and Tariq, Sarah.)
* [https://web.stanford.edu/class/cs237d/smoke.pdf](Visual Simulation of Smoke. Fedkiw, Ronald and Stam, Jos and Wann Jensen, Henrik)

## License

```
MIT License

Copyright (c) 2022 Héctor Barreiro Cabrera, Stephen Sinclair, Miguel A. Otaduy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```