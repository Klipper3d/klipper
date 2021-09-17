# Timestamp

This document describes the Timestamp module.  There is no configuration required.  Simply put a [timestamp] section in your printer.cfg document to get timestamp information available in gcode macros and elsewhere.

## Congifuration

    [timestamp]

    # No configuration parameters available or necessary

    # Variables available for use in gcode macros and elsewhere
    # Time is in 24h notation

    # printer.timestamp.Timestamp
    #   returns a string in the form YYYY-MM-DD HH:MM:SS.MICROS
    #   where:  from left to right
    #           YYYY - four digit year
    #           MM - two digit month, 01-12
    #           DD - two digit day, 01-31
    #           HH - two digit hour, 01-12
    #           MM - two digit minute, 00-59
    #           SS - two digit second, 00-59
    #           MICROS - microseconds, 000000-999999

    # printer.timestamp.Time
    #   returns a string in the form HH:MM:SS
    #   where:  HH - two digit hour, 01-12
    #           MM - two digit minute, 00-59
    #           SS - two digit second, 00-59

    # printer.timestamp.Hour
    #   returns an integer in the form "H" or "HH"
    #   where:  H or HH - single or two digit hour, 01-12

    # printer.timestamp.Minute
    #   returns an integer in the form "M" or "MM"
    #   where:  M or MM - single or two digit minute, 00-59

    # printer.timestamp.Second
    #   returns an integer in the form "S" or "SS"
    #   where:  S or SS - single or two digit second, 00-59

## Commands
**TIMESTAMP** - returns the current timestamp, see variables above for more details
**TIME** - returns the current time, see variables above for more details
**HOUR** - returns the current hour, see variables above for more details
**MINUTE** - returns the current minute, see variables above for more details
**SECOND** - returns the current second, see variables above for more details
