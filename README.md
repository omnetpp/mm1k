# M/M/1/K Queue Validation

An OMNeT++ simulation of a finite-buffer M/M/1/K queue, validated against
closed-form analytical results.

## Model

```
Source --> Queue --> Sink
```

- **Source**: Poisson arrivals (exponential inter-arrival times)
- **Queue**: Exponential service, finite capacity K (including job in service), drops when full
- **Sink**: Absorbs completed jobs, records sojourn time

## Parameters

| Parameter | Value |
|-----------|-------|
| λ (arrival rate) | 0.8 |
| μ (service rate) | 1.0 |
| K (system capacity) | 5 |
| Simulation time | 100 000 s |
| Repetitions | 5 |

## Analytical Reference (M/M/1/K)

With ρ = λ/μ = 0.8:

| Metric | Formula | Value |
|--------|---------|-------|
| P(drop) | π_K = π_0 · ρ^K | 0.088819 |
| Throughput | λ · (1 − P_drop) | 0.728944 |
| Utilization | 1 − π_0 | 0.728944 |
| E[N] | Σ n·π_n | 1.868332 |
| E[T] | E[N] / throughput | 2.563065 |

## Building and Running (via opp_repl)

```python
from opp_repl import *

load_opp_file("/home/levy/workspace/mm1k/mm1k.opp")
p = get_simulation_project("mm1k")
build_project(simulation_project=p)
r = run_simulations(simulation_project=p, config_filter="Mm1kValidation")
```

## Files

| File | Description |
|------|-------------|
| `mm1k.opp` | opp_repl project descriptor |
| `Network.ned` | Network topology and module declarations |
| `Source.h` / `Source.cc` | Poisson arrival generator |
| `Queue.h` / `Queue.cc` | Finite-capacity queue with exponential service |
| `Sink.h` / `Sink.cc` | Job sink, records lifetime statistics |
| `omnetpp.ini` | Simulation configuration |

## Recorded Statistics

- **generated** — total jobs created (Source)
- **accepted** / **dropped** / **dropProbability** — queue admission (Queue)
- **busy:timeavg** — server utilization (Queue)
- **queueLength:timeavg** — mean number in system (Queue)
- **lifeTime:mean** — mean sojourn time (Sink)

## Validation Results

| Metric | Analytical | Simulated | Rel. Error |
|--------|-----------|-----------|-----------|
| Drop probability | 0.088819 | 0.089923 | 1.24% |
| Throughput | 0.728944 | 0.727422 | 0.21% |
| Utilization | 0.728944 | 0.727225 | 0.24% |
| E[N] | 1.868332 | 1.864657 | 0.20% |
| E[T] | 2.563065 | 2.563365 | 0.01% |

All errors are below 1.3%, consistent with statistical noise.
