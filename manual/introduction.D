.SP 1
.SH
1 INTRODUCTION
.PP
The language !pLucid can best be described as a functional
dataflow programming language. We use the term !dataflow
because a
!pLucid program defines a dataflow net and we use the term
!functional because for each node in the net,
the entire output history is a function of the entire input
history.
.PP
A brief look at !pLucid's
pedigree shows that its block structuring mechanism is the @where
clause from P.J.Landin's ISWIM, and that its data types are those of
Pop2, i.e. integers, reals, booleans, words, character strings
and finite lists.
The language pLucid is !typeless and so
there are neither type declarations nor compile-time type checking.
Those programmers who are familiar with programming the UNIX shell will
feel at home programming in pLucid. The reason for this is that
basic to both pLucid and UNIX are the concepts of
!filter and !!data stream&.
.PP
A pLucid program is !!an expression&, and a
program execution is an !!expression evaluation&.
For example, consider the simple expression
.PR
x  +  y
.PE
which constitutes a complete pLucid program.
When evaluated this program repeatedly demands
values for @x and @y and outputs their sum.
Note that the free variables in a pLucid expression are assumed to be
inputs.
The above program induces an endless
interaction between the user and the evaluator that might
proceed as follows:
.IB
     x( 0): 2            !!The first demand for a value of& x!!.&
     y( 0): 3            !!The first demand for a value of& y!!.&
output( 0): 5            !!The first value of the sum.&
     x( 1): 1            !!The second ......&
     y( 1): ~8
output( 1): ~7
     x( 2): 2.73         !!The third .....&
     y( 2): 1
output( 2): 3.73
     x( 3):              !!A fourth demand for x, for which the user&
                         !!has not yet supplied a value.&
.IE
In section 5 we explain how these apparently endless
computations can be made to terminate in a graceful manner.
.PP
Another example program is the following:
.PR
hd(  tl( L )  )
.PE
When this program is evaluated it repeatedly demands values
for L (which are assumed to be finite lists) and outputs
the head of their tail (i.e. the second element of the input
list).
This is another example of a continuously operating program
evaluation. For this program a
listing of the interaction between the user and the
evaluator of might
look as follows:
.IB
     L( 0): [42 7 5]
output( 0): 7
     L( 1): [3 [2.4 8] 9]
output( 1): [2.4 8]
     L( 2): [2]
output( 2): ?         !!(the pLucid error object) &
     L( 3): [5 1 2]
output( 3): 1
        .   .
        .   .
        .   .
.IE

Note that, given an input which is not a finite list
or a finite list consisting of at least one object, the resulting output
is the error value @@?&.
.PP
One important property shared by the above examples is that
they can all be thought of as filters.
The first example can be thought of as an addition filter,
i.e.  it takes
two streams of inputs and produces the stream of their sums.
The following seqeunce of snapshots of the dataflow
computation:
.IB
   |   |         |   |         |   |         |   |
 2 |   | 3       |   |       1 |   | ~8  2.73|   | 1
   V   V         V   V         V   V         V   V
 +-+---+-+     +-+---+-+     +-+---+-+     +-+---+-+
 |  plus | ==> |  plus | ==> |  plus | ==> |  plus | ==> continued
 +---+---+     +---+---+     +---+---+     +---+---+
     |             |  5          |             | ~7
     |             |             |  5          |  5
     V             V             V             V
.IE
note in pLucid negative numbers are prefixed by the symbol ~ as
in ~8 (minus 8).
.IB
   |   |         |   |
   |   |         |   |
   V   V         V   V
 +-+---+-+     +-+---+-+
 |  plus | ==> |  plus | ==> ...... Ad infinitum
 +---+---+     +---+---+
     | 3.73        | 3.73
     | ~7          | ~7
     V 5           V 5

.IE

Similarly the second example is a combination of two filters. The first
called the tail filter,
takes one input stream and produces the
stream of tails. The output of this filter is !piped to the input
of the second filter, namely the head filter. This second filter
produces as its output the heads of the sequence of finite
lists input.
The following is a sequence of snapshots of the computation:

.IB
      |                |                    |
      | [42 7 5]       | [3 [2.4 8] 9]      | [2]
      V                |                    |
   +--+---+         +--+---+             +--+---+
   |  tl  |         |  tl  |             |  tl  |
   +--+---+         +--+---+             +--+---+
      |        ==>     | [7 5]      ==>     | [[2.4 8] 9] ==> continued
      V                V                    V
   +--+---+         +--+---+             +--+---+
   |  hd  |         |  hd  |             |  hd  |
   +--+---+         +--+---+             +--+---+
      |                |                    | 7
      V                V                    V
      |                |                    |
.IE
.IB
      |                |                    |
      | [5 1 2]        |                    |
      V                V                    V
   +--+---+         +--+---+             +--+---+
   |  tl  |         |  tl  |             |  tl  |
   +--+---+         +--+---+             +--+---+
      |  []            |  [1 2]             |
      V        ==>     V         ==>        V        ==>  Ad infinitum
   +--+---+         +--+---+             +--+---+
   |  hd  |         |  hd  |             |  hd  |
   +--+---+         +--+---+             +--+---+
      |                |                    |   1
      |  [2.4 8]       |  error             |   error
      |  7             |  [2.4 8]           |   [2.4 8]
      V                V  7                 V   7
.IE
.PP
Simple filters like the above are usually memoryless, but
there is no reason why this should be the case for more
complex filters. It is
possible for a filter to produce as output values that
depend upon values already processed.
An example of such a filter is one that outputs the
running total of the numbers that it has received as input.
Another example of a filter with memory is one that takes
as input a sequence of numbers,
one at a time, and produces as output the smallest and the largest
of the numbers read so far. Initially the program reads the first
number and gives this number as both the smallest and the
largest read so far. Then it asks for the next input and if this
input is smaller than the smallest it replaces the old smallest
and in addition is output as the current smallest. If
this input is larger than the largest it becomes the largest and is
output as the current largest.
In the case of it being the same value as the current smallest
or the current largest the input is ignored.
Note that if the input is anything but a number, then the
output will be the special error value. This program can be written
in pLucid as
follows :
.PR
[% "smallest", s , "largest", h %]
        where
           s =  x  fby if next x <  s then next x else s fi;
           h =  x  fby if next x >  h then next x else h fi;
        end
.PE
A sequence of snapshots of a computation is illustrated in the
following diagram:
.IB
       |           |                            |
       |  4        | 5                          | 1
       v           V                            V
    +--+--+     +--+--+                      +--+--+
    |  P  | ==> |  P  |         ====>        |  P  |     ==> continued
    +--+--+     +--+--+                      +--+--+
       |           |                            |
       |           | [smallest 4 largest 4]     | [smallest 4 largest 5]
       |           |                            | [smallest 4 largest 4]
       V           V                            V
.IE
.IB

       |                            |
       | 0                          |
       V                            V
    +--+--+                      +--+--+
    |  P  |         =====>       |  P  |     ====>   Ad infinitum
    +--+--+                      +--+--+
       | [smallest 1 largest 5]     | [smallest 0 largest 5]
       | [smallest 4 largest 5]     | [smallest 1 largest 5]
       | [smallest 4 largest 4]     | [smallest 4 largest 5]
       V                            | [smallest 4 largest 4]
                                    V

.IE
