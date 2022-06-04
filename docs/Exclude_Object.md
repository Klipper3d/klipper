# Exclude Objects

The `[exclude_object]` module allows Klipper to exclude objects while a print is
in progress. To enable this feature include an [exclude_object config
section](Config_Reference.md#exclude_object) (also see the [command
reference](G-Codes.md#exclude-object) and
[sample-macros.cfg](../config/sample-macros.cfg) file for a
Marlin/RepRapFirmware compatible M486 G-Code macro.)

Unlike other 3D printer firmware options, a printer running Klipper utilizes a
suite of components and users have many options to choose from.  Therefore, in
order to provide a a consistent user experience, the `[exclude_object]` module
will establish a contract or API of sorts.  The contract covers the contents of
the gcode file, how the internal state of the module is controlled, and how that
state is provided to clients.

## Workflow Overview
A typical workflow for printing a file might look like this:
1. Slicing is completed and the file is uploaded for printing.  During the
    upload, the file is processed and `[exclude_object]` markers are added to
    the file. Alternately, slicers may be configured to prepare object exclusion
    markers natively, or in it's own pre-processing step.
2. When printing starts, Klipper will reset the `[exclude_object]`
   [status](Status_Reference.md#exclude_object).
3. When Klipper processes the `EXCLUDE_OBJECT_DEFINE` block, it will update the
   status with the known objects and pass it on to clients.
4. The client may use that information to present a UI to the user so that
   progress can be tracked.  Klipper will update the status to include the
   currently printing object which the client can use for display purposes.
5. If the user requests that an object be cancelled, the client will issue an
   `EXCLUDE_OBJECT NAME=<name>` command to Klipper.
6. When Klipper process the command, it will add the object to the list of
   excluded objects and update the status for the client.
7. The client will receive the updated status from Klipper and can use that
   information to reflect the object's status in the UI.
8. When printing finishes, the `[exclude_object]` status will continue to be
   available until another action resets it.

## The GCode File
The specialized gcode processing needed to support excluding objects does not
fit into Klipper's core design goals.  Therefore, this module requires that the
file is processed before being sent to Klipper for printing.  Using a
post-process script in the slicer or having middleware process the file on
upload are two possibilities for preparing the file for Klipper. A reference
post-processing script is available both as an executable and a python library,
see
[cancelobject-preprocessor](https://github.com/kageurufu/cancelobject-preprocessor).

### Object Definitions

The `EXCLUDE_OBJECT_DEFINE` command is used to provide a summary of each object
in the gcode file to be printed.  Provides a summary of an object in the file.
Objects don't need to be defined in order to be referenced by other commands.
The primary purpose of this command is to provide information to the UI without
needing to parse the entire gcode file.

Object definitions are named, to allow users to easily select an object to be
excluded, and additional metadata may be provided to allow for graphical
cancellation displays. Currently defined metadata includes a `CENTER` X,Y
coordinate, and a `POLYGON` list of X,Y points representing a minimal outline of
the object. This could be a simple bounding box, or a complicated hull for
showing more detailed visualizations of the printed objects. Especially when
gcode files include multiple parts with overlapping bounding regions, center
points become hard to visually distinguish. `POLYGONS` must be a json-compatible
array of point `[X,Y]` tuples without whitespace. Additional parameters will be
saved as strings in the object definition and provided in status updates.

`EXCLUDE_OBJECT_DEFINE NAME=calibration_pyramid CENTER=50,50
POLYGON=[[40,40],[50,60],[60,40]]`

All available G-Code commands are documented in the [G-Code
Reference](./G-Codes.md#excludeobject)

## Status Information
The state of this module is provided to clients by the [exclude_object
status](Status_Reference.md#exclude_object).

The status is reset when:
- The Klipper firmware is restarted.
- There is a reset of the `[virtual_sdcard]`.  Notably, this is reset by Klipper
  at the start of a print.
- When an `EXCLUDE_OBJECT_DEFINE RESET=1` command is issued.

The list of defined objects is represented in the `exclude_object.objects`
status field.  In a well defined gcode file, this will be done with
`EXCLUDE_OBJECT_DEFINE` commands at the beginning of the file.  This will
provide clients with object names and coordinates so the UI can provide a
graphical representation of the objects if desired.

As the print progresses, the `exclude_object.current_object` status field will
be updated as Klipper processes `EXCLUDE_OBJECT_START` and `EXCLUDE_OBJECT_END`
commands.  The `current_object` field will be set even if the object has been
excluded. Undefined objects marked with a `EXCLUDE_OBJECT_START` will be added
to the known objects to assist in UI hinting, without any additional metadata.

As `EXCLUDE_OBJECT` commands are issued, the list of excluded objects is
provided in the `exclude_object.excluded_objects` array.  Since Klipper looks
ahead to process upcoming gcode, there may be a delay between when the command
is issued and when the status is updated.
