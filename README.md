libdeck
=======

libdeck is a simple C library for implementing card games. Although
built in a generic way so that any card game can be implemented with it,
currently only Texas Hold'em poker is built in - hands can be classified
(such as pair, two pair, three of a kind etc.), compared against other
hands, and winning odds can be calculated.

## Building

libdeck uses autotools, so it can be configured and built as follows:

    $ aclocal
    $ autoconf
    $ autoheader
    $ libtoolize
    $ automake -a
    $ ./configure
    $ make

## Usage

The library comes with a test application called 'testapp'. This
application demonstrates library usage by simulating one round of a
3 player Texas Hold'em game, with random initial hands, a flop, turn
and river while also calculating winning odds of each hands at each
step of the way. Example output:

    $ src/testapp
    Shuffled deck = {7d, 5c, 10d, 7s, 8s, 4s, 4c, Ks, 9c, 8d, Ad, As, 9h,
    6c, 10c, 4h, Kd, 7c, 8c, 9d, 3s, 4d, 5d, 3h, 6s, 5s, Qc, Kh, Jh, Qd,
    2d, Jd, Jc, 8h, Ac, 6h, Kc, Qh, 2c, 3c, Qs, Js, 2h, 9s, 5h, 10s, 6d,
    3d, 7h, 10h, Ah, 2s}[52]
    
    Hand 1 = {Ah, 2s}[2]
    Hand 2 = {7h, 10h}[2]
    Hand 3 = {6d, 3d}[2]
    
    Pre-flop odds of winning:
    Hand1: 32%
    Hand2: 37%
    Hand3: 28%
    
    Flop = {9s, 5h, 10s}[3]
    Post-flop odds of winning:
    Hand1: 17%
    Hand2: 77%
    Hand3: 5%
    
    Turn = {2h}[1]
    Post-turn odds of winning:
    Hand1: 11%
    Hand2: 80%
    Hand3: 7%
    
    River = {Js}[1]
    
    Community = {9s, 5h, 10s, 2h, Js}[5]
    
    Hand 1 made = {2s, 2h, Ah, Js, 10s}[5] = Pair of 2's
    Hand 2 made = {10h, 10s, Js, 9s, 7h}[5] = Pair of 10's
    Hand 3 made = {Js, 10s, 9s, 6d, 5h}[5] = J high
    
    Hand 2 wins!
