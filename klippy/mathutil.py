# Simple math helper functions
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, multiprocessing, traceback
import queuelogger


######################################################################
# Coordinate descent
######################################################################

# Helper code that implements coordinate descent
def coordinate_descent(adj_params, params, error_func):
    # Define potential changes
    params = dict(params)
    dp = {param_name: 1. for param_name in adj_params}
    # Calculate the error
    best_err = error_func(params)
    logging.info("Coordinate descent initial error: %s", best_err)

    threshold = 0.00001
    rounds = 0

    while sum(dp.values()) > threshold and rounds < 10000:
        rounds += 1
        for param_name in adj_params:
            orig = params[param_name]
            params[param_name] = orig + dp[param_name]
            err = error_func(params)
            if err < best_err:
                # There was some improvement
                best_err = err
                dp[param_name] *= 1.1
                continue
            params[param_name] = orig - dp[param_name]
            err = error_func(params)
            if err < best_err:
                # There was some improvement
                best_err = err
                dp[param_name] *= 1.1
                continue
            params[param_name] = orig
            dp[param_name] *= 0.9
    logging.info("Coordinate descent best_err: %s  rounds: %d",
                 best_err, rounds)
    return params

# Helper to run the coordinate descent function in a background
# process so that it does not block the main thread.
def background_coordinate_descent(printer, adj_params, params, error_func):
    parent_conn, child_conn = multiprocessing.Pipe()
    def wrapper():
        queuelogger.clear_bg_logging()
        try:
            res = coordinate_descent(adj_params, params, error_func)
        except:
            child_conn.send((True, traceback.format_exc()))
            child_conn.close()
            return
        child_conn.send((False, res))
        child_conn.close()
    # Start a process to perform the calculation
    calc_proc = multiprocessing.Process(target=wrapper)
    calc_proc.daemon = True
    calc_proc.start()
    # Wait for the process to finish
    reactor = printer.get_reactor()
    gcode = printer.lookup_object("gcode")
    eventtime = last_report_time = reactor.monotonic()
    while calc_proc.is_alive():
        if eventtime > last_report_time + 5.:
            last_report_time = eventtime
            gcode.respond_info("Working on calibration...", log=False)
        eventtime = reactor.pause(eventtime + .1)
    # Return results
    is_err, res = parent_conn.recv()
    if is_err:
        raise Exception("Error in coordinate descent: %s" % (res,))
    calc_proc.join()
    parent_conn.close()
    return res


######################################################################
# Trilateration
######################################################################

# Trilateration finds the intersection of three spheres.  See the
# wikipedia article for the details of the algorithm.
def trilateration(sphere_coords, radius2):
    sphere_coord1, sphere_coord2, sphere_coord3 = sphere_coords
    s21 = matrix_sub(sphere_coord2, sphere_coord1)
    s31 = matrix_sub(sphere_coord3, sphere_coord1)

    d = math.sqrt(matrix_magsq(s21))
    ex = matrix_mul(s21, 1. / d)
    i = matrix_dot(ex, s31)
    vect_ey = matrix_sub(s31, matrix_mul(ex, i))
    ey = matrix_mul(vect_ey, 1. / math.sqrt(matrix_magsq(vect_ey)))
    ez = matrix_cross(ex, ey)
    j = matrix_dot(ey, s31)

    x = (radius2[0] - radius2[1] + d**2) / (2. * d)
    y = (radius2[0] - radius2[2] - x**2 + (x-i)**2 + j**2) / (2. * j)
    z = -math.sqrt(radius2[0] - x**2 - y**2)

    ex_x = matrix_mul(ex, x)
    ey_y = matrix_mul(ey, y)
    ez_z = matrix_mul(ez, z)
    return matrix_add(sphere_coord1, matrix_add(ex_x, matrix_add(ey_y, ez_z)))


######################################################################
# Matrix helper functions for 3x1 matrices
######################################################################

def matrix_cross(m1, m2):
    return [m1[1] * m2[2] - m1[2] * m2[1],
            m1[2] * m2[0] - m1[0] * m2[2],
            m1[0] * m2[1] - m1[1] * m2[0]]

def matrix_dot(m1, m2):
    return m1[0] * m2[0] + m1[1] * m2[1] + m1[2] * m2[2]

def matrix_magsq(m1):
    return m1[0]**2 + m1[1]**2 + m1[2]**2

def matrix_add(m1, m2):
    return [m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]]

def matrix_sub(m1, m2):
    return [m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]]

def matrix_mul(m1, s):
    return [m1[0]*s, m1[1]*s, m1[2]*s]

######################################################################
# Polynomial Helper Classes and Functions
######################################################################

def calc_determinant(matrix):
    m = matrix
    aei = m[0][0] * m[1][1] * m[2][2]
    bfg = m[1][0] * m[2][1] * m[0][2]
    cdh = m[2][0] * m[0][1] * m[1][2]
    ceg = m[2][0] * m[1][1] * m[0][2]
    bdi = m[1][0] * m[0][1] * m[2][2]
    afh = m[0][0] * m[2][1] * m[1][2]
    return aei + bfg + cdh - ceg - bdi - afh

class Polynomial2d:
    def __init__(self, a, b, c):
        self.a = a
        self.b = b
        self.c = c

    def __call__(self, xval):
        return self.c * xval * xval + self.b * xval + self.a

    def get_coefs(self):
        return (self.a, self.b, self.c)

    def __str__(self):
        return "%f, %f, %f" % (self.a, self.b, self.c)

    def __repr__(self):
        parts = ["y(x) ="]
        deg = 2
        for i, coef in enumerate((self.c, self.b, self.a)):
            if round(coef, 8) == int(coef):
                coef == int(coef)
            if abs(coef) < 1e-10:
                continue
            cur_deg = deg - i
            x_str = "x^%d" % (cur_deg,) if cur_deg > 1 else "x" * cur_deg
            if len(parts) == 1:
                parts.append("%f%s" % (coef, x_str))
            else:
                sym = "-" if coef < 0 else "+"
                parts.append("%s %f%s" % (sym, abs(coef), x_str))
        return " ".join(parts)

    @classmethod
    def fit(cls, coords):
        xlist = [c[0] for c in coords]
        ylist = [c[1] for c in coords]
        count = len(coords)
        sum_x = sum(xlist)
        sum_y = sum(ylist)
        sum_x2 = sum([x**2 for x in xlist])
        sum_x3 = sum([x**3 for x in xlist])
        sum_x4 = sum([x**4 for x in xlist])
        sum_xy = sum([x * y for x, y in coords])
        sum_x2y = sum([y*x**2 for x, y in coords])
        vector_b = [sum_y, sum_xy, sum_x2y]
        m = [
            [count, sum_x, sum_x2],
            [sum_x, sum_x2, sum_x3],
            [sum_x2, sum_x3, sum_x4]
        ]
        m0 = [vector_b, m[1], m[2]]
        m1 = [m[0], vector_b, m[2]]
        m2 = [m[0], m[1], vector_b]
        det_m = calc_determinant(m)
        a0 = calc_determinant(m0) / det_m
        a1 = calc_determinant(m1) / det_m
        a2 = calc_determinant(m2) / det_m
        return cls(a0, a1, a2)
