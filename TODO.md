# GPU hardware and programming-model slide backlog

## Goal

Revise `gpu_programming.pptx` into a genuinely general introduction to GPU
hardware and high-level programming models. The main narrative should teach
portable concepts first, then show NVIDIA and AMD Instinct terminology and
important architectural differences.

The deck is shared introductory material for the more specific trainings in
this repository. It should therefore explain concepts and trade-offs without
becoming a CUDA, HIP, OpenMP, or Kokkos programming course.

## Editorial principles

- [ ] Introduce a vendor-neutral term before vendor-specific terminology.
- [ ] Use NVIDIA and AMD examples to illustrate a concept, not define it.
- [ ] Distinguish the programming model from a particular hardware
      implementation.
- [ ] Prefer capability and topology queries over hard-coded architectural
      assumptions.
- [ ] Label all product-specific numbers with the exact model and form factor.
- [ ] Distinguish bits from bytes and per-link, pairwise, per-device aggregate,
      and system aggregate bandwidth.
- [ ] Use `KB`, `MB`, and `GB` on learner-facing slides for simplicity. Treat
      them as vendor-style binary capacity labels (`KB` = 1,024 bytes) and use
      exact byte counts when presenting device-query output or calculations.
- [ ] Put volatile product specifications in an appendix rather than the core
      conceptual narrative.
- [ ] Add official sources for non-trivial hardware claims and tables to the
      speaker notes.

## P0: correct misleading or incorrect material

Complete this section before adding new AMD material; otherwise the new slides
will inherit ambiguous terminology and units.

Status reviewed on 2026-08-06 against the current working-tree deck (base
commit `91bb837`; the PowerPoint file has uncommitted edits). A checked P0 item
was verified in the current deck or explicitly accepted as a non-blocking
limitation; partial work is recorded with nested checkboxes.

### Execution model

- [x] **Slide 11: correct the AMD wavefront width.**
  - State that NVIDIA warps contain 32 threads.
  - State that AMD Instinct/CDNA wavefronts contain 64 work-items.
  - Mention that AMD Radeon/RDNA commonly uses wave32, so "AMD GPU" is not a
    sufficient architectural description.
  - Introduce *subgroup* as the vendor-neutral term.
  - Acceptance: no slide equates an AMD Instinct wavefront with 32 threads.

- [x] **Slide 24: remove "warp: implicit lockstep" as a correctness rule.**
  - Explain that subgroup execution is useful for understanding performance,
    but synchronization and memory ordering require the appropriate explicit
    primitives.
  - Mention NVIDIA independent thread scheduling where relevant.
  - Keep H100 thread-block clusters as a clearly labelled NVIDIA-specific
    extension rather than a generic synchronization level.
  - Acceptance: no example relies on implicit warp-synchronous communication.

- [x] **Slides 22 and 23: qualify NVIDIA-specific execution limits.**
  - [x] Label the block dimensions, warp-group size, scheduler count,
        resident-warp limit, and issue-rate statements as NVIDIA examples:
        H100 on slide 22 and A100 on slide 23.
  - [x] Add "query the device" guidance.
  - [ ] On slide 23, change the A100 maximum from 32 to **64 resident warps per
        SM**. Do not confuse this with the maximum number of resident blocks.
  - [x] Qualify the four-instructions-per-cycle scheduler ceiling: the slide now
        states that the actual number depends on the pipelines used and presents
        the schedulers as selecting ready warps, rather than as a generic FP32
        throughput calculation.
  - Acceptance: every numerical limit is correct for the named device and is
    clearly presented as a queryable, architecture-specific value.

### Rates, capacities, and derived quantities

- [x] **Slides 16, 17, and 31: audit every bandwidth value and unit.**
  - [x] Use `GB/s` and `TB/s` for byte rates and `Gb/s` only for genuine bit
        rates on slides 16 and 17.
  - [x] State the per-direction basis for NVLink, PCIe, and HDR InfiniBand on
        slide 17.
  - [x] On slide 16, replace `lane` with **NVLink** or **link**: one A100 NVLink
        provides 25 GB/s in each direction, or 50 GB/s aggregate
        bidirectionally. Four links connect each GPU pair in the four-GPU
        topology.
  - [x] On slide 31, state the directional basis in the GPU-GPU and PCIe column
        headings.
  - [x] Correct or redefine the H100 and B200 GPU-GPU column. NVIDIA specifies
        900 GB/s and 1,800 GB/s respectively as aggregate bidirectional NVLink
        bandwidth per GPU through an NVSwitch fabric. Those are 450 GB/s and
        900 GB/s one-way per-GPU aggregate rates, not automatically fixed
        pairwise rates of 150 GB/s and 300 GB/s each way. State whether the
        table reports per-link, direct pairwise, or per-GPU fabric injection
        bandwidth and use values consistent with that definition.
  - [x] Treat exact P100 and V100 topology revalidation as non-blocking legacy
        cleanup. These rows are not important to the current infrastructure;
        retain them only as approximate historical context or move them to an
        appendix in a later pass.
  - Acceptance: every bandwidth value can be traced to an official product or
    architecture source using the same definition.

- [x] **Slide 14: correct the `13,824 instructions/cycle` label.**
  - The slide now calculates NVIDIA A100 peak FP32 throughput as
    `64 FP32 lanes/SM × 2 FLOP/FMA × 108 SMs = 13,824 FLOP/cycle`, or about
    19.5 TFLOP/s.
  - The word *peak* supplies the necessary ideal-throughput qualifier for this
    introductory slide; the instructor can elaborate on occupancy, instruction
    readiness, and stalls.
  - The calculation is no longer derived from the number of warp schedulers.

- [x] **Slides 15 and 29: standardize capacity units.**
  - [x] Use the simpler `KB`, `MB`, and `GB` convention on the slides. This
        matches NVIDIA's documentation convention, where `KB` in the relevant
        capability tables denotes 1,024 bytes.
  - [x] Use uppercase `B` for byte capacities; slide 29 now uses `24 KB`.
  - [x] Correct the register rows. Slides 15 and 29 now state **64 K 32-bit
        registers per SM**, represented as `64 K × 4 B` = 256 KB, for every
        listed architecture.
  - [x] Correct the HGX B200 memory entries. The Blackwell tuning guide gives a
        maximum shared-memory capacity of **228 KB per SM** and a combined
        L1/texture/shared-memory capacity of **256 KB**. Slide 29 currently says
        256 KB and 256-288 KB respectively. Keep the table explicit about the
        unified, runtime-configurable carveout.
  - Acceptance: display units are simple and consistent, while the underlying
    byte count and the meaning of each capacity remain correct.

- [x] **Slide 13: verify the matrix equation and tile table visually.**
  - The native PowerPoint view has been verified and the A100/data-type scope is
    explicit.
  - The equation still fails in the automated PNG renderer used for this
    review, but that renderer-specific fidelity issue is accepted as
    non-blocking for the current deck.

## P1: establish a cross-vendor conceptual vocabulary

- [ ] **Revise slides 10 and 11 around generic compute concepts.**
  - Introduce a generic *compute unit* before SM and CU.
  - Introduce a generic *execution subgroup* before warp and wavefront.
  - Keep thread/work-item, block/work-group, and grid mappings together.

- [ ] **Add a compact NVIDIA/AMD terminology crosswalk.**

  | Concept | NVIDIA | AMD Instinct/CDNA |
  | --- | --- | --- |
  | Compute unit | Streaming Multiprocessor (SM) | Compute Unit (CU) |
  | Execution subgroup | Warp, 32 threads | Wavefront, 64 work-items |
  | Cooperative thread group | Thread block | Work-group / HIP block |
  | Local scratchpad | Shared memory | Local Data Share (LDS) |
  | Register resources | Register file | Scalar and vector registers (SGPR/VGPR) |
  | Matrix accelerator | Tensor Core | Matrix Core / MFMA |
  | Architecture target | Compute capability / `sm_*` | GFX target, for example `gfx942` |
  | GPU interconnect | NVLink/NVSwitch | Infinity Fabric/xGMI |
  | Collective library | NCCL | RCCL |

- [ ] **Use the crosswalk consistently throughout the deck.**
  - Replace unqualified `SM`, `warp`, `shared memory`, `Tensor Core`, and
    `NVLink` with the generic concept or a clearly labelled vendor example.
  - Acceptance: learners can follow the core narrative without knowing CUDA or
    HIP terminology in advance.

## P1: add the AMD Instinct hardware concepts that are not simple renamings

### Compute unit and matrix execution

- [ ] **Add an AMD CDNA Compute Unit example to the hardware section.**
  - Show scalar, vector, matrix, load/store, scheduling, register, LDS, and L1
    resources at a conceptual level.
  - Explain SGPR and VGPR pressure as separate occupancy constraints.
  - Avoid generation-specific lane and issue-rate counts on the conceptual
    slide.

- [ ] **Generalize slide 13 from Tensor Cores to matrix accelerators.**
  - Add AMD Matrix Cores and matrix fused multiply-add (MFMA).
  - Explain that supported types, tile shapes, accumulation types, sparsity,
    and throughput are architecture-dependent.
  - Retain NVIDIA Tensor Cores as one concrete implementation.

### Memory and packaging

- [ ] **Extend the memory hierarchy with AMD terminology.**
  - Registers: distinguish SGPRs, VGPRs, and matrix accumulator resources.
  - Local memory: map CUDA shared memory to AMD LDS.
  - Caches: include per-CU L1, distributed/partitioned L2, and Infinity Cache
    where present.
  - Global memory: retain HBM as the common concept.

- [ ] **Add chiplet topology as a first-class concept.**
  - Introduce XCDs and the distinction between package, physical compute die,
    logical partition, and runtime-visible device.
  - Explain that distributed cache and HBM resources can create NUMA-like
    locality within a GPU package.
  - Mention that partitioning can change how many logical devices one physical
    accelerator exposes.
  - Acceptance: no slide assumes one physical package always equals one runtime
    device or one uniform memory domain.

- [ ] **Replace the binary memory-management model on slide 20 with three
      cases.**
  1. Explicit host/device allocations and copies.
  2. Managed or unified virtual memory with page migration.
  3. Coherent shared physical memory, using MI300A as the example.
  - Contrast MI300X as a discrete GPU with MI300A as an APU.
  - Explain that migration, zero-copy, and coherence depend on hardware,
    operating-system, runtime, and configuration support.

### Multi-GPU topology

- [ ] **Generalize slides 16 and 17 beyond NVLink.**
  - Add AMD Infinity Fabric/xGMI as the Instinct intra-node counterpart.
  - Show PCIe and the network fabric as distinct communication levels.
  - Explain that connectivity may be direct, switched, fully connected, or
    non-uniform depending on the platform.
  - Make topology discovery and process/GPU affinity part of the conceptual
    message.

- [ ] **Add collective communication libraries to the topology discussion.**
  - Map NCCL to RCCL.
  - Mention MPI plus GPU-aware communication without turning the slide into a
    distributed-programming tutorial.
  - Explain why collective performance depends on topology and participant
    count, not only nominal link bandwidth.

## P1: revise the programming-model section

- [ ] **Reorganize slide 19 by abstraction level.**
  1. Frameworks and domain libraries.
  2. Portable libraries and programming abstractions such as Kokkos and SYCL.
  3. Directive models such as OpenMP target and OpenACC.
  4. Native kernel/runtime APIs: CUDA and HIP.
  - Distinguish the abstract programming model from the available compiler or
    backend implementation.
  - Do not imply equal maturity or feature coverage on every platform.

- [ ] **Add representative library mappings.**
  - Dense linear algebra: cuBLAS versus rocBLAS/hipBLAS.
  - FFT: cuFFT versus rocFFT/hipFFT.
  - Collectives: NCCL versus RCCL.
  - Deep learning primitives: cuDNN versus MIOpen, if this remains in scope.
  - Keep the point conceptual: prefer a tuned library over writing a kernel
    when the required operation already exists.

- [ ] **Revise slide 25 from "compute capability" to capability discovery.**
  - NVIDIA example: compute capability and `sm_*` compiler targets.
  - AMD example: GFX targets such as `gfx90a`, `gfx942`, and current successors.
  - Include runtime queries for subgroup width, limits, memory sizes, supported
    precision, atomics, peer access, and managed-memory features.
  - Acceptance: no portable recommendation branches only on a product name or
    architecture-version number when a feature query exists.

- [ ] **Make slide 26 performance guidance architecture-neutral.**
  - Retain divergence, arithmetic intensity, occupancy, locality, transfer
    minimization, alignment, and structure-of-arrays versus array-of-structures.
  - Describe coalescing as aligned, contiguous access by neighbouring subgroup
    lanes; do not present 128-byte transactions as universal.
  - Explain that launch size, tile size, vector width, and occupancy targets
    require measurement on the target architecture.

- [ ] **Expand slide 32's profiling call to action.**
  - NVIDIA: Nsight Systems and Nsight Compute.
  - AMD: ROCm Systems Profiler and ROCm Compute Profiler.
  - Record model, driver, compiler, runtime, launch configuration, precision,
    input shape, and transfer policy with benchmark results.

## P2: restructure the architecture-evolution section

- [ ] **Move detailed NVIDIA product tables to an appendix.**
  - Keep the core message that memory, compute, topology, precision, and
    synchronization capabilities evolve.
  - Avoid requiring learners to read dense specification tables during the
    conceptual introduction.

- [ ] **Add a separate AMD Instinct evolution table.**
  - Include representative CDNA generations, for example MI100, MI250X,
    MI300A/MI300X, and a current CDNA4 product.
  - Include architecture family, GFX target, wavefront width, memory capacity
    and bandwidth, matrix formats, and interconnect generation only when the
    values support a teaching point.
  - Record a source and a "last verified" date for every row.

- [ ] **Do not compare CUDA cores and AMD stream processors directly.**
  - Prefer application-relevant quantities such as supported precisions,
    theoretical operation rates with clearly stated assumptions, memory
    capacity/bandwidth, and measured kernel performance.
  - Keep vector and matrix throughput separate.

- [ ] **Add an architecture-change checklist to the final synthesis.**
  - Re-query limits and capabilities.
  - Revisit tile and launch sizes.
  - Recheck locality and topology assumptions.
  - Revalidate numerical tolerances and precision choices.
  - Re-profile rather than carrying tuning parameters forward unchanged.

## Validation and definition of done

### Content validation

- [ ] Verify NVIDIA claims against current NVIDIA programming guides and
      product documentation.
- [ ] Verify AMD claims against current AMD ROCm, HIP, Instinct, and CDNA
      documentation.
- [ ] Put a source block in the speaker notes for every non-trivial hardware
      diagram, number, table, and externally sourced claim.
- [ ] Record which model and form factor each numerical specification describes.
- [ ] Search the complete deck for unqualified vendor terms and either
      generalize or label them.

### Visual validation

- [ ] Render every slide to images and inspect each slide at full size.
- [ ] Verify equations, mathematical symbols, icons, and special characters in
      both PowerPoint and an exported PDF.
- [ ] Check tables for clipping, illegible text, and excessive density.
- [ ] Check that new cross-vendor comparisons use consistent visual hierarchy
      and do not imply false one-to-one hardware equivalence.
- [ ] Ensure vendor-neutral concepts remain visually primary and vendor examples
      remain secondary.

### Teaching validation

- [ ] Confirm that the deck still works as the shared introduction for the
      Python, OpenMP offload, and Kokkos trainings listed in `README.md`.
- [ ] Confirm that AMD coverage does not require prior HIP knowledge.
- [ ] Confirm that NVIDIA coverage does not require prior CUDA knowledge.
- [ ] Ask a reviewer familiar with AMD Instinct to check terminology, memory
      behaviour, topology, and performance guidance.
- [ ] Ask a reviewer familiar with modern NVIDIA GPUs to check the revised
      synchronization and bandwidth material.
- [ ] Run a timed presentation rehearsal and remove detail that belongs in the
      appendix or a vendor-specific course.

## Primary source starting points

- [AMD device hardware glossary](https://rocm.docs.amd.com/en/latest/reference/glossary/device-hardware.html)
- [HIP programming model](https://rocm.docs.amd.com/projects/HIP/en/latest/understand/programming_model.html)
- [HIP hardware implementation](https://rocm.docs.amd.com/projects/HIP/en/latest/understand/hardware_implementation.html)
- [HIP compiler targets](https://rocm.docs.amd.com/projects/HIP/en/latest/understand/compilers.html)
- [AMD CDNA 3 architecture white paper](https://www.amd.com/content/dam/amd/en/documents/instinct-tech-docs/white-papers/amd-cdna-3-white-paper.pdf)
- [ROCm programming guide](https://rocm.docs.amd.com/en/latest/how-to/programming_guide.html)
- [ROCm profiling and debugging tools](https://rocm.docs.amd.com/en/latest/components/profilers-and-debuggers.html)
- [NVIDIA CUDA programming guide](https://docs.nvidia.com/cuda/cuda-programming-guide/)
- [NVIDIA A100 specifications](https://www.nvidia.com/en-us/data-center/a100/)
