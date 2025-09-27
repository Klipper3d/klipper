# Klipper Python Development Guidelines

## Core Principles
- **KISS (Keep It Simple, Stupid)**: Klipper uses Python for most code to make it easier to develop new functionality
- **SOLID Principles**: Follow Single responsibility, Open/closed, Liskov substitution, Interface segregation, Dependency inversion
- **DRY (Don't Repeat Yourself)**: Extract reusable modules in `klippy/extras/` directory

## Common Commands
- `make menuconfig`: Configure micro-controller build options
- `make`: Compile micro-controller firmware
- `make flash FLASH_DEVICE=/dev/serial/by-id/...`: Flash firmware to MCU
- `sudo service klipper stop/start/restart`: Control Klipper service
- `~/klippy-env/bin/python ./klippy/klippy.py ~/printer.cfg`: Run Klipper host software
- `python3 -m pip install -r scripts/klippy-requirements.txt`: Install Python dependencies

## Klipper Code Style Guidelines
**IMPORTANT**: Follow both PEP 8 and Klipper-specific conventions

### PEP 8 Compliance
- Use 4 spaces for indentation (never tabs)
- Limit lines to 79 characters maximum
- Use snake_case for variables and functions
- Use CamelCase for class names
- Use UPPER_CASE for constants
- Surround top-level functions and classes with two blank lines

### Klipper-Specific Conventions
- Avoid accessing internal member variables (methods starting with underscore) of other printer objects
- Assign values to all member variables in Python constructors
- Use floating point constants consistently (prefer `self.speed = 1.` over `self.speed = 1`)
- Place copyright notice at top of modules for main Klipper inclusion

## Project Structure
```
klippy/                 # Host software (Python)
├── klippy.py          # Main entry point
├── gcode.py           # G-code command processing
├── kinematics/        # Robot kinematics code
├── extras/            # Extensible modules (preferred for new functionality)
├── chelper/           # C code helpers via CFFI
config/                # Example printer configurations
scripts/               # Build and utility scripts
src/                   # Micro-controller C code
test/                  # Automated test cases
```

## Module Development
Use dynamic module loading - if config section `[my_module]` exists, Klipper loads `klippy/extras/my_module.py`

### Module Template
```python
# Klipper module for [functionality]
#
# Copyright (C) [Year] [Author] <[email]>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class MyModule:
    def __init__(self, config):
        self.printer = config.get_printer()
        # Initialize all member variables here
        self.param = config.getfloat('param', 1.0)  # Use floating point
        
    def get_status(self, eventtime):
        return {'param': self.param}

def load_config(config):
    return MyModule(config)
```

## Testing Workflow
- Use existing modules as reference (e.g., `klippy/extras/servo.py`)
- Test with batch mode: `python klippy.py config.cfg -i test.gcode -o test.serial`
- Run regression tests from main directory
- Test module loading with appropriate config sections

## G-code Command Conventions
- Commands follow standard G-code format
- Use descriptive command names for custom commands
- Implement proper parameter validation and error handling
- Document commands in module docstrings

## Timing and Events
Klipper schedules events for near future execution - avoid instantaneous actions
- System time: floating point seconds (variables named `eventtime` or `curtime`)
- Use proper event scheduling for hardware operations
- Handle clock synchronization between multiple micro-controllers

## Configuration Best Practices
- Use `config.getfloat()`, `config.getint()`, `config.get()` for parameter reading
- Provide sensible defaults for optional parameters
- Validate configuration parameters in constructor
- Document configuration options in module comments

## Debugging and Development

- Use `scripts/motan/data_logger.py` for motion data analysis
- Enable API server for advanced debugging
- Use batch mode for G-code command analysis
- Test with `klippy-env` virtual environment

## Hardware Interface Guidelines
- Follow micro-controller communication protocols
- Use DECL_COMMAND macros for MCU commands
- Handle serial communication threading properly
- Implement proper error recovery and reporting

## Performance Considerations
- Klipper achieves high precision (25 microsecond) stepper timing
- Optimize for real-time performance in critical paths
- Use efficient algorithms for kinematics calculations
- Consider memory usage in embedded environments

## Documentation Requirements
- Add comprehensive docstrings for all public methods
- Document configuration parameters and their effects
- Include usage examples in module comments
- Update relevant documentation files when adding features

## Git Workflow
- Create feature branches: `git checkout -b feature/new-module`
- Follow conventional commit messages: `feat:`, `fix:`, `docs:`
- Test thoroughly before submitting pull requests
- Include configuration examples with new modules