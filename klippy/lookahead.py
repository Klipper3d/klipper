# Move queue look-ahead
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class MoveQueue:
    def __init__(self, dummy_move):
        self.dummy_move = dummy_move
        self.queue = []
        self.prev_junction_max = 0.
        self.junction_flush = 0.
    def prev_move(self):
        if self.queue:
            return self.queue[-1]
        return self.dummy_move
    def flush(self, lazy=False):
        next_junction_max = 0.
        can_flush = not lazy
        flush_count = len(self.queue)
        junction_end = [None] * flush_count
        for i in range(len(self.queue)-1, -1, -1):
            move = self.queue[i]
            junction_end[i] = next_junction_max
            if not can_flush:
                flush_count -= 1
            next_junction_max = next_junction_max + move.junction_delta
            if next_junction_max >= move.junction_start_max:
                next_junction_max = move.junction_start_max
                can_flush = True
        prev_junction_max = self.prev_junction_max
        for i in range(flush_count):
            move = self.queue[i]
            next_junction_max = min(prev_junction_max + move.junction_delta
                                    , junction_end[i])
            move.process(prev_junction_max, next_junction_max)
            prev_junction_max = next_junction_max
        del self.queue[:flush_count]
        self.prev_junction_max = prev_junction_max
        self.junction_flush = 0.
        if self.queue:
            self.junction_flush = self.queue[-1].junction_max
    def add_move(self, move):
        self.queue.append(move)
        if len(self.queue) == 1:
            self.junction_flush = move.junction_max
            return
        self.junction_flush -= move.junction_delta
        if self.junction_flush <= 0.:
            self.flush(lazy=True)
