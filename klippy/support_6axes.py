# Code for support 6 axes
#
# This file may be distributed under the terms of the GNU GPLv3 license.
 
class Axes:
    @classmethod
    def extend(self, pos):
        if len(pos) > 4:
            return pos

        if isinstance(pos, tuple):
            if len(pos) == 4:
                return (pos[0], pos[1], pos[2], 0., 0., 0., pos[3])
            else:
                return (pos[0], pos[1], pos[2], 0., 0., 0.)

        if len(pos) == 4:
            pos.extend([0.,0.,pos[3]])
            pos[3] = 0.
        else:
            pos.extend([0., 0., 0.])

        return pos

    @classmethod
    def shrink(self, pos):
        if len(pos) <= 4:
            return pos
        
        if isinstance(pos, tuple):
            if len(pos) == 7:
                return (pos[0], pos[1], pos[2], pos[6])
            else:
                return (pos[0], pos[1], pos[2])

        e = pos.pop()
        pos.pop()
        pos.pop()
        if len(pos) == 4:
            pos[3] = e

        return pos
