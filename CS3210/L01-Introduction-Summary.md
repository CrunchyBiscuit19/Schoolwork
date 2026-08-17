# CS3210 Lecture 1 — Introduction to Parallel Computinh

---

## 1. Why Parallel Computing Exists — the Power Wall

- Two ways to make code faster: **(1)** raise single-threaded performance (faster CPU), or **(2)** rewrite serial code as **parallel code** that uses multiple cores.
- Single-thread scaling relied on rising clock **frequency**. **Higher frequency ⇒ more heat ⇒ cannot cool** it.
- **~2004: the "Power Wall" (Power Density Wall).** Frequency, single-thread performance, and power all flatten; **only the core count keeps rising.**
- **Consequence (testable):** since ~2004 the way to get more performance is **parallel software**, not a faster single core.

**Definition — Parallel Computer:** a collection of processing elements that cooperate to solve problems quickly.

---

## 2. Serial vs Parallel Computing Model

**Serial:** problem → discrete series of instructions, executed **one at a time**, **one after another**, on a single processing unit.

**Parallel:**
1. Problem split into **m** discrete parts (**tasks**) solvable concurrently.
2. Each part broken into instructions (**i**).
3. Instructions from different parts execute **in parallel** on **p** processing units.

### Dependencies limit parallelism (key exam point)
Evaluate `a = (b+1) * (b-c)`:
- i₁: `+ b 1 t₁`
- i₂: `- b c t₂`
- i₃: `* t₁ t₂ a`

- **i₃ depends on i₁ and i₂** → cannot start until both finish. Randomly splitting instructions into parallel streams **does not work**; dependencies must be respected.
- **Sequential model** = single control flow.
- **Shared-memory parallel model** = multiple control flows using **fork/join**:
  - **fork** = create a parallel executing thread (run i₁ and i₂ concurrently).
  - **join** = block executing threads until **n** threads reach the same point (then run i₃).

---

## 3. Processing Units (PU) — von Neumann Model

- Each **core** = an independent **PU** that executes a linear stream of instructions.
- PU components: **Memory** (instructions + data), **Control Unit** (instruction counter), **ALU** (registers), **Input/Output**.

**Parallel computing** = simultaneous use of multiple PUs to solve a problem faster / solve a larger problem. PUs may be:
1. One processor, multiple cores
2. One computer, multiple processors
3. Multiple computers over a network
4. Combinations of the above

Ideal: partition the problem into enough **independent** parts.

---

## 4. Decompose → Schedule → Map

Pipeline: **Application Problem** → *decompose* → **Tasks** → *schedule & map* → **Physical Cores/Processors**

- **Decomposition:** one problem → many possible decompositions. Splits problem into **tasks**.
- **Granularity** = size of tasks (choosable). *(Fine-grained = many small tasks; coarse-grained = fewer large tasks.)*
- **Scheduling** = assignment of tasks to processes/threads; sets execution order. Can be **manual / static / dynamic**.
- **Mapping** = assignment of processes/threads to physical cores/processors.
- Tasks have **data** or **control dependencies** → these **impose the execution order** and constrain scheduling.

### Computational Model Attributes (5 — likely definition question)
| Attribute                     | Meaning                                                              |
| ----------------------------- | -------------------------------------------------------------------- |
| **Operation Mechanism**       | Primitive units of computation (data types & ops in instruction set) |
| **Data Mechanism**            | Definition of address spaces — how data are accessed/stored          |
| **Control Mechanism**         | Schedulable units — rules for partitioning & scheduling              |
| **Communication Mechanism**   | Modes/patterns of communication among parallel PEs                   |
| **Synchronization Mechanism** | Ensures information arrives at the right time                        |

### Memory organizations (know the pairing)
- **Shared-memory** → threads
- **Distributed-memory** → processes

---

## 5. Concurrency vs. Parallelism (classic exam distinction)

| **Concurrency**                                             | **Parallelism**                                         |
| ----------------------------------------------------------- | ------------------------------------------------------- |
| Tasks start/run/complete in **overlapping** time periods    | Tasks run **simultaneously, at the exact same instant** |
| May **not** be on the CPU at the same instant (interleaved) | Tasks actually **execute** at the same time             |
| Achievable on a **single core** (interleaving)              | Requires **multiple cores**                             |

One-liner: **concurrency = dealing with many things at once (overlap); parallelism = doing many things at once (simultaneous).**

---

## 6. Worked Example — Sum of n Numbers (know the step counts)

**Serial:** loop `sum += x` over n values → n additions, sequential.

**Parallel v1 — master core:**
- p cores (p < n), each computes a partial sum of **n/p** values in private `my_sum`.
- One **master core** receives every other core's partial sum and adds them → global sum.
- Cost of the final combine = **(p − 1) steps** (master adds one at a time).

**Parallel v2 — tree / pairwise reduction:**
- Cores add in pairs, then pairs of pairs, etc. → combine in **⌈log₂ p⌉ steps**.

### Comparison (memorize this table)
|             | 8 cores         | 1000 cores        |
| ----------- | --------------- | ----------------- |
| v1 (master) | 7 steps         | 999 steps         |
| v2 (tree)   | 3 steps         | 10 steps          |
| Improvement | factor of **2** | factor of **100** |

**Key insight:** the combine cost is **(p−1) vs log₂ p** — the *algorithm/structure* of the parallelization matters more as p grows. (Example values: n=24, p=8 → partials 8,19,7,15,7,13,12,14, global sum **95**.)

---

## 7. Parallel Performance

- Two perspectives: **Execution time** vs **Throughput**.
- **Parallel execution time = computation time + parallelization overheads.**
- **Parallelization overheads:** work distribution to processors, communication/synchronization, idle time, etc.

---

## 8. Parallel Programming Challenges

- Finding enough parallelism (**Amdahl's Law**)
- **Granularity**
- **Locality**
- **Load balance**
- Coordination and synchronization
- Debugging
- Performance modeling / monitoring

---

## 9. Reference — SI Prefixes (for FLOPS / performance calcs)

| name | prefix | multiplier |
| ---- | ------ | ---------- |
| exa  | E      | 10¹⁸       |
| peta | P      | 10¹⁵       |
| tera | T      | 10¹²       |
| giga | G      | 10⁹        |
| mega | M      | 10⁶        |
| kilo | K      | 10³        |

---

## Quick Revision Checklist
- [ ] Why single-thread scaling stopped → **Power/Density Wall (~2004)**, heat limits frequency.
- [ ] Dependencies limit parallelism; can't randomly split instructions. **fork/join** semantics.
- [ ] Serial (single control flow) vs parallel (multiple control flows).
- [ ] Decompose → Schedule → Map; definitions of **granularity, scheduling, mapping**.
- [ ] 5 computational model attributes.
- [ ] Shared-memory = threads; distributed-memory = processes.
- [ ] Concurrency (overlap, 1 core OK) ≠ Parallelism (simultaneous, needs multiple cores).
- [ ] Sum example: combine cost **(p−1) master vs log₂ p tree**; 8-core → 7 vs 3, 1000-core → 999 vs 10.
- [ ] Parallel time = computation + overheads; list the overheads.
- [ ] Challenges list, esp. **Amdahl's Law**, load balance, locality, granularity.
