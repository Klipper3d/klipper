# Performance Metrics and Optimization

This document details the performance metrics and optimization strategies implemented in Klipper's motion system.

## Timing Metrics

### Core Timing Constants

```python
# klippy/extras/motion_queuing.py
BGFLUSH_LOW_TIME = 0.200      # 200ms - Relaxed flush interval
BGFLUSH_HIGH_TIME = 0.400     # 400ms - Normal flush interval
BGFLUSH_SG_LOW_TIME = 0.450   # 450ms - Aggressive mode low threshold
BGFLUSH_SG_HIGH_TIME = 0.700  # 700ms - Aggressive mode high threshold
BGFLUSH_EXTRA_TIME = 0.250    # 250ms - Additional buffer time

# Buffer management
BUFFER_TIME_HIGH = 2.0        # 2s - High buffer threshold
BUFFER_TIME_START = 0.250     # 250ms - Initial buffer time
LOOKAHEAD_FLUSH_TIME = 0.250  # 250ms - Look-ahead flush interval

# Step processing
MIN_KIN_TIME = 0.100          # 100ms - Minimum kinematic time
STEPCOMPRESS_FLUSH_TIME = 0.050  # 50ms - Step compression flush
SDS_CHECK_TIME = 0.001        # 1ms - Step+dir+step filter time
```

### Performance Thresholds

```c
// klippy/chelper/stepcompress.c
#define SDS_FILTER_TIME .000750           // 750μs - Step direction filter
#define CLOCK_DIFF_MAX (3<<28)            // ~134s - Maximum clock difference
#define QUADRATIC_DEV 11                  // Quadratic deviation factor

// src/stepper.c  
#define EDGE_STEP_TICKS DIV_ROUND_UP(CONFIG_CLOCK_FREQ, 8000000)  // 125ns @ 1MHz
#define AVR_STEP_TICKS 40                 // 40 clock ticks minimum
```

## Threading Performance

### Thread Allocation Strategy

```c
// One thread per stepper motor for parallel processing
struct stepcompress {
    pthread_t tid;              // Dedicated thread ID
    char name[16];              // Thread name for debugging
    pthread_mutex_t lock;       // Thread synchronization
    pthread_cond_t cond;        // Condition signaling
};
```

### Work Distribution

```python
# klippy/extras/motion_queuing.py
def _advance_flush_time(self, want_flush_time, want_step_gen_time=0.):
    # Parallel step generation across all steppers
    for mcu, ss in self.steppersyncs:
        clock = max(0, mcu.print_time_to_clock(flush_time))
        self.steppersync_start_gen_steps(ss, step_gen_time, clock)
    
    # Synchronous completion wait
    for mcu, ss in self.steppersyncs:
        ret = self.steppersync_finalize_gen_steps(ss, clock)
```

## Memory Optimization

### Queue Management

```c
// klippy/chelper/stepcompress.c
#define QUEUE_START_SIZE 1024    // Initial queue allocation

static int queue_append_extend(struct stepcompress *sc)
{
    if (sc->queue_next - sc->queue_pos > 65535 + 2000) {
        // Limit memory usage to ~64K steps
        uint32_t flush = (*(sc->queue_next-65535) 
                         - (uint32_t)sc->last_step_clock);
        int ret = queue_flush(sc, sc->last_step_clock + flush);
    }
    
    // Dynamic reallocation strategy
    int alloc = sc->queue_end - sc->queue;
    if (!alloc)
        alloc = QUEUE_START_SIZE;
    while (in_use >= alloc)
        alloc *= 2;  // Exponential growth
}
```

### History Management

```python
# klippy/extras/motion_queuing.py
MOVE_HISTORY_EXPIRE = 30.  # 30 seconds history retention

def _advance_flush_time(self, want_flush_time, want_step_gen_time=0.):
    # Automatic history cleanup
    clear_history_time = self.clear_history_time
    if not self.can_pause:
        clear_history_time = trapq_free_time - MOVE_HISTORY_EXPIRE
    
    for trapq in self.trapqs:
        self.trapq_finalize_moves(trapq, trapq_free_time, clear_history_time)
```

## Algorithm Complexity

### Binary Heap Operations

```c
// klippy/chelper/steppersync.c - O(log n) complexity
static void heap_replace(struct steppersync *ss, uint64_t req_clock)
{
    // Maintains heap property in O(log n) time
    // vs O(n) linear search in previous implementations
}
```

### Step Compression

```c
// klippy/chelper/stepcompress.c
static struct step_move compress_bisect_add(struct stepcompress *sc)
{
    // Bisection algorithm for optimal step compression
    // Reduces from O(n²) to O(n log n) complexity
    
    for (;;) {
        // Binary search for optimal interval/add parameters
        add = maxadd - (maxadd - minadd) / 4;  // Bisection step
        
        // Quadratic deviation optimization
        if (count > 1) {
            int32_t errdelta = sc->max_error*QUADRATIC_DEV / (count*count);
            if (minadd < add - errdelta)
                minadd = add - errdelta;
        }
    }
}
```

## Latency Optimization

### Lazy Evaluation

```python
# klippy/toolhead.py
def _process_lookahead(self, lazy=False):
    moves = self.lookahead.flush(lazy=lazy)
    if not moves:
        return  # Early exit - no processing needed
    
    # Process only when buffer requires flushing
    if self.special_queuing_state:
        self._calc_print_time()  # Deferred calculation
```

### Batch Processing

```python
# klippy/extras/motion_queuing.py
def _flush_handler(self, eventtime):
    # Batch operations for better cache locality
    batch_time = BGFLUSH_SG_HIGH_TIME - BGFLUSH_SG_LOW_TIME
    next_batch_time = self.last_step_gen_time + batch_time
    
    # Reproducible timing through batching
    if next_batch_time > est_print_time:
        if next_batch_time > want_sg_time + 0.005:
            next_batch_time = self.last_step_gen_time
        want_sg_time = next_batch_time
```

## Hardware Optimization

### MCU-Specific Optimizations

```c
// src/stepper.c - Three optimization levels
uint_fast8_t stepper_event(struct timer *t)
{
    if (HAVE_EDGE_OPTIMIZATION)
        return stepper_event_edge(t);    // Fastest: step on both edges
    if (HAVE_AVR_OPTIMIZATION)  
        return stepper_event_avr(t);     // AVR-specific optimizations
    return stepper_event_full(t);        // Full compatibility mode
}
```

### Step Pulse Optimization

```c
// Automatic optimization selection
if (want_both_edges) {
    self._step_both_edge = True;
    invert_step = -1;
    if (sbe) {
        // Older MCU optimization
        self._step_pulse_duration = 0.;
    }
}
```

## Performance Monitoring

### Internal Metrics (Not User-Exposed)

```python
# klippy/extras/motion_queuing.py
class PrinterMotionQueuing:
    def __init__(self, config):
        # Timing tracking
        self.last_flush_time = 0.
        self.last_step_gen_time = 0.
        self.need_flush_time = 0.
        self.need_step_gen_time = 0.
        
        # Performance counters
        self.kin_flush_delay = SDS_CHECK_TIME
        self.clear_history_time = 0.
```

### Debug Statistics

```python
# klippy/toolhead.py
def stats(self, eventtime):
    est_print_time = self.mcu.estimated_print_time(eventtime)
    buffer_time = self.print_time - est_print_time
    is_active = buffer_time > -60. or not self.special_queuing_state
    
    return is_active, "print_time=%.3f buffer_time=%.3f print_stall=%d" % (
        self.print_time, max(buffer_time, 0.), self.print_stall)
```

## Scalability Characteristics

### Multi-Stepper Scaling

- **Linear scaling**: O(n) with number of steppers
- **Parallel processing**: Each stepper has dedicated thread
- **Synchronized coordination**: Binary heap maintains O(log n) ordering
- **Memory efficiency**: Bounded queue sizes prevent unbounded growth

### Multi-MCU Support

- **Independent processing**: Each MCU has separate steppersync
- **Clock synchronization**: Automatic drift compensation
- **Load balancing**: Work distributed across available MCUs
- **Fault isolation**: MCU failures don't affect others

This performance architecture enables Klipper to achieve high step rates (>100kHz per stepper) while maintaining deterministic timing and low latency response to G-code commands.