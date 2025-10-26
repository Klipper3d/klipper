import logging
import typing


class Bucket:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object("gcode")
        self.toolhead = None

        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler(
            "klippy:connect", self.handle_connect
        )

        self.has_custom_bed_bound = config.getboolean(
            "has_custom_boundary", False
        )
        self.bucket_position = config.getfloatlist(
            "bucket_position", None, count=2
        )
        self.travel_speed = config.getfloat(
            "travel_speed", default=50.0, minval=20.0, maxval=10000.0
        )

        self.custom_bed_bound_object = None

        self.gcode.register_command(
            "MOVE_TO_BUCKET",
            self.cmd_MOVE_TO_BUCKET,
            "Gcode for moving the toolhead to the bucket position. Takes into account if there is a custom bed boundary",
        )

    def handle_ready(self):
        if self.has_custom_bed_bound:
            self.custom_bed_bound_object = self.printer.lookup_object(
                "bed_custom_bound", default=None
            )

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object("toolhead", default=None)

    def move_to_bucket(self, split: typing.Optional["bool"] = False):
        """Moves to bucket position"""
        if not self.toolhead:
            return
        try:
            if self.custom_bed_bound_object:
                _conf_bound = (
                    self.custom_bed_bound_object.check_boundary_limits(
                        position=(
                            self.bucket_position[0],
                            self.bucket_position[1],
                        )
                    )
                )
                if (
                    not _conf_bound["x"] or not _conf_bound["y"]
                ) and self.custom_bed_bound_object.get_status().get(
                    "status", ""
                ) == "custom":
                    self.custom_bed_bound_object.restore_default_boundary()
                    

            if not split:
                self.toolhead.manual_move(
                    [self.bucket_position[0], self.bucket_position[1]],
                    self.travel_speed,
                )
            else:
                self.toolhead.manual_move(
                    [self.bucket_position[0]], self.travel_speed
                )
                self.toolhead.wait_moves()
                self.toolhead.manual_move(
                    [self.bucket_position[0], self.bucket_position[1]],
                    self.travel_speed,
                )

            self.toolhead.wait_moves()

            if (
                self.custom_bed_bound_object
                and self.custom_bed_bound_object.get_status().get("status", "")
                == "default"
            ):
                self.custom_bed_bound_object.set_custom_boundary()
        except Exception as e:
            raise BucketMoveError(
                f"Exception occurred when moving toolhead to bucket position: {e}"
            )

    def cmd_MOVE_TO_BUCKET(self, gcmd):
        _split = gcmd.get("SPLIT", False, parser=bool)
        self.move_to_bucket(split=_split)


class BucketMoveError(Exception):
    "Raised when there is an exception moving the toolhead to the bucket"

    def __init__(self, message, errors=None):
        super(BucketMoveError, self).__init__(message)
        self.errors = errors


def load_config(config):
    return Bucket(config)
