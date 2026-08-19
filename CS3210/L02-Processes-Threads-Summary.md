# CS3210 Lecture 2 — Processes, Threads, and Synchronization
---

## 1. Program Parallelization — Who Does What

Pipeline: **Sequential Algorithm** → *decompose* → **Tasks** → *schedule* → **Processes/Threads** → *map* → **Physical Cores/Processors**

- **Decomposition** → done by the **programmer**.
- **Scheduling** (tasks → processes/threads) and **Mapping** (processes/threads → cores) → done by the **OS & libraries**.

---

## 2. Processes

**Definition:** an instance of a program in execution, identified by a **PID**.

A process comprises:
- Executable program (**PC** — program counter)
- Global data (+ OS resources: open files, network connections)
- Stack / heap
- Current register values (GPRs and special registers)

Key property: **own address space → exclusive access to its data.** Two processes exchanging data need **explicit communication (IPC)**.

### Memory layout of a process (know the 4 segments)
| Segment   | Holds                             |
| --------- | --------------------------------- |
| **Text**  | instructions                      |
| **Data**  | global variables                  |
| **Heap**  | dynamic allocation (grows up)     |
| **Stack** | function invocations (grows down) |

### Multi-programming / Multitasking
- Several processes at different stages of execution → needs **context switch** (switching between processes).
- **State of the suspended process must be saved → overhead.**
- 2 types of execution: **time-slicing (pseudo-parallelism)** on one core, vs **true parallel** execution on different resources.

### Creating a process in Unix — `fork()`
- `fork()` creates a new process P₂; `int exec(char *prog, char *argv[])` replaces the program image.
- **P₂ is an identical copy of P₁** at the time of the fork: works on a **copy** of P₁'s address space, and starts executing at the instruction **following the fork call**.
- P₂ gets its own PID; can branch to execute different statements.
- **`fork()` return value distinguishes parent/child:** returns **0 in the child**, and the **child's PID in the parent** — this is the exam-critical detail.
```c
int child_pid = fork();
if (child_pid == 0) {   // child
    printf("Child ... %d\n", getpid());
} else {                // parent
    printf("My child is %d\n", child_pid);
}
```

### Process termination
- Child calls **`exit(status)`**.
- Parent waits with **`wait`** or **`waitpid(pid)`**.

### Process state graph (5 states)
**New → Ready → Running → Terminated**, plus **Waiting**.
- Create Process: New → Ready
- Schedule / Unschedule: Ready ↔ Running
- Running → Waiting: on I/O, page fault, etc.
- Waiting → Ready: I/O done
- Running → Terminated: process exit

### Process ↔ OS interaction (know the sync/async split)
| **Exceptions**                                                              | **Interrupts**                                                           |
| --------------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| Caused by executing a **machine-level instruction**                         | Caused by **external events** (usually hardware: timer, mouse, keyboard) |
| e.g. overflow, underflow, divide-by-zero, illegal/mis-aligned memory access | Occur **independently** of program execution                             |
| **Synchronous** (due to program execution)                                  | **Asynchronous**                                                         |
| Run an **exception handler**                                                | Run an **interrupt handler**                                             |

### IPC (inter-process communication)
- **Shared memory** — must protect access with locks.
- **Message passing** — blocking/non-blocking, synchronous/asynchronous.
- **Unix-specific:** pipes & signals.

### Disadvantages of processes (why threads exist)
- Creating a process is **costly**: system-call overhead; all data structures must be allocated, initialized, and **copied**.
- **Context-switching overhead**.
- **Communication is costly** — goes through the OS.

---

## 3. Threads

**Definition:** an extension of the process model — a process may contain multiple independent control flows called **threads**. A thread defines a sequential execution stream within a process (**PC, SP, registers**).

- **Threads share the address space of the process** → all threads see the same values → **shared-memory architecture**.
- **Thread creation is faster than process creation** — no copy of the address space needed.
- Different threads of a process can run on **different cores** of a multicore processor.

### What is shared vs private (very common exam question)
| Shared by all threads (per process)           | Private per thread                                           |
| --------------------------------------------- | ------------------------------------------------------------ |
| Code (text), Data (globals), Heap, open files | **Runtime stack** (function stack frames), registers, PC, SP |

- Global variables and dynamically allocated objects are accessible by **any** thread.
- Each thread has a **private runtime stack**; it exists **iff the thread is active**.

### User-level vs Kernel threads
|              | **User-level threads**                                                                                                        | **Kernel threads**                                   |
| ------------ | ----------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------- |
| Managed by   | a **thread library**; OS unaware                                                                                              | the **OS** (OS-aware)                                |
| Advantage    | context switch is **fast**                                                                                                    | can exploit real parallelism; efficient on multicore |
| Disadvantage | OS can't map threads of same process to different resources → **no parallelism**; a **blocking I/O** in one thread blocks all | (avoids the user-level disadvantages)                |

### Thread-mapping models (know all three)
| Model            | Description                                                                                                                                                                            |
| ---------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Many-to-One**  | All user threads mapped to one process; **thread library** does scheduling                                                                                                             |
| **One-to-One**   | Each user thread → exactly one kernel thread; **OS** schedules & maps; no library scheduler                                                                                            |
| **Many-to-Many** | Library scheduler assigns user threads to a set of kernel threads; kernel scheduler maps kernel threads to resources; a user thread may map to a **different** kernel thread over time |

### POSIX threads (pthreads) — core API
- `pthread_create(&thread, NULL, function, (void*) arg)` — create a thread running `function`.
- `pthread_join(thread, NULL)` — wait for a thread to finish. **Without join, `exit` may terminate the process (and all threads) before they complete.**

### Choosing number of threads
Should suit the application's **degree of parallelism** and **available resources**, but **not too large** — keep thread creation/management/termination overhead low.

---

## 4. Synchronization — The Core Problem

- Threads cooperate: share resources, access shared data, coordinate execution.
- **Threads interleave arbitrarily and at different rates; scheduling is NOT under program control.**
- **Synchronization = restricting the possible interleavings of thread executions** for correctness. *(Discussed for threads, also applies to processes.)*

### The basic problem
If two concurrent threads read/modify/write a **shared variable**, access must be controlled to avoid erroneous behaviour.
- Mechanisms: **locks, mutexes, semaphores, monitors, condition variables**.
- Coordination patterns: **bounded buffer, producer-consumer**.

### Classic bank-account example (the canonical race)
```c
withdraw(account, amount) {
    balance = get_balance(account);
    balance = balance - amount;
    put_balance(account, balance);
    return balance;
}
```
Two threads withdrawing simultaneously can **interleave** (context switch between read and write) so that one update is lost — e.g. both read $1000, both write $900, final balance $900 instead of $800.

### Key definitions (memorize the distinctions)
- **Race condition:** multiple execution paths (threads/processes) running at the same time **finish in a different order than expected**; actual behavior differs from designed behavior due to timing/interleaving. **Critical** race conditions cause invalid execution/bugs.
- **Critical section:** a region of code that requires **mutual exclusion** — can't be entered by more than one thread at a time; others suspended until the first leaves. Typically accesses a shared resource.
- **Mutual exclusion:** only **one thread at a time** executes in the critical section; others **wait on entry**; when one leaves, another can enter.
- **Data race** (a *type* of race condition): **(1)** two concurrent threads access a shared memory location **without protection**, **AND (2)** at least one **modifies** it. Solution: synchronize access to any shared data structure (buffers, queues, lists, hash tables…).

---

## 5. Synchronization Mechanisms

Overview:
| Mechanism      | Notes                                                                                     |
| -------------- | ----------------------------------------------------------------------------------------- |
| **Locks**      | Primitive, minimal semantics; used to build others                                        |
| **Semaphores** | Basic, easy to grasp but hard to program with                                             |
| **Monitors**   | High-level, requires language support, operations implicit                                |
| **Messages**   | Communication + sync via data transfer over a channel; direct fit for distributed systems |

### Locks
- Two operations: **`acquire()`** (enter critical section), **`release()`** (leave).
- Between acquire/release the thread **holds the lock**; `acquire` doesn't return until any previous holder releases. Calls must be **paired**.
- Locks can **spin (spinlock)** or **block (mutex)**.

### Semaphores
- **Abstract data type providing mutual exclusion through atomic counters** (Dijkstra, "THE" system, 1968).
- Integer supporting two atomic operations:
  - **`Wait()`** = **P()** = **decrement**, block until semaphore is open (> 0).
  - **`Signal()`** = **V()** = **increment**, allow another thread to enter.
- **Safety property:** semaphore value is **always ≥ 0**.
- **It is undefined which blocked thread runs after a signal.**
- **As a mutex:** initialize to 1; wrap critical section in `wait(S) … signal(S)`.

**Semaphore drawbacks:** essentially shared global variables (accessible anywhere); **no connection between the semaphore and the data it controls**; used for both mutual exclusion AND coordination → hard to use, bug-prone.

### Barrier
A synchronization point: threads arriving at the barrier **wait until all** have arrived, then all proceed together.

---

## 6. Concurrency Failures (know the definitions & distinctions)

| Failure        | Definition                                                                                                                                                                                                                      |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Deadlock**   | Every process in a set is waiting for an event that can be caused **only by another process in the set** — none proceed. Arises when competing for limited resources or when incorrectly synchronized.                          |
| **Starvation** | A process is prevented from making **progress** because another holds the resource it needs; a **side-effect of the scheduling algorithm** (e.g. high-priority process always wins; one thread always beats another to a lock). |
| **Livelock**   | Like deadlock, but process states **constantly change** with respect to one another while **none progresses**; a special case of resource starvation.                                                                           |

### Four conditions for deadlock (ALL must hold simultaneously — classic exam question)
1. **Mutual exclusion** — at least one resource held in non-sharable mode.
2. **Hold and wait** — a process holds one resource while waiting for another.
3. **No pre-emption** — resources can't be forcibly taken (critical sections can't be aborted externally).
4. **Circular wait** — a cycle of processes [P1…Pn] where P1 waits for P2, …, Pn waits for P1.

### Four approaches to dealing with deadlock
1. **Ignore it** (hope it doesn't happen).
2. **Prevention** — make it impossible (break one of the 4 conditions).
3. **Avoidance** — control resource allocation (e.g. Banker's-style).
4. **Detection and recovery** — look for a cycle in dependencies.

---

## 7. Classical Synchronization Problems

List: **Producer-consumer** (infinite / finite buffer), **Readers-writers**, **Dining philosophers**, **Barbershop**, …

### Producer-consumer (infinite buffer)
Producers add items to a shared structure; consumers remove & process them.
- Variables: `mutex = Semaphore(1)`, `items = Semaphore(0)` (counts available items).

**Correct (improved) version — signal order matters:**
```
Producer:                     Consumer:
  event = waitForEvent()        items.wait()      # wait for an item
  mutex.wait()                  mutex.wait()
    buffer.add(event)             event = buffer.get()
  mutex.signal()                mutex.signal()
  items.signal()                event.process()
```
- **Improvement:** release `mutex` *before* `items.signal()` — minimizes time the mutex is held.
- **Broken version:** if the consumer does `mutex.wait()` **before** `items.wait()`, it can grab the mutex then block on `items` while holding the mutex → **deadlock** (producer can never add). ⚠ **Never hold a mutex while blocking on another semaphore.**

### Producer-consumer with finite buffer
Add a third semaphore **`spaces = Semaphore(N)`** (empty slots):
```
Producer:                     Consumer:
  event = waitForEvent()        items.wait()
  spaces.wait()   # NEW         mutex.wait()
  mutex.wait()                    event = buffer.get()
    buffer.add(event)           mutex.signal()
  mutex.signal()                spaces.signal()   # NEW
  items.signal()                event.process()
```
- Producer waits on `spaces` (blocks when buffer full); consumer signals `spaces` after removing.

### Readers-writers
Multiple readers may read concurrently, but a writer needs exclusive access.
- **Lightswitch** pattern: first reader "locks" `roomEmpty` (turns light on); last reader "unlocks" it (turns light off). Uses an internal `mutex` + counter.
- **Basic version starves writers** → fix with a **`turnstile = Semaphore(1)`** so a waiting writer blocks new readers (no-starve readers-writers).
- **Readers-writers with priorities:** use `writeSwitch`/`readSwitch` lightswitches with `noReaders`/`noWriters` semaphores to give writers priority.

---

## 8. Lock Implementation (spinlocks & atomicity)

### Naive spinlock — and why it's broken
```c
struct lock { int held = 0; }
void acquire(lock) { while (lock->held); lock->held = 1; }  // busy-wait
void release(lock) { lock->held = 0; }
```
- Called a **spinlock** (thread spins waiting).
- **Doesn't work:** a **context switch between the `while` test and `held = 1`** lets two threads both see the lock free and both acquire it → race condition. **The acquire/release implementation itself is a critical section.**

### Fix: atomicity
- **acquire/release must be atomic** — an **atomic operation** executes as though it can't be interrupted ("all or nothing").
- Need **hardware help**:
  - **Atomic instructions** (e.g. **test-and-set**).
  - **Disable/enable interrupts** (prevents context switches).

### Test-and-set (atomic instruction)
Semantics: record old value, set the value, return the old value — **executed atomically by hardware**.
```c
bool test_and_set(bool *flag) {
    bool old = *flag;
    *flag = True;
    return old;
}
```
Correct spinlock built on it:
```c
void acquire(lock) { while (test_and_set(&lock->held)); }
void release(lock) { lock->held = 0; }
```

### Problems with spinlocks
- **Wasteful:** a spinning thread burns CPU; on a uniprocessor the lock holder can't even run to release it.
- The holder gave up the CPU via **yield/sleep** or an **involuntary context switch**.

### Higher-level synchronization
- **All synchronization requires atomicity.**
- Two common high-level mechanisms:
  - **Semaphores:** binary (mutex) and counting.
  - **Monitors:** built on mutex + condition-variable locks as primitives.

---

## Quick Revision Checklist
- [ ] Process = program in execution (PID); own address space; 4 memory segments (text/data/heap/stack).
- [ ] `fork()` returns **0 to child, child PID to parent**; child is a copy resuming after the fork.
- [ ] Process states: New/Ready/Running/Waiting/Terminated + transitions.
- [ ] Exceptions = synchronous (instruction); Interrupts = asynchronous (external).
- [ ] Threads share text/data/heap/files; **private stack + registers**; faster to create than processes.
- [ ] User-level (fast, no true parallelism, blocks on I/O) vs kernel threads; Many-to-One / One-to-One / Many-to-Many.
- [ ] Race condition vs data race vs critical section vs mutual exclusion — define each.
- [ ] Semaphore: `wait`/P = decrement+block, `signal`/V = increment; value always ≥ 0; undefined which thread wakes.
- [ ] Deadlock 4 conditions (mutex, hold-and-wait, no-preemption, circular wait) + 4 handling approaches.
- [ ] Deadlock vs starvation vs livelock.
- [ ] Producer-consumer: `mutex`, `items`, (finite:) `spaces`; **never hold mutex while blocking** → broken version deadlocks.
- [ ] Readers-writers: lightswitch, turnstile to avoid writer starvation, priority variants.
- [ ] Naive spinlock races between test and set → need atomic **test-and-set** (or disable interrupts).
