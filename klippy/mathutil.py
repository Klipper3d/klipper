# Simple math helper functions
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2025-2026  Dmitry Butyugin <dmbutyugin@google.com>
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
# Matrix helper functions for NxM matrices
######################################################################

# Transpose a matrix
def mat_transp(a):
    return [[a[j][i] for j in range(len(a))]
            for i in range(len(a[0]))]

# Multiply two matrices
def mat_mat_mul(a, b):
    rows_a = len(a)
    cols_a = len(a[0])
    rows_b = len(b)
    cols_b = len(b[0])
    if cols_a != rows_b:
        return None
    bt = mat_transp(b)
    return [[sum([aik * bkj for aik, bkj in zip(a[i], bt[j])])
             for j in range(cols_b)]
            for i in range(rows_a)]

# Optimized version of mat_mat_mul(mat_transp(a), b)
def mat_transp_mul(a, b):
    rows_at = len(a[0])
    cols_at = len(a)
    rows_b = len(b)
    cols_b = len(b[0])
    if cols_at != rows_b:
        return None
    at = mat_transp(a)
    bt = mat_transp(b) if a is not b else at
    res = [[0.] * cols_b for i in range(rows_at)]
    for i in range(rows_at):
        if a is b:
            res[i][:i] = [r[i] for r in res[:i]]
            j_start = i
        else:
            j_start = 0
        axi = at[i]
        for j in range(j_start, cols_b):
            bxj = bt[j]
            res[i][j] = sum([aki * bkj for aki, bkj in zip(axi, bxj)])
    return res

def gaussian_solve(a, rhs, allow_underdetermined=False):
    res = [row[:] for row in rhs]
    m = [row[:] for row in a]
    n = len(m)
    # Perform the LU-decomposition through Gaussian elimination
    for i in range(n):
        # Find a pivot and swap the corresponding rows
        abs_col = [abs(m[j][i]) for j in range(i, n)]
        j = abs_col.index(max(abs_col)) + i
        if i != j:
            m[i], m[j] = m[j], m[i]
            res[i], res[j] = res[j], res[i]

        # Scale the i-th row
        if abs(m[i][i]) < 1e-10:
            if not allow_underdetermined:
                return None
            recipr = 0.
        else:
            recipr = 1. / m[i][i]
        m[i][i+1:] = [mij * recipr for mij in m[i][i+1:]]
        res[i][:] = [rij * recipr for rij in res[i]]
        m[i][i] = 1.

        # Zero-out the i-th column after the row i
        mi = m[i]
        ri = res[i]
        for j in range(i+1, n):
            mj = m[j]
            c = mj[i]
            mj[i:] = [mjk - c * mik for mjk, mik in zip(mj[i:], mi[i:])]
            rj = res[j]
            rj[:] = [rjk - c * rik for rjk, rik in zip(rj, ri)]

    # Solve the system with the upper-triangular matrix
    for i in range(n-2, -1, -1):
        mi = m[i]
        ri = res[i]
        for j in range(i+1, n):
            c = mi[j]
            rj = res[j]
            ri[:] = [rik - c * rjk for rik, rjk in zip(ri, rj)]
    return res

def pseudo_inverse(m):
    mtm = mat_transp_mul(m, m)
    mt = mat_transp(m)
    return gaussian_solve(mtm, mt)

# Find least squares solution for a set of linear equations
def solve_linear_equations(eqs, ans, allow_underdetermined=False):
    eqst_eqs = mat_transp_mul(eqs, eqs)
    eqst_ans = mat_transp_mul(eqs, ans)
    return gaussian_solve(eqst_eqs, eqst_ans, allow_underdetermined)
