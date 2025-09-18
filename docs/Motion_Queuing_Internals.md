# Motion Queuing Internals

This document describes the internal implementation details of Klipper's motion queuing system that are not covered in the main Code_Overview.md document.

## Binary Heap Optimization

The **steppersync.c** module uses a binary heap algorithm to efficiently manage command ordering across multiple steppers.

### Implementation Details

```c
// klippy/chelper/steppersync.c
static void heap_replace(struct steppersync *ss, uint64_t req_clock)
{
    uint64_t *mc = ss->move_clocks;
    int nmc = ss->num_move_clocks, pos = 0;
    for (;;) {
        int child1_pos = 2*pos+1, child2_pos = 2*pos+2;
        uint64_t child2_clock = child2_pos < nmc ? mc[child2_pos] : UINT64_MAX;
        uint64_t child1_clock = child1_pos < nmc ? mc[child1_pos] : UINT64_MAX;
        if (req_clock <= child1_clock && req_clock <= child2_clock) {
            mc[pos] = req_clock;
            break;
        }
        // Maintain heap property by bubbling down
        if (child1_clock < child2_clock) {
            mc[pos] = child1_clock;
            pos = child1_pos;
        } else {
            mc[pos] = child2_clock;
            pos = child2_pos;
        }
    }
}
```

### Purpose
- **O(log n) complexity** for command ordering vs O(n) linear search
- **Synchronizes multiple steppers** by maintaining temporal order
- **Prevents starvation** by ensuring fair scheduling across steppers

## Adaptive Flush System

The motion queuing system implements adaptive flushing based on printer activity levels.

### Timing Constants

```python
# klippy/extras/motion_queuing.py
BGFLUSH_LOW_TIME = 0.200      # Relaxed flushing interval
BGFLUSH_HIGH_TIME = 0.400     # Normal flushing interval  
BGFLUSH_SG_LOW_TIME = 0.450   # Aggressive stepping low threshold
BGFLUSH_SG_HIGH_TIME = 0.700  # Aggressive stepping high threshold
BGFLUSH_EXTRA_TIME = 0.250    # Additional buffer time
```

### Adaptive Algorithm

```python
def _flush_handler(self, eventtime):
    est_print_time = self.mcu.estimated_print_time(eventtime)
    aggr_sg_time = self.need_step_gen_time - 2.*self.kin_flush_delay
    
    if self.last_step_gen_time < aggr_sg_time:
        # AGGRESSIVE MODE: Printer is actively stepping
        want_sg_time = est_print_time + BGFLUSH_SG_HIGH_TIME
        batch_time = BGFLUSH_SG_HIGH_TIME - BGFLUSH_SG_LOW_TIME
        next_batch_time = self.last_step_gen_time + batch_time
        
        # Batch processing for reproducibility
        if next_batch_time > est_print_time:
            if next_batch_time > want_sg_time + 0.005:
                next_batch_time = self.last_step_gen_time
            want_sg_time = next_batch_time
    else:
        # RELAXED MODE: Printer is idle or finishing moves
        want_flush_time = est_print_time + BGFLUSH_HIGH_TIME
        max_flush_time = self.need_flush_time + BGFLUSH_EXTRA_TIME
        want_flush_time = min(want_flush_time, max_flush_time)
```

### Benefits
- **Reduces latency** during active printing
- **Conserves CPU** during idle periods  
- **Improves reproducibility** through batched processing
- **Prevents buffer overflow** with adaptive thresholds

## Lazy Evaluation in Look-Ahead

The look-ahead queue implements lazy evaluation to optimize performance.

### Implementation

```python
# klippy/toolhead.py
def _process_lookahead(self, lazy=False):
    moves = self.lookahead.flush(lazy=lazy)
    if not moves:
        return
    # Process only when necessary
    
def flush(self, lazy=False):
    update_flush_count = lazy
    # Skip expensive calculations when lazy=True
    if update_flush_count and peak_cruise_v2:
        flush_count = i
        update_flush_count = False
```

### Optimization Strategy
- **Deferred processing**: Only calculate when moves are needed
- **Partial evaluation**: Process subset of moves when possible
- **Early termination**: Stop processing when sufficient moves queued
- **Reduced overhead**: Skip unnecessary junction calculations

## Threading Synchronization Details

The system uses POSIX threads with careful synchronization primitives.

### Per-Stepper Threading

```c
// klippy/chelper/stepcompress.c
struct stepcompress {
    pthread_t tid;                    // Background thread ID
    pthread_mutex_t lock;             // Protects shared variables
    pthread_cond_t cond;              // Condition variable for signaling
    int have_work;                    // Work availability flag
    double bg_gen_steps_time;         // Background generation time
    uint64_t bg_flush_clock;          // Background flush clock
    int32_t bg_result;                // Background operation result
};
```

### Thread Lifecycle

```c
static void *sc_background_thread(void *data)
{
    struct stepcompress *sc = data;
    set_thread_name(sc->name);
    
    pthread_mutex_lock(&sc->lock);
    for (;;) {
        if (!sc->have_work) {
            // Wait for work signal
            pthread_cond_wait(&sc->cond, &sc->lock);
            continue;
        }
        if (sc->have_work < 0)
            break; // Exit request
            
        // Process step generation
        sc->bg_result = stepcompress_generate_steps(sc, 
                       sc->bg_gen_steps_time, sc->bg_flush_clock);
        sc->have_work = 0;
        pthread_cond_signal(&sc->cond); // Signal completion
    }
    pthread_mutex_unlock(&sc->lock);
    return NULL;
}
```

### Synchronization Protocol

1. **Work Assignment**: Main thread sets work parameters and signals
2. **Background Processing**: Worker thread processes steps independently  
3. **Completion Signaling**: Worker signals completion via condition variable
4. **Result Collection**: Main thread collects results synchronously

### Thread Safety Measures
- **Mutex protection** for all shared variables
- **Condition variables** for efficient waiting
- **Atomic work flags** to prevent race conditions
- **Proper cleanup** on thread termination

## Performance Metrics

### Timing Measurements

```python
# Internal timing tracking (not exposed to users)
self.last_flush_time = 0.        # Last flush operation timestamp
self.last_step_gen_time = 0.     # Last step generation timestamp  
self.need_flush_time = 0.        # Next required flush time
self.need_step_gen_time = 0.     # Next required step generation time
self.kin_flush_delay = SDS_CHECK_TIME  # Kinematic flush delay
```

### Step+Dir+Step Filter

```c
// klippy/chelper/stepcompress.c
#define SDS_FILTER_TIME .000750  // 750 microseconds

int stepcompress_append(struct stepcompress *sc, int sdir, 
                       double print_time, double step_time)
{
    if (sc->next_step_clock) {
        if (unlikely(sdir != sc->next_step_dir)) {
            double diff = (int64_t)(step_clock - sc->next_step_clock);
            if (diff < SDS_FILTER_TIME * sc->mcu_freq) {
                // Rollback to avoid rapid step+dir+step sequence
                sc->next_step_clock = 0;
                sc->next_step_dir = sdir;
                return 0;
            }
        }
    }
}
```

### Buffer Management

```python
# Motion queue buffer timing constants
MIN_KIN_TIME = 0.100             # Minimum kinematic processing time
STEPCOMPRESS_FLUSH_TIME = 0.050  # Step compression flush interval
MOVE_HISTORY_EXPIRE = 30.        # Move history retention time
```

## Integration Points

### ToolHead Integration

The motion queuing system integrates with ToolHead through callback mechanisms:

```python
# klippy/toolhead.py
def __init__(self, config):
    self.motion_queuing = self.printer.load_object(config, 'motion_queuing')
    self.motion_queuing.register_flush_callback(self._handle_step_flush,
                                                can_add_trapq=True)
```

### MCU Coordination

Multiple MCUs are coordinated through synchronized step generation:

```python
def setup_mcu_movequeue(self, mcu, serialqueue, move_count):
    # Create steppersync for each MCU
    ss = ffi_lib.steppersync_alloc(serialqueue, stepqueues, 
                                  len(stepqueues), move_count)
    self.steppersyncs.append((mcu, ss))
```

This internal architecture enables Klipper's high-performance, multi-stepper coordination while maintaining deterministic timing and efficient resource utilization.