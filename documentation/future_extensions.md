# Future Extensions and Extension Architecture

## 1. Purpose

This project is intentionally structured so that advanced numerical features can be added without rewriting the core API or breaking the existing solver workflow. The current library already separates problem definition, method selection, execution, diagnostics, and reporting; that separation is the foundation for future extension.

## 2. Extension design rules

The following principles must remain true for every future extension:

1. Keep the public API stable and minimal.
2. Add new capability behind a dedicated module rather than folding it into existing algorithms.
3. Reuse the common `NUMERIX_Function`, `NUMERIX_Problem`, `NUMERIX_Result`, and reporting types where appropriate.
4. Distinguish heuristic analysis from mathematically proven guarantees.
5. Keep optional dependency code behind explicit integration layers instead of coupling it to the core library.
6. Preserve deterministic behavior where reproducibility matters.

## 3. Extension roadmap

### 3.1 Automatic differentiation

Recommended architecture:

- Introduce a lightweight dual-number layer under `src/automatic_differentiation/`.
- Add a `NUMERIX_Dual` or equivalent type that stores value and derivative components.
- Extend function evaluation to accept dual-valued inputs in dedicated AD paths.
- Keep the default numerical callback path unchanged for ordinary users.

This supports derivative estimation without requiring users to reimplement callback logic or expose internals of solver functions.

### 3.2 Sparse matrix support

Recommended architecture:

- Define a separate sparse matrix abstraction with CSR or COO storage.
- Keep dense matrix operations in the existing linear algebra layer.
- Add a `NUMERIX_SparseMatrix` type that exposes row/column indices and values independently from dense storage.
- Provide sparse solver interfaces only when a compatible algorithm is implemented.

The key is to avoid extending dense array logic into a sparse representation that would confuse the main API.

### 3.3 Verified interval arithmetic

Recommended architecture:

- Add an interval type with lower and upper endpoints.
- Separate proof-oriented checks from ordinary numerical approximations.
- Make interval operations optional and clearly flagged as verification-oriented, not replacement for general fast arithmetic.

This should not be mixed into every result type. A verification layer can report interval containment and confidence separately from standard outputs.

### 3.4 Input uncertainty propagation

Recommended architecture:

- Add a parameter uncertainty report or distribution descriptor.
- Compute how input perturbations propagate into output values, residuals, and solver state.
- Store uncertainty data as a diagnostics artifact, not as a new general numerical result.

This is best implemented as a planning and diagnostic capability rather than a solver core modification.

### 3.5 Deterministic parallel computation

Recommended architecture:

- Keep scheduling logic in the execution layer.
- Add a reproducibility policy that controls reduction order.
- Guarantee fixed ordering for reductions, merging, and reporting data.
- Keep thread safety isolated from dense algorithm code.

This should not be hard-coded into each numerical method; the parallel portfolio layer should own the deterministic behavior.

### 3.6 Checkpoint and resume

Recommended architecture:

- Add `NUMERIX_Checkpoint` and `NUMERIX_ResumeState` structures.
- Save iteration counters, state variables, convergence data, and active method metadata.
- Keep persistence logic in a separate checkpoint module, not spread across numeric kernels.

This fits naturally into the execution controller and its step-by-step monitor.

### 3.7 Automatic breakpoint detection

Recommended architecture:

- Use the existing piecewise and discontinuity analysis tools as a foundation.
- Classify breakpoints as heuristic or proof-backed.
- Keep interpretation explicit: heuristics should never be presented as mathematical proofs.

This is a natural extension to the discontinuity-analysis layer.

### 3.8 Numerical problem visualization

Recommended architecture:

- Export function samples, solver traces, and convergence curves as simple CSV or plain-text data.
- Keep CLI output generation separate from core numerical routines.
- Let the CLI and benchmark layers handle serialization and plotting metadata.

This keeps the library independent of plotting libraries and GUI assumptions.

### 3.9 Reproducible experiment runner

Recommended architecture:

- Save problem configuration, solver settings, tolerance values, and method selection.
- Recreate the experiment using a deterministic configuration object.
- Store benchmark metadata in a report rather than scattering it across modules.

This belongs naturally in the benchmarking and reporting layer, not in numerical algorithm code.

## 4. Recommended future module layout

```text
src/
  automatic_differentiation/
  sparse/
  interval/
  uncertainty/
  checkpointing/
  visualization/
  experiment_runner/
```

Each one should expose only its own public types and should integrate through the existing `NUMERIX_Function`, `NUMERIX_Problem`, and reporting structures.

## 5. Practical compatibility with the current project

The current design already supports the required extension points:

- `NUMERIX_Function` handles callback and expression-based evaluation.
- `NUMERIX_Problem` captures the mathematical setup.
- `NUMERIX_Result` provides execution and diagnostics fields.
- `NUMERIX_CLIConfig` supports interaction through the CLI layer.
- `NUMERIX_ConvergenceMonitor`, `NUMERIX_ExecutionLimits`, and reliability reports provide observational hooks for future features.

This means future work can be layered in without restructuring the numerical core, which satisfies the goal of keeping the library modular and evolvable.
