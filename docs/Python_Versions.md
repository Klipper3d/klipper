# Python Versions

## Current Support
Klippy (Klipper's python component) supports python 2.7, and python 3.5 and later,
however user-installed host modules (AKA extras) may require higher versions.

The tools in `scripts/`, vary, but all work on 3.6+.

## Future Support
The project has no plans to sunset support for Python 2 at this time.
New versions of Python 3 may not be compatible immediately after release.

## Development Notes
* `ConfigParser` and `Queue` are handled by top-scope tweaks in `utils.py`.
* CI Tests run with 2.7 and 3.8
* Klippy's codebase only requires py 3.3, but dependances raise it to 3.5

## References
* [Discourse topic on python versions](https://klipper.discourse.group/t/discuss-python-versions/20942)
