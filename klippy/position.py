# class for coordinate position
#
# Copyright (C) 2022-2023  Andrew Mirsky <andrew@mirsky.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
import sys

class Position(object):
    '''
        _mutable_ namedtuple-like object for storing coordinate values

        more efficient in terms of memory space and speed
         of access than standard class
         see : https://wiki.python.org/moin/UsingSlots

        drop-in replacement for positions currently represented by tuples,
         lists or `Coord`s as it allows access by indexing but also
         enables 'pythonic' access via attribute names

        >> p = Position(1,2,3,4)
        >> p = Position(x=1, y=2, z=3, e=4)
        >> print(p)
        Position<x: 1.000, y: 2.000, z: 3.000, e: 4.000>

        >> p[0] = 5
        >> p['y'] = 6
        >> p['Z'] = 7
        >> p.e += 4

        >> for v in p:
        >> .   print(v)
        5
        6
        7
        8

        >> list(p)
        [5, 6, 7, 8]

        >> Position.axes()
        ('x', 'y', 'z', 'e')

        >> for i,k in enumerate('XYZ'):
        >> .   print("{} : {}".format(k, p[i]))
        X: 5
        Y: 6
        Z: 7

        >> for k in 'XYZ':
        >> .   print("{} {}".format(k, p[k]))
        X: 5
        Y: 6
        Z: 7

        >> for k,v in p.items():
        >> .   print("{}: {}".format(k,v))
        x: 5
        y: 6
        z: 7
        e: 8

        >> p1 = Position(x=1, y=1, z=1, e=1)
        >> p2 = Position(x=2, y=2, z=2, e=2)
        >> p1 + p2
        Position<x: 3.000, y: 3.000, z: 3.000, e: 3.000>
        >> p1 - p2
        Position<x: -1.000, y: -1.000, z: -1.000, e: -1.000>
        >> p2 * -1
        Position<x: -2.000, y: -2.000, z: -2.000, e: -2.000>
    '''
    __slots__ = ('x', 'y', 'z', 'e')
    # __map__ = {idx: key for idx, key in enumerate(__slots__)}

    def __init__(self, x=None, y=None, z=None, e=None):
        self.x = x
        self.y = y
        self.z = z
        self.e = e

    def __getitem__(self, key):
        if type(key) is int and key < len(self):
            return self.__getattribute__(self.__slots__[key])
        if type(key) is str and key.lower() in self.__slots__:
            return self.__getattribute__(key.lower())
        raise KeyError

    # if sys.version_info[0] < 3:
    #     def __getattribute__(self, item):
    #         return getattr(self, item)

    def __setitem__(self, key, val):
        if type(key) is int and key < len(self):
            self.__setattr__(self.__slots__[key], val)
        elif type(key) is str and key.lower() in self.__slots__:
            self.__setattr__(key.lower(), val)
        else:
            raise KeyError

    class PositionTupleIter:
        def __init__(self, position):
            self._position = position
            self._idx = 0

        def __next__(self):
            if self._idx < len(self._position):
                self._idx += 1
                return self._position[self._idx-1]
            raise StopIteration

    def __iter__(self):
        return self.PositionTupleIter(self)

    def __len__(self):
        return len(self.__slots__)

    @property
    def __dict__(self):
        return {a: self[a] for a in self.__slots__}

    def __getstate__(self):
        return self.__dict__

    def __setstate__(self, d):
        for a in self.__slots__:
            self[a] = d[a]

    @classmethod
    def axes(cls):
        return cls.__slots__

    def keys(self):
        return self.__slots__

    # memory efficient helper for items()
    class PositionDictIter:
        def __init__(self, position):
            self._position = position
            self._idx = 0

        def __iter__(self):
            return self

        # needed for python 2 compatibility
        def next(self):
            return self.__next__()

        def __next__(self):
            if self._idx < len(self._position):
                self._idx += 1
                return (self._position.__slots__[self._idx-1],
                        self._position[self._idx-1])
            raise StopIteration

    def items(self):
        return self.PositionDictIter(self)

    __format__ = "Position<x: {:03.3f}, y: {:03.3f}, z: {:03.3f}, e: {:03.3f}>"
    def __str__(self):
        return self.__format__.format(self.x, self.y, self.z, self.e)

    def __add__(self, b):
        return Position(self.x+b.x, self.y+b.y, self.z+b.z, self.e+b.e)

    def __sub__(self, b):
        return Position(self.x-b.x, self.y-b.y, self.z-b.z, self.e-b.e)

    def __mul__(self, m):
        return Position(self.x * m, self.y * m, self.z * m, self.e * m)

    __rmul__ = __mul__
