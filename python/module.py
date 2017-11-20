#! /usr/bin/python2

from collections import Counter
#from itertools import izip as zip
#import functools -> permet d'importer uniquement __eq__ et __lt__ au lieu de cmp

from six.moves import zip, map

"""
    args will be a tuple
    kargs will be a dictionary
"""
def function_name(arg, *args, **kargs):
    return


def factoriel(entier):
    if entier == 0:
        return 1
    return entier * factoriel(entier - 1)


def aux_fact(n, p=1):
    if p == 0:
        return 0
    if n == 0:
        return p
    return aux_fact(n-1, p=p*n)


def fact(n):
    return aux_fact(n)


def all_pairs(obj_iter):
    pair_list = []
    for i in obj_iter:
        if i % 2 == 0:
            pair_list.append(i)
    return pair_list


def all_pairs2(*args):
    pair_list = []
    for i in args:
        if i % 2 == 0:
            pair_list.append(i)
    return pair_list


def rot13(chaine_caracteres):
    return_str = ""
    for letter in chaine_caracteres:
        if letter.isalpha():
            if letter.upper() < "N":
                return_str += chr(ord(letter)+13)
            else:
                return_str += chr(ord(letter)-13)
        else:
            return_str += letter
    return return_str

#too many stuff for interpretor, prefer using values of ord("a")
def rot13_draft(chaine_caracteres):
    return_str = ""
    for letter in chaine_caracteres:
        if letter.isalpha():
            if letter.islower():
                return_str += chr((ord(letter)-ord("a"))%26+ord("a"))
            else:
                return_str += chr((ord(letter)-ord("A"))%26+ord("A"))
        else:
            return_str += letter
    return return_str


def set_of_words(filename):
    myset = set()
    with open(filename) as f:
        for line in f:
            #myset.update(line.split())
            for word in line.split():
                myset.add(word)
    return myset


def dict_of_words(filename):
    #mydict={}
    mydict = dict()
    with open(filename) as f:
        for line in f:
            for word in line.split():
                mydict[word] = mydict.get(word, 0) + 1
    return mydict


def generator_fx(f, x):
    while True:
        yield x
        x = f(x)


def set_of_words_short(filename):
    with open(filename) as f:
        return set(word for line in f for word in line.split())


def dict_of_words_short(filename):
    #mydict={}
    with open(filename) as f:
        return Counter(word for line in f for word in line.split())


class Vector(object):
    def __init__(self, *coord):
        self.coordinates = coord

    def __len__(self):
        return len(self.coordinates)

    def __abs__(self):
        return sum(i ** 2 for i in self.coordinates) ** .5

    def __repr__(self):
        return "%s(%s)" % (
                self.__class__.__name__,
                ", ".join(repr(i) for i in self.coordinates),
                )

    def __eq__(self, other):
        if isinstance(other, Vector):
            return self.coordinates == other.coordinates
        else:
            return False

    def __ne__(self, other):
        return not (self == other)

    def _check_ops(self, other):
        if not isinstance(other, Vector):
            return False
        if len(self) != len(other):
            raise TypeError('Unsupported operand: need same vector length')
        return True

    def __add__(self, other):
        if not self._check_ops(other):
            return NotImplemented
        return Vector(*(i+j for i, j in zip(self.coordinates, other.coordinates)))

    def __rmul__(self, other):
        return Vector(*(i * other for i in self.coordinates))

    def __mul__(self, other):
        if not self._check_ops(other):
            return NotImplemented
        return sum(i*j for i, j in zip(self.coordinates, other.coordinates))

    def __cmp__(self, other):
        if not self._check_ops(other):
            return NotImplemented
        for i, j in zip(self.coordinates, other.coordinates):
            if i < j:
                return -1
            if i > j:
                return 1
        return 0


def cachefunc(f):
    res = {}
    def func(*args):
        if args in res:
            return res[args]
        res2 = res[args] = f(*args)
        return res2
    return func


@cachefunc
def test(x):
    print "appel"
    return x if x else None



if __name__ == '__main__':
    print all_pairs([1, 2, 3, 6, 5, 8, 4, 4, 7])
    print all_pairs2(1, 2, 3, 6, 5, 8, 4, 4, 7)
    print all_pairs2(*xrange(10))
    print fact(6)
    print aux_fact(7, 4)
    print factoriel(12)
    print rot13("abcdef")
    print rot13(rot13("abcdef"))
    print rot13("aiohfgshkljgh65454_'+,_/NANAR")
    print set_of_words("monjolifichier.txt")
    print dict_of_words("monjolifichier.txt")
    print set_of_words_short("monjolifichier.txt")
    print dict_of_words_short("monjolifichier.txt")

    for v in generator_fx(lambda x: x + 2, 0):
        print v
        if v > 10:
            break

    mylist = [x ** 2 for x in xrange(20) if x % 2 == 0]
    mydict = {x: x ** 2 for x in xrange(20) if x % 2 == 0}
    myset = {x ** 2 for x in xrange(20) if x % 2 == 0}
    #generateur
    (x ** 2 for x in xrange(20) if x % 2 == 0)
    tuple(x ** 2 for x in xrange(20))
    frozenset(x ** 2 for x in xrange(20))
    bytearray(chr(x) for x in xrange(20))
    #''.join(chr(x) for x in xrange(20))
    #u''.join(chr(x) for x in xrange(20))
    #s = set(cell for line in table for cell in line)

    v = Vector(-3, 4)
    v1 = Vector(1, 2)
    v2 = Vector(2, 3)
    print v
    print abs(v)
    print len(v)
    print v1 + v2
    print 2 * v1
    print v1 * v2
    print (v1 > v2)
