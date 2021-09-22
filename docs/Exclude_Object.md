# Exclude Obects

The `[exclude_object]` module allows Klipper to exclude objects while a print is in progress.
To enable this feature include an [exclude_object config section](Config_Reference.md#exclude_object)
(also see the [command reference](G-Codes.md#exclude-object).)

Unlike other 3D printer firmware options, a printer running Klipper is utilizing a suite of
components and users have many options to choose from.  Therefore, in order to provide a
a consistent user experience, the `[exclude_object]` moudle will establish a contract or API
of sorts.  The contract covers the contents of the gcode file, how the internal state of the
module is controlled, and how that state is provided to clients.

## Workflow Overview
A typical worfklow for printing a file might look like this:
1. Slicing is completed and the file is uploaded for printing.  During the upload, the file
   is processed and `[exclude_object]` markup is added to the file.
1. When printing starts, Klipper will reset the `[exclude_object]` status.
1. When Klipper processes the `DEFINE_OBJECT` block, it will update the status with the known
   objects and pass it on to clients.
1. The client may use that information to present a UI to the user so that progress can be
   tracked.  Klipper will update the status to include the currently printing object which
   the client can use for display purposes.
1. If the user requests that an object be cancelled, the client will issue an `EXCLUDE_OBJECT`
   command to Klipper.
1. When Klipper process the command, it will add the object to the list of excluded objects
   and update the status for the client.
1. The client will receive the updated status from Klipper and can use that information to
   reflect the object's status in the UI.
1. When printing finishes, the `[exclude_object]` status will continue to be available until
   another action resets it.

## The GCode File
The specialized GCode processing needed to support exlucing objects does not fit into Klipper's
core design goals.  Therefore, this module requires that the file is processed before being sent
to Klipper for printing.  Using a post-process script in the slicer or having middleware process
the file on upload are two possibilities for preparing the file for Klipper.

### GCode File Command Reference
`DEFINE_OBJECT`:  Provides a summary of an object in the file.  Objects don't need to be defined
in order to be referenced by other commands.  The primary purpose of this command is to provide
information to the UI without needing to parse the entire gcode file.

It takes the following parameters:

- `NAME`: This parameter is required.  It is the identifier used by other commands in this module.
 The name must be unique among all objects in the file being printed, and must be consistent across all layers.
- `CENTER`: An X,Y coordinate for the object.  Typically it will be in the center of the object, but
   that is not a requirement.  While this parameter is technically optional, not including it will
   likely limit the functionality of other components.  Example: `CENTER=150.07362,138.27616`.
- `POLYGON`: An array of X,Y coordinates specifying vertices that define a polygon outline for the object.
   The polygon information is primarly for the use of graphical interfces.  This parameter is optional, but
   like `CENTER`, the functionality of other components may be reduced if it is not given.  It is left to the
   software processing the gcode file to determine the complexity of the polygon being provided.  At a
   minimum, it is recommended that this be a bounding box.
   Example: `POLYGON=[[142.7,130.95],[142.7,145.75],[157.5,145.75],[157.5,130.95]]`

`START_CURRENT_OBJECT`: This command takes a `NAME` parameter and denotes the start of
the gcode for an object on the current layer.

`END_CURRENT_OBJECT`:  Denotes the end of the object's gcode for the layer.  It is paired with
`START_CURRENT_OBJECT`.  A `NAME` parameter is optional.  A warning will be given if
an `END_CURRENT_OBJECT` command is encountered when it wasn't expected or of the given
name does not match the current object.

## Managing Excluded Objects
The `EXCLUDE_OBJECT` command is used to request that Klipper stops printing the specified object.
The command may be executed at any time and Klipper will track the object name until the status is
reset.  This command may be executed manually, but will often be part of the exclude object implementation
of a client.

`LIST_OBJECTS`, `LIST_EXCLUDED_OBJECTS`, and `EXCLUDE_OBJECT_RESET` commands are also available

### Command Reference
`EXCLUDE_OBJECT`: This command takes a `NAME` parameter and instructs Klipper to ignore
gcode that is marked by `START_CURRENT_OBJECT` and `END_CURRENT_OBJECT` for the named
object.  The command can be issued for the currently printing object.  In that case, Klipper will
immediately move on to the next object.  An object can be marked for exclusion before Klipper
encounters it in the file.

`LIST_OBJECTS`: Lists the objects known to Klipper.

`LIST_EXCLUDED_OBJECTS`: Lists the excluded objects.

`EXCLUDE_OBJECT_RESET`: Resets the state of the `[exclude_object]` module.  This clears the lists
containing known objects, cancelled objects, and the name of the current object.

## Status Infomation
The state of this module is provided to clients by the [exclude_object status](Status_Reference.md#exclude_object).

The status is reset when:
- The Klipper firmware is restarted.
- There is a reset of the `[virtual_sdcard]`.  Notable, this is reset by Klipper at the start of a print.
- When an `EXCLUDE_OBJECT_RESET` command is issued.

The list of defined objects is represented in the `exclude_object.objects` status field.  In a well defined
gcode file, this will be done with `DEFINE_OBJECT` commands at the beginning of the file.  This will provide
clients with object names and coordinates so the UI can provide a graphical representation of the objects if
desired.

As the print progresses, the `exclude_object.current_object` status field will be updated as Klipper processes
`START_CURRENT_OBJECT` and `END_CURRENT_OBJECT` commands.  The `current_object` field will be set even if the
object has been excluded.

As `EXCLUDE_OBJECT` commands are issued, the list of excluded objects is provided in the `exclude_object.excluded_objects`
array.  Since Klipper looks ahead to process upcoming gcode, there may be a delay betwen when the command is
issued and when the status is updated.
