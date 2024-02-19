#!/usr/bin/env python3
# Bed Mesh data plotting and analysis
#
# Copyright (C) 2024 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import argparse
import sys
import os
import stat
import errno
import time
import socket
import re
import json
import collections
import numpy as np
import matplotlib
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import matplotlib.animation as ani

MESH_DUMP_REQUEST = json.dumps(
    {"id": 1, "method": "bed_mesh/dump_mesh"}
)

def sock_error_exit(msg):
    sys.stderr.write(msg + "\n")
    sys.exit(-1)

def webhook_socket_create(uds_filename):
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    while 1:
        try:
            sock.connect(uds_filename)
        except socket.error as e:
            if e.errno == errno.ECONNREFUSED:
                time.sleep(0.1)
                continue
            sock_error_exit(
                "Unable to connect socket %s [%d,%s]"
                % (uds_filename, e.errno, errno.errorcode[e.errno])
            )
        break
    print("Connected")
    return sock

def process_message(msg):
    try:
        resp = json.loads(msg)
    except json.JSONDecodeError:
        return None
    if resp.get("id", -1) != 1:
        return None
    if "error" in resp:
        err = resp["error"].get("message", "Unknown")
        sock_error_exit(
            "Error requesting mesh dump: %s" % (err,)
        )
    return resp["result"]


def request_from_unixsocket(unix_sock_name):
    print("Connecting to Unix Socket File '%s'" % (unix_sock_name,))
    whsock = webhook_socket_create(unix_sock_name)
    whsock.settimeout(1.)
    # send mesh query
    whsock.send(MESH_DUMP_REQUEST.encode() + b"\x03")
    sock_data = b""
    end_time = time.monotonic() + 10.0
    try:
        while time.monotonic() < end_time:
            try:
                data = whsock.recv(4096)
            except TimeoutError:
                pass
            else:
                if not data:
                    sock_error_exit("Socket closed before mesh received")
                parts = data.split(b"\x03")
                parts[0] = sock_data + parts[0]
                sock_data = parts.pop()
                for msg in parts:
                    result = process_message(msg)
                    if result is not None:
                        return result
            time.sleep(.1)
    finally:
        whsock.close()
    sock_error_exit("Mesh dump request timed out")

def request_from_websocket(url):
    print("Connecting to websocket url '%s'" % (url,))
    try:
        from websockets.sync.client import connect
    except ModuleNotFoundError:
        sock_error_exit("Python module 'websockets' not installed.")
        raise
    with connect(url) as websocket:
        websocket.send(MESH_DUMP_REQUEST)
        end_time = time.monotonic() + 20.0
        while time.monotonic() < end_time:
            try:
                msg = websocket.recv(10.)
            except TimeoutError:
                continue
            result = process_message(msg)
            if result is not None:
                return result
            time.sleep(.1)
    sock_error_exit("Mesh dump request timed out")

def request_mesh_data(input_name):
    url_match = re.match(r"((?:https?)|(?:wss?))://(.+)", input_name.lower())
    if url_match is None:
        file_path = os.path.abspath(os.path.expanduser(input_name))
        if not os.path.exists(file_path):
            sock_error_exit("Path '%s' does not exist" % (file_path,))
        st_res = os.stat(file_path)
        if stat.S_ISSOCK(st_res.st_mode):
            return request_from_unixsocket(file_path)
        else:
            print("Reading mesh data from json file '%s'" % (file_path,))
            with open(file_path, "r") as f:
                return json.load(f)
    scheme = url_match.group(1)
    host = url_match.group(2).rstrip("/")
    scheme = scheme.replace("http", "ws")
    url = "%s://%s/klippysocket" % (scheme, host)
    return request_from_websocket(url)

class PathAnimation:
    instance = None
    def __init__(self, artist, x_travel, y_travel):
        self.travel_artist = artist
        self.x_travel = x_travel
        self.y_travel = y_travel
        fig = plt.gcf()
        self.animation = ani.FuncAnimation(
            fig=fig, func=self.update, frames=self.gen_path_position(),
            cache_frame_data=False, interval=60
        )
        PathAnimation.instance = self

    def gen_path_position(self):
        count = 1
        x_travel, y_travel = self.x_travel, self.y_travel
        last_x, last_y = x_travel[0], y_travel[0]
        yield count
        for xpos, ypos in zip(x_travel[1:], y_travel[1:]):
            count += 1
            if xpos == last_x or ypos == last_y:
                yield count
            last_x, last_y = xpos, ypos

    def update(self, frame):
        x_travel, y_travel = self.x_travel, self.y_travel
        self.travel_artist.set_xdata(x_travel[:frame])
        self.travel_artist.set_ydata(y_travel[:frame])
        return (self.travel_artist,)


def _gen_mesh_coords(min_c, max_c, count):
    dist = (max_c - min_c) / (count - 1)
    return [min_c + i * dist for i in range(count)]

def _plot_path(travel_path, probed, diff, cmd_args):
    x_travel, y_travel = np.array(travel_path).transpose()
    x_probed, y_probed = np.array(probed).transpose()
    plt.xlabel("X")
    plt.ylabel("Y")
    # plot travel
    travel_line = plt.plot(x_travel, y_travel, "b-")[0]
    # plot intermediate points
    plt.plot(x_probed, y_probed, "k.")
    # plot start point
    plt.plot([x_travel[0]], [y_travel[0]], "g>")
    # plot stop point
    plt.plot([x_travel[-1]], [y_travel[-1]], "r*")
    if diff:
        diff_x, diff_y = np.array(diff).transpose()
        plt.plot(diff_x, diff_y, "m.")
    if cmd_args.animate and cmd_args.output is None:
        PathAnimation(travel_line, x_travel, y_travel)

def _format_mesh_data(matrix, params):
    min_pt = (params["min_x"], params["min_y"])
    max_pt = (params["max_x"], params["max_y"])
    xvals = _gen_mesh_coords(min_pt[0], max_pt[0], len(matrix[0]))
    yvals = _gen_mesh_coords(min_pt[1], max_pt[0], len(matrix))
    x, y = np.meshgrid(xvals, yvals)
    z = np.array(matrix)
    return x, y, z

def _set_xy_limits(mesh_data, cmd_args):
    if not cmd_args.scale_plot:
        return
    ax = plt.gca()
    axis_min = mesh_data["axis_minimum"]
    axis_max = mesh_data["axis_maximum"]
    ax.set_xlim((axis_min[0], axis_max[0]))
    ax.set_ylim((axis_min[1], axis_max[1]))

def _plot_mesh(ax, matrix, params, cmap=cm.viridis, label=None):
    x, y, z = _format_mesh_data(matrix, params)
    surface = ax.plot_surface(x, y, z, cmap=cmap, label=label)
    scale = max(abs(z.min()), abs(z.max())) * 3
    return surface, scale

def plot_probe_points(mesh_data, cmd_args):
    """Plot original generated points"""
    calibration = mesh_data["calibration"]
    x, y = np.array(calibration["points"]).transpose()
    plt.title("Generated Probe Points")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.plot(x, y, "b.")
    _set_xy_limits(mesh_data, cmd_args)

def plot_probe_path(mesh_data, cmd_args):
    """Plot probe travel path"""
    calibration = mesh_data["calibration"]
    orig_pts = calibration["points"]
    path_pts = calibration["probe_path"]
    diff = [pt for pt in orig_pts if pt not in path_pts]
    plt.title("Probe Travel Path")
    _plot_path(path_pts, path_pts[1:-1], diff, cmd_args)
    _set_xy_limits(mesh_data, cmd_args)

def plot_rapid_path(mesh_data, cmd_args):
    """Plot rapid scan travel path"""
    calibration = mesh_data["calibration"]
    orig_pts = calibration["points"]
    rapid_pts = calibration["rapid_path"]
    rapid_path = [pt[0] for pt in rapid_pts]
    probed = [pt for pt, is_ppt in rapid_pts if is_ppt]
    diff = [pt for pt in orig_pts if pt not in probed]
    plt.title("Rapid Scan Travel Path")
    _plot_path(rapid_path, probed, diff, cmd_args)
    _set_xy_limits(mesh_data, cmd_args)

def plot_probed_matrix(mesh_data, cmd_args):
    """Plot probed Z values"""
    ax = plt.subplot(projection="3d")
    profile = cmd_args.profile_name
    if profile is not None:
        req_mesh = mesh_data["profiles"].get(profile)
        if req_mesh is None:
            raise Exception("Profile %s not found" % (profile,))
        matrix = req_mesh["points"]
        name = profile
    else:
        req_mesh = mesh_data["current_mesh"]
        if not req_mesh:
            raise Exception("No current mesh data in dump")
        matrix = req_mesh["probed_matrix"]
        name = req_mesh["name"]
    params = req_mesh["mesh_params"]
    surface, scale = _plot_mesh(ax, matrix, params)
    ax.set_title("Probed Mesh (%s)" % (name,))
    ax.set(zlim=(-scale, scale))
    plt.gcf().colorbar(surface, shrink=.75)
    _set_xy_limits(mesh_data, cmd_args)

def plot_mesh_matrix(mesh_data, cmd_args):
    """Plot mesh Z values"""
    ax = plt.subplot(projection="3d")
    req_mesh = mesh_data["current_mesh"]
    if not req_mesh:
        raise Exception("No current mesh data in dump")
    matrix = req_mesh["mesh_matrix"]
    params = req_mesh["mesh_params"]
    surface, scale = _plot_mesh(ax, matrix, params)
    name = req_mesh["name"]
    ax.set_title("Interpolated Mesh (%s)" % (name,))
    ax.set(zlim=(-scale, scale))
    plt.gcf().colorbar(surface, shrink=.75)
    _set_xy_limits(mesh_data, cmd_args)

def plot_overlay(mesh_data, cmd_args):
    """Plots the current probed mesh overlaid with a profile"""
    ax = plt.subplot(projection="3d")
    # Plot Profile
    profile = cmd_args.profile_name
    if profile is None:
        raise Exception("A profile must be specified to plot an overlay")
    req_mesh = mesh_data["profiles"].get(profile)
    if req_mesh is None:
        raise Exception("Profile %s not found" % (profile,))
    matrix = req_mesh["points"]
    params = req_mesh["mesh_params"]
    prof_surf, prof_scale = _plot_mesh(ax, matrix, params, label=profile)
    # Plot Current
    req_mesh = mesh_data["current_mesh"]
    if not req_mesh:
        raise Exception("No current mesh data in dump")
    matrix = req_mesh["probed_matrix"]
    params = req_mesh["mesh_params"]
    cur_name = req_mesh["name"]
    cur_surf, cur_scale = _plot_mesh(ax, matrix, params, cm.inferno, cur_name)
    ax.set_title("Probed Mesh Overlay")
    scale = max(cur_scale, prof_scale)
    ax.set(zlim=(-scale, scale))
    ax.legend(loc='best')
    plt.gcf().colorbar(prof_surf, shrink=.75)
    _set_xy_limits(mesh_data, cmd_args)

def plot_delta(mesh_data, cmd_args):
    """Plots the delta between current probed mesh and a profile"""
    ax = plt.subplot(projection="3d")
    # Plot Profile
    profile = cmd_args.profile_name
    if profile is None:
        raise Exception("A profile must be specified to plot an overlay")
    req_mesh = mesh_data["profiles"].get(profile)
    if req_mesh is None:
        raise Exception("Profile %s not found" % (profile,))
    prof_matix = req_mesh["points"]
    prof_params = req_mesh["mesh_params"]
    req_mesh = mesh_data["current_mesh"]
    if not req_mesh:
        raise Exception("No current mesh data in dump")
    cur_matrix = req_mesh["probed_matrix"]
    cur_params = req_mesh["mesh_params"]
    cur_name = req_mesh["name"]
    # validate that the params match
    pfields = ("x_count", "y_count", "min_x", "max_x", "min_y", "max_y")
    for field in pfields:
        if abs(prof_params[field] - cur_params[field]) >= 1e-6:
            raise Exception(
                "Values for field %s do not match, cant plot deviation"
            )
    delta = np.array(cur_matrix) - np.array(prof_matix)
    surface, scale = _plot_mesh(ax, delta, cur_params)
    ax.set(zlim=(-scale, scale))
    ax.set_title("Probed Mesh Delta (%s, %s)" % (cur_name, profile))
    _set_xy_limits(mesh_data, cmd_args)


PLOT_TYPES = {
    "points": plot_probe_points,
    "path": plot_probe_path,
    "rapid": plot_rapid_path,
    "probedz": plot_probed_matrix,
    "meshz": plot_mesh_matrix,
    "overlay": plot_overlay,
    "delta": plot_delta,
}

def print_types(cmd_args):
    typelist = [
        "%-10s%s" % (name, func.__doc__) for name, func in PLOT_TYPES.items()
    ]
    print("\n".join(typelist))

def plot_mesh_data(cmd_args):
    mesh_data = request_mesh_data(cmd_args.input)
    if cmd_args.output is not None:
        matplotlib.use("svg")

    fig = plt.figure()
    plot_func = PLOT_TYPES[cmd_args.type]
    plot_func(mesh_data, cmd_args)
    fig.set_size_inches(10, 8)
    fig.tight_layout()
    if cmd_args.output is None:
        plt.show()
    else:
        fig.savefig(cmd_args.output)

def _check_path_unique(name, path):
    path = np.array(path)
    unique_pts, counts = np.unique(path, return_counts=True, axis=0)
    for idx, count in enumerate(counts):
        if count != 1:
            coord = unique_pts[idx]
            print(
                "  WARNING: Backtracking or duplicate found in %s path at %s, "
                "this may be due to multiple samples in a faulty region."
                % (name, coord)
            )

def _analyze_mesh(name, mesh_axes):
    print("\nAnalyzing Probed Mesh %s..." % (name,))
    x, y, z = mesh_axes
    min_idx, max_idx = z.argmin(), z.argmax()
    min_x, min_y = x.flatten()[min_idx], y.flatten()[min_idx]
    max_x, max_y = x.flatten()[max_idx], y.flatten()[max_idx]

    print(
        "  Min Coord (%.2f, %.2f), Max Coord (%.2f, %.2f), "
        "Probe Count: (%d, %d)" %
        (x.min(), y.min(), x.max(), y.max(), len(z), len(z[0]))
    )
    print(
        "  Mesh range: min %.4f (%.2f, %.2f), max %.4f (%.2f, %.2f)"
        % (z.min(), min_x, min_y, z.max(), max_x, max_y)
    )
    print("  Mean: %.4f, Standard Deviation: %.4f" % (z.mean(), z.std()))

def _compare_mesh(name_a, name_b, mesh_a, mesh_b):
    ax, ay, az = mesh_a
    bx, by, bz = mesh_b
    if not np.array_equal(ax, bx) or not np.array_equal(ay, by):
        return
    delta = az - bz
    abs_max = max(abs(delta.max()), abs(delta.min()))
    abs_mean = sum([abs(z) for z in delta.flatten()]) / len(delta.flatten())
    min_idx, max_idx = delta.argmin(), delta.argmax()
    min_x, min_y = ax.flatten()[min_idx], ay.flatten()[min_idx]
    max_x, max_y = ax.flatten()[max_idx], ay.flatten()[max_idx]
    print("  Delta from %s to %s..." % (name_a, name_b))
    print(
        "    Range: min %.4f (%.2f, %.2f), max %.4f (%.2f, %.2f)\n"
        "    Mean: %.6f, Standard Deviation: %.6f\n"
        "    Absolute Max: %.6f, Absolute Mean: %.6f"
        % (delta.min(), min_x, min_y, delta.max(), max_x, max_y,
           delta.mean(), delta.std(), abs_max, abs_mean)
    )

def analyze(cmd_args):
    mesh_data = request_mesh_data(cmd_args.input)
    print("Analyzing Travel Path...")
    calibration = mesh_data["calibration"]
    org_pts = calibration["points"]
    probe_path = calibration["probe_path"]
    rapid_path = calibration["rapid_path"]
    rapid_points = [pt for pt, is_pt in rapid_path if is_pt]
    rapid_moves = [pt[0] for pt in rapid_path]
    print("  Original point count: %d" % (len(org_pts)))
    print("  Probe path count: %d" % (len(probe_path)))
    print("  Rapid scan sample count: %d" % (len(probe_path)))
    print("  Rapid scan move count: %d" % (len(rapid_moves)))
    if np.array_equal(rapid_points, probe_path):
        print("  Rapid scan points match probe path points")
    else:
        diff = [pt for pt in rapid_points if pt not in probe_path]
        print(
            "  ERROR: Rapid scan points do not match probe points\n"
            "difference: %s" % (diff,)
        )
    _check_path_unique("probe", probe_path)
    _check_path_unique("rapid scan", rapid_moves)
    req_mesh = mesh_data["current_mesh"]
    formatted_data = collections.OrderedDict()
    if req_mesh:
        matrix = req_mesh["probed_matrix"]
        params = req_mesh["mesh_params"]
        name = req_mesh["name"]
        formatted_data[name] = _format_mesh_data(matrix, params)
    profiles = mesh_data["profiles"]
    for prof_name, prof_data in profiles.items():
        if prof_name in formatted_data:
            continue
        matrix = prof_data["points"]
        params = prof_data["mesh_params"]
        formatted_data[prof_name] = _format_mesh_data(matrix, params)
    while formatted_data:
        name, current_axes = formatted_data.popitem()
        _analyze_mesh(name, current_axes)
        for prof_name, prof_axes in formatted_data.items():
            _compare_mesh(name, prof_name, current_axes, prof_axes)

def dump_request(cmd_args):
    mesh_data = request_mesh_data(cmd_args.input)
    outfile = cmd_args.output
    if outfile is None:
        postfix = time.strftime("%Y%m%d_%H%M%S")
        outfile = "klipper-bedmesh-%s.json" % (postfix,)
    outfile = os.path.abspath(os.path.expanduser(outfile))
    print("Saving Mesh Output to '%s'" % (outfile))
    with open(outfile, "w") as f:
        f.write(json.dumps(mesh_data))

def main():
    parser = argparse.ArgumentParser(description="Graph Bed Mesh Data")
    sub_parsers = parser.add_subparsers()
    list_parser = sub_parsers.add_parser(
        "list", help="List available plot types"
    )
    list_parser.set_defaults(func=print_types)
    plot_parser = sub_parsers.add_parser("plot", help="Plot a specified type")
    analyze_parser = sub_parsers.add_parser(
        "analyze", help="Perform analysis on mesh data"
    )
    dump_parser = sub_parsers.add_parser(
        "dump", help="Dump API response to json file"
    )
    plot_parser.add_argument(
        "-a", "--animate", action="store_true",
        help="Animate paths in live preview"
    )
    plot_parser.add_argument(
        "-s", "--scale-plot", action="store_true",
        help="Use axis limits reported by Klipper to scale plot X/Y"
    )
    plot_parser.add_argument(
        "-p", "--profile-name", type=str, default=None,
        help="Optional name of a profile to plot for 'probedz'"
    )
    plot_parser.add_argument(
        "-o", "--output", type=str, default=None,
        help="Output file path"
    )
    plot_parser.add_argument(
        "type", metavar="<plot type>", type=str, choices=PLOT_TYPES.keys(),
        help="Type of data to graph"
    )
    plot_parser.add_argument(
        "input", metavar="<input>",
        help="Path/url to Klipper Socket or path to json file"
    )
    plot_parser.set_defaults(func=plot_mesh_data)
    analyze_parser.add_argument(
        "input", metavar="<input>",
        help="Path/url to Klipper Socket or path to json file"
    )
    analyze_parser.set_defaults(func=analyze)
    dump_parser.add_argument(
        "-o", "--output", type=str, default=None,
        help="Json output file path"
    )
    dump_parser.add_argument(
        "input", metavar="<input>",
        help="Path or url to Klipper Socket"
    )
    dump_parser.set_defaults(func=dump_request)
    cmd_args = parser.parse_args()
    cmd_args.func(cmd_args)


if __name__ == "__main__":
    main()
