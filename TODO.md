# TODO: multi-GPU foundations

## Purpose

Extend `gpu_programming.pptx` with a concise, framework-neutral introduction to
multi-GPU programming.  This presentation is shared by:

- Python on GPUs;
- OpenMP GPU offloading; and
- Heterogeneous programming with Kokkos.

The shared presentation should establish the durable concepts and vocabulary.
Framework-specific syntax, complete examples, and exercises should remain in the
individual trainings.

## Intended learning outcome

After this section, participants should be able to explain why effective
multi-GPU execution requires explicit decomposition, placement, communication,
synchronization, and measurement.

They are not expected to be able to implement a complete multi-GPU application
from this presentation alone.

## Scope boundary

### Include here

- GPUs as separate memory and execution domains;
- common ways to partition work and data;
- device selection, ownership, and process-to-device mapping;
- point-to-point communication and collective operations;
- the effect of hardware topology on communication cost;
- load balance, synchronization, and scaling limits; and
- the distinction between single-node and multi-node execution.

### Leave to the downstream trainings

- JAX device meshes, sharding APIs, and multi-process initialization;
- PyTorch distributed training and Lightning strategies;
- OpenMP directives, runtime calls, and compiler-specific behavior;
- Kokkos execution-space construction and MPI integration;
- CUDA, HIP, NCCL, or MPI API details; and
- substantial multi-GPU programming exercises.

## Priority 0: review and correct the current material

- [ ] Review slides 16 and 17 as the starting point for the new section.
- [ ] Present the number of GPUs per node as an example rather than “typically
      4 GPUs/node”; configurations and interconnect topologies vary widely.
- [ ] Check all bandwidth units, especially the distinction between `GB/s` and
      `Gb/s`.
- [ ] Label hardware-specific bandwidth figures with the device/interconnect
      generation and an approximate date.
- [ ] Prefer a relative bandwidth/latency hierarchy over figures that will age
      quickly.
- [ ] Ensure that “global memory” is not described in a way that suggests it is
      global across all GPUs.
- [ ] Check that the existing device-to-device transfer and device-enumeration
      slides use terminology consistent with the new section.

## Priority 1: add the conceptual multi-GPU section

Place this section after the hardware topology and bandwidth discussion and the
“Programming model” divider, before framework-specific programming-system
examples.  Target approximately 10–15 minutes in the core presentation.

### Slide 1: Multiple GPUs are separate memory domains

- [ ] Show that each GPU has its own memory, execution queues, and locality.
- [ ] Distinguish a unified virtual address space or managed memory from uniform
      shared-memory performance.
- [ ] Explain that data placement is part of both correctness and performance.
- [ ] Introduce device identity and ownership without using a framework API.

### Slide 2: Choose a decomposition

- [ ] Compare independent tasks or ensemble/parameter-sweep parallelism.
- [ ] Introduce data parallelism: replicated computation with partitioned input.
- [ ] Introduce domain decomposition: partitioned state with boundary or halo
      exchange.
- [ ] Mention model and pipeline parallelism as advanced alternatives, without
      expanding them into a deep-learning lesson.
- [ ] State that the correct decomposition depends on data dependencies and the
      compute-to-communication ratio.

### Slide 3: Communication is part of the algorithm

- [ ] Contrast peer-to-peer transfer with staging data through host memory.
- [ ] Introduce broadcast, reduce, all-reduce, all-gather, and reduce-scatter by
      purpose rather than API.
- [ ] Relate halo exchange to point-to-point communication.
- [ ] Show where synchronization occurs and why excessive synchronization can
      eliminate a speed-up.
- [ ] Mention communication/computation overlap, while avoiding the impression
      that overlap is automatic.

### Slide 4: Map processes and devices deliberately

- [ ] Compare one process per GPU with one process controlling multiple GPUs.
- [ ] Define global rank, node-local rank, and device index at a conceptual
      level.
- [ ] Explain device affinity and accidental GPU oversubscription.
- [ ] Show how the same mapping idea appears in distributed Python, MPI plus
      Kokkos, and host-controlled OpenMP offload.
- [ ] Explain that topology-aware mapping can reduce communication cost.

### Slide 5: Scaling has conditions

- [ ] Distinguish strong scaling from weak scaling.
- [ ] Cover load imbalance, communication volume, latency, synchronization, and
      per-GPU workload size.
- [ ] Explain why adding GPUs may make a small problem slower.
- [ ] Include speed-up and parallel-efficiency definitions, using a small,
      readable example.
- [ ] Mention that parallel reduction order can change floating-point results;
      reproducibility should use tolerances and recorded configurations rather
      than assuming bitwise identity.

## Priority 2: improve the topology narrative

- [ ] Add a simple topology diagram showing GPU-local memory, GPU-to-GPU links,
      the CPU/NUMA domains, PCIe, and the network interface.
- [ ] Use the diagram to distinguish locality from mere addressability.
- [ ] Explain that NVLink, NVSwitch, PCIe, and network paths are not
      interchangeable.
- [ ] Distinguish communication within one node from communication between
      nodes.
- [ ] Present GPUDirect-style paths as possible optimizations, not assumptions
      that hold on every system.
- [ ] Add an instruction to inspect the actual node topology and scheduler GPU
      allocation before interpreting performance results.

## Priority 3: add concise hand-offs to the downstream trainings

Keep these as brief “what comes next” references, not API tutorials.

- [ ] Python on GPUs: connect decomposition and placement to JAX meshes,
      sharding, and collective operations.
- [ ] Machine learning with Python: connect data parallelism and all-reduce to
      distributed PyTorch/Lightning training.
- [ ] OpenMP GPU offloading: connect device identity and data environments to
      the `device` clause and runtime device selection.
- [ ] Heterogeneous programming with Kokkos: connect rank/device mapping and
      ownership to execution-space instances and MPI integration.
- [ ] Use the same terms for device, rank, process, partition, collective, and
      topology in all four trainings.
- [ ] Link to the relevant downstream lesson instead of duplicating its code.

## Optional material

Place these slides in an appendix or mark them as optional so that a
single-GPU-oriented delivery is not forced into a distributed-computing detour.

- [ ] Worked strong- and weak-scaling plots.
- [ ] A ring-all-reduce diagram and its bandwidth/latency trade-offs.
- [ ] Two-dimensional domain decomposition and halo exchange.
- [ ] A comparison of replicated, sharded, and pipelined computation.
- [ ] Multi-node rank and device mapping through a batch scheduler.
- [ ] Failure modes: out-of-memory on one rank, mismatched collectives,
      oversubscription, skewed partitions, and hidden host staging.

## Teaching and visual-design checks

- [ ] Use one central idea per slide and avoid dense framework comparison
      tables.
- [ ] Prefer diagrams that reveal ownership, data movement, or synchronization
      over decorative GPU imagery.
- [ ] Use consistent colors for host memory, device memory, computation, and
      communication across the section.
- [ ] Make topology arrows directional where direction matters and label
      synchronization separately from data movement.
- [ ] Keep the mandatory path understandable to participants who have only used
      one GPU.
- [ ] Clearly mark advanced or multi-node content as optional.
- [ ] Include alt text or an equivalent textual explanation for each substantive
      diagram.

## Technical references to verify during authoring

- [ ] CUDA Programming Guide: multi-GPU systems and memory locality.
- [ ] OpenMP specification: device data environments and the `device` clause.
- [ ] Kokkos documentation: multi-GPU execution spaces and MPI-rank device
      mapping.
- [ ] NCCL documentation: collective-operation semantics.
- [ ] Current documentation for any hardware bandwidth figures used in the
      slides.

Avoid treating a vendor programming guide as the definition of the general
model.  Use it to verify concrete behavior and terminology, then present the
portable concept.

## Validation

- [ ] Review the section once from the perspective of each downstream training:
      Python/JAX, machine learning, OpenMP, and Kokkos.
- [ ] Confirm that every framework-specific concept has a clear shared concept
      to refer back to.
- [ ] Confirm that the shared deck does not duplicate downstream code or command
      sequences.
- [ ] Verify all factual claims and hardware examples against current primary
      documentation.
- [ ] Check slide readability in slideshow mode and in the exported PDF.
- [ ] Run a timed delivery; keep the mandatory addition within 10–15 minutes.
- [ ] Ask at least one trainer from a non-Python course to review the terminology
      and examples.
- [ ] Recheck links and references after the final presentation update.

## Completion criteria

- [ ] The core presentation explains decomposition, placement, communication,
      synchronization, and scaling without requiring a particular framework.
- [ ] The new core section can be delivered in at most 15 minutes.
- [ ] Existing bandwidth and topology claims are accurate, sourced, and clearly
      qualified.
- [ ] Each downstream training has an explicit hand-off from the shared
      concepts to its own implementation material.
- [ ] Optional advanced slides can be omitted without breaking the narrative.
- [ ] The updated deck has been technically reviewed and presentation-tested.
