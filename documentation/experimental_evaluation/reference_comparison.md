# Phase 10: Reference Library Comparison

## Objective

Compare the NUMERIX implementation against common scientific reference stacks while keeping the comparison fair, documented, and reproducible.

## Scope

The comparison framework includes:

- NUMERIX itself
- GSL
- BLAS
- LAPACK
- Numerical Recipes reference implementations, where licensing and source availability permit direct comparison

## Methodology

1. Use the same root-finding problem for each library.
2. Use equivalent numerical tolerances.
3. Use the same compiler optimization settings.
4. Measure residual, runtime, and convergence behavior.
5. Record whether a library is available in the current environment.

## Current environment note

This repository does not ship a configured GSL, BLAS, or LAPACK runtime in the local build environment. Therefore, the comparison layer records those libraries as available = 0 and documents that they were not benchmarked directly in this workspace.

## Reporting rule

The library must not claim a direct source-code benchmark where no installation or reference implementation is available. Instead, it records the comparison as a structured reference analysis with a clear availability note.

## Result format

The comparison engine stores:

- library identity
- chosen method
- residual
- accuracy
- runtime data
- availability status
- explanatory note

This keeps the project honest while still providing a standard comparison interface.
