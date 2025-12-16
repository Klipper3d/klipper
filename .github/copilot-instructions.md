# Klipper Coding Guidelines for AI Agents

Klipper is a 3D printer firmware combining a host computer (Klippy/Python) with microcontroller code (C). Understanding the architectural split and communication patterns is critical.

## Architecture: Host-MCU Duality

**Key Mental Model**: Klipper splits into two execution contexts:
- **Host (Klippy)**: Python running on Raspberry Pi/Linux - handles G-code parsing, motion planning, state management
- **Microcontroller (MCU)**: C firmware on AVR/STM32/RP2040 - handles real-time stepper pulses, GPIO, timing with microsecond precision

These communicate bidirectionally via serial protocol (defined in [Protocol.md](../docs/Protocol.md)). The host is the "brain" (high-level logic), the MCU is the "hand" (precise timing).

**Key files**:
- [klippy/klippy.py](../klippy/klippy.py): Main host entry, printer object orchestration
- [klippy/mcu.py](../klippy/mcu.py): Host-MCU communication layer
- [src/](../src/): MCU C code (architecture-specific in subdirs: avr/, stm32/, etc.)

## Component Lifecycle & Event Handlers

Klipper uses an **event-driven reactor pattern** (not typical OOP). Startup follows:

1. **Config Loading** → Parse printer config file, instantiate modules
2. **Object Registration** → Modules call `printer.add_object(name, obj)` 
3. **klippy:connect** event → All registered handlers called (MCU discovery, handshakes)
4. **klippy:ready** event → System fully operational, user G-code accepted

**Register handlers with**: `printer.register_event_handler("event:name", callback)`

Examples:
- `"klippy:connect"` - MCU identification, hardware setup
- `"klippy:ready"` - Final initialization, webhooks readiness
- `"gcode:command_error"` - Error handling
- Domain-specific: `"homing:homing_move_begin"`, `"probe:before_calibrate"`

See [klippy/klippy.py](../klippy/klippy.py), [klippy/probe.py](../klippy/extras/pr[def]

## Module Loading Pattern

Host modules ("extras") dynamically load from [klippy/extras/](../klippy/extras/). Three load patterns:

1. **Single instance**: `load_config(config)` - module name is config section name
   - Example: `[heater_bed]` → loads heater_bed.py
   
2. **Prefix-based**: `load_config_prefix(config)` - allows multiple instances with indexed names
   - Example: `[stepper_x]`, `[stepper_y]` → both call stepper.py with `load_config_prefix`
   
3. **Early modules**: `gcode.py`, `webhooks.py` call `add_early_printer_objects()` to load before config parsing

Pattern: All modules must call `printer.add_object("section_name", instance)` and register event handlers.

## MCU Command Protocol

**Command registration**: Host → MCU uses `DECL_COMMAND` macro in C files (e.g., [src/stepper.c](../src/stepper.c)).

**Response handling** in Python:
```python
# Send command, wait for response
query = mcu.query_command("get_temp oid=%d" % oid)
mcu.register_response(callback, "temp_data", oid)  # Register response handler
```

Commands are **serialized** and scheduled with timing: `minclock` (earliest), `reqclock` (preferred). The MCU prioritizes real-time stepper tasks; command latency is expected. See [mcu.py](../klippy/mcu.py) for command wrappers like `CommandQueryWrapper`.

**Async command pattern** (used for queries during runtime):
```python
class RetryAsyncCommand:  # Retries until response received
```

## Move Pipeline & Timing

Understanding **move execution** is essential for motion-related changes:

1. **G-code parsing** → [gcode.py](../klippy/gcode.py) → GCode commands (e.g., `G1`)
2. **gcode_move.py** → Translates G-code parameters to printer moves
3. **ToolHead.move()** → Adds to lookahead queue, flushes to steppers
4. **Kinematics** → Converts Cartesian XYZ to stepper positions via [klippy/kinematics/](../klippy/kinematics/) (cartesian.py, corexy.py, etc.)
5. **MCU Step Generation** → [klippy/stepper.py](../klippy/stepper.py) + C helper [chelper/steppersync.c](../klippy/chelper/)

**Critical**: Moves are **looked-ahead** (junction calculations) and **compressed** (step pulses consolidated into efficient time intervals). Never assume 1:1 gcode line → move execution.

## Python-C Integration (CFFI)

Klipper uses CFFI for performance-critical code (stepper compression, message protocol):

- **C Code**: [klippy/chelper/](../klippy/chelper/) (C files + header generation)
- **Python Wrapper**: [klippy/chelper/__init__.py](../klippy/chelper/__init__.py)
- **Usage**: `ffi_main, ffi_lib = chelper.get_ffi(); ffi_lib.function_name(...)`

Don't modify C helpers lightly; they handle microsecond-precision operations. Reference implementation examples in [stepper.py](../klippy/stepper.py) for setup patterns.

## Naming & Conventions

- **Config section names** → Python class instances: `[stepper_x]` config object → `MCU_stepper` class
- **OID (Object ID)** → Unique ID allocated by MCU for each managed object (stepper, heater, etc.); enables multiplexing over single serial connection
- **Ticks** → MCU clock cycles (16-bit or 32-bit depending on board); host converts time to ticks via `mcu.seconds_to_ticks()`
- **Print time** → Host's reference clock (seconds from startup); used for scheduling moves
- **Event time** → Reactor monotonic time (seconds); used in callbacks

## Configuration Parsing

Config system in [configfile.py](../klippy/configfile.py):
- `config.get_section("section_name")` → ConfigWrapper for a section
- `config.get("option", parser=float, minval=0.0, maxval=100.0)` → Type-safe parsing with validation
- `config.has_option("option")` → Check presence (important: options are case-insensitive internally)
- Custom sections accessible via `config.get_prefix_sections("prefix")`

Error handling: Raise `config_error` for invalid config, not generic exceptions.

## Testing & Building

- **Build**: `make` compiles MCU code (Kconfig-driven, board selection via .config); output → `out/klipper.elf.hex`
- **Host Tests**: `test/klippy/` contains Python unit tests (pytest-compatible)
- **MCU Simulation**: MCU can be compiled for host testing (linux-based simulator in `src/simulator/`)

Common workflow: `make menuconfig` (Kconfig board setup) → `make` → `scripts/flash_usb.py` for flashing.

## Common Pitfalls

1. **Blocking operations**: Never sleep/block in reactor callbacks; use `reactor.register_timer()` for delayed work
2. **Serial communication timing**: Host-MCU round-trip is ~1-5ms; don't assume instant responses
3. **Greenletting**: Reactor uses greenlets; don't spawn threads unless necessary (use `reactor` facilities)
4. **Config changes**: Config is read once at startup; runtime changes require module reload (handled via `RESTART` gcode)
5. **Tick precision**: MCU ticks overflow (16-bit); host handles wraparound; don't compare bare tick values

## Directory Reference

| Path | Purpose |
|------|---------|
| `klippy/` | Host Python code (reactor, gcode, motion, modules) |
| `klippy/extras/` | Dynamically loaded modules (heaters, fans, sensors, displays) |
| `klippy/kinematics/` | Printer types (cartesian, corexy, delta, etc.) |
| `klippy/chelper/` | Performance-critical C helpers (CFFI) |
| `src/` | Microcontroller C firmware (arch-specific subdirs) |
| `config/` | Example printer configurations |
| `docs/` | Architecture docs: Code_Overview.md, Kinematics.md, Protocol.md |

## Debugging Tips

- **Host logs**: `klippy/queuelogger.py` → check log file for errors; use `logging.info()` for debug output
- **MCU logs**: `logging` module available; serial protocol debugging in [msgproto.py](../klippy/msgproto.py)
- **State inspection**: `printer.lookup_object(name)` to access any component at runtime
- **Webhooks API**: [klippy/webhooks.py](../klippy/webhooks.py) provides inspection endpoints for debugging


[def]: ippy/klippy.py) for usag

[def]: be.py) for examples.