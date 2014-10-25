.SP 8
.SH
2.3 USER DEFINED FUNCTIONS  (UDF'S)
.PP
Pointwise functions (or operators) in pLucid are similar to  pure functions
(i.e. functions without side effects)
in  other  programming  languages.   For  example,  the following
program repeatedly reads in integers and outputs the
corresponding factorial.
.PR
fac(n)
  where
    fac(n) = if n eq 0 then 1 else n*fac(n-1) fi;
  end
.PE
The function @fac defined in the body of the @where clause  can  be
thought of as a machine or !!black box& which continuously consumes
values of @n and produces values of @@fac(n)&.
In other words it is a filter in the UNIX sense, the only
difference being that this !!black box& filters integers rather
than characters. The following is a sequence of snapshots for a
particular sequence of inputs:
.IB
      |           |           |           |           |
      | 1         | 3         |  4        | 2         |
      V           |           |           |           |
   +--+--+     +--+--+     +--+--+     +--+--+     +--+--+
   | fac | ==> | fac | ==> | fac | ==> | fac | ==> | fac |
   +--+--+     +--+--+     +--+--+     +--+--+     +--+--+
      |           |           |           | 24        | 2
      |           | 1         |  6        | 6         | 24
      |           |           |  1        | 1         | 6
      V           V           V           V           | 1
                                                      V
.IE
The filter produces each of the factorials @@fac(n)&
1,6,24,2 as @n takes each of the values
1,3,4,2. However pLucid programmers are not restricted to only
pointwise filters. A pLucid filter may have internal memory
and/or
it may produce outputs at a different to that at which it
consumes inputs.
.PP
The following non-recursive udf has internal memory. It
computes the average of the last three numbers input.
.PR
  avg3(input)
     where
        avg3(n) = avg
                   where
                     avg = (one+two+three)/3;
                     one = n;
                     two = next n;
                     three = next next n;
                     // This is a comment
                     // A simpler way of writing this whole
                     // where clause would be as the following 
                     // expression
                     // (n + next n + next next n)/3
                   end;
     end
.PE
.PP
Another example of a program using a udf  is  one  which
reads  in  a stream of finite
lists  and  outputs a stream of the atoms in those lists, e.g.
if the
list  [1, [3.7], true]  is input, then the atoms 1, 3.7, and true
will be output.
.PR
flatseq(x)
  where
       flat(x) = if x eq nil then nil
                  elseif isatom( x ) then x fby nil
                      else join(flat(hd x),flat(tl x))
                 fi;
     join(x,y) = if first x eq nil then y
                    else x fby join(next x,y)
                 fi;
    flatseq(x) = join(flat(first x),flatseq(next x));
  end
.PE
The function @flatseq consumes a stream of finite lists and  produces
a stream of all
the  atoms in those lists, while the function @flat consumes just one
list and produces its  atoms.   The  function  join  consumes  two
streams  of  atoms  for example  1,2,[],[],[],...  and
3,4.75,[],[],[],...  and produces the  stream
1,2,3,4.75,[],[],[],...
The filters
@@flat&, @join and @flatseq could in principle
be implemented as three concurrently
running processes. Note that
@flatseq produces values faster than it consumes them, as
for each list going into @flatseq many atoms may be produced as
output.
.PP
Another example of a simple program with functions is the
following. The program
generates the prime numbers using the !!sieve of Eratosthenes&.
.PR
sieve(n)
  where
   n        = 2 fby n+1;
   sieve(i) = i fby sieve(i wheneve i mod first i ne 0);
  end
.PE
.SH
2.4 THE OPERATORS asa,whenever,upon and attime
.SH
asa
.PP
If @asa were not a predefined infix operator it could be
defined as follows:
.PH
 asa(x,y) =if first y then first x
                else  asa(next x,next y) fi
.PE
Althought this is a concise and precise way of defining @asa it
may be better at this stage to give a more operational
description, such as the following.
The @asa operator computes by repeatedly reading in pairs of values
of @x and @y until (if ever) @y has the value @@true&. Assuming
a @true is eventually read then the value taken by the @@asa&
operator will be the value of @x corresponding to the @@true&
value of @@y&. Moreover the @asa takes on this value everywhere.
In this respect the @asa operator can be thought of as a constant.
For example, if @x takes the values
0,1,2 and @y the values false, false, true
then @@x asa y& takes the values 2,2,2,...
Another example is if @x takes the values abc, 3, [d,e]
and @y the values false,
true, false, then  @@x asa y& produces as output the
stream of values 3,3,3,...
.PP
The following program illustrates a use of the @asa operator.
The program is
a slightly different version of
the Newton's algorithm program of Section 2.2.
.PR
(approx asa count eq 10) fby eod
  where
    approx = 1 fby (approx+first n/approx)/2;
    count  = 1 fby count+1;
  end
.PE
Given a number 42 say as input (i.e. the variable @n is a free
variable in this program and is therefore assumed to be an input
variable),
the program
outputs the 10th approximation to its square root
i.e. 6.48.
.SH
whenever
.PP
The predined infix operator @whenever (sometimes written @@wvr&)
could be defined recursively as follows:
.PH
whenever(x,y) = if first y then x fby Z else Z fi
                       where
                         Z = whenever(next x,next y);
                       end
.PE
Again, at this stage,
a more operational description will be of more benefit to
the reader.
The operator @whenever is best thought of as a process that
filters out some
elements (possibly none) from the stream @@x&
according to a control stream (i.e. booleans) @@y&.
In terms of machines (see the factorial example in
section 2.3) @whenever repeatedly takes pairs of values
for @x and @@y&; if @y is true then @x is output
and if @y is false then @x is discarded.
For example suppose at each step in the computation @@y&
takes on successive values from the
stream true,false,true,false,...
i.e.
.PH
y = true fby false fby y
.PE
In addition suppose at each step in the computation @@x&
takes on successive values from the stream 0,1,2,...   i.e.
.PH
x = 0 fby x+1
.PE
If @x and @y take on successive values as
described above then, @@x whenever y& will produce as output
successive values from the stream 0,2,4,6,... of even
numbers. The following is a pictorial description of the
computation:
.IB
     |   |         |   |         |   |         |   |
     |   |         |   |         |   |         |   |
   0 |   | true  1 |   | false 2 |   | true    |   | false
     v   v         v   v         v   v         v   V
   +-+---+-+     +-+---+-+     +-+---+-+     +-+---+-+
   |  wvr  | ==> |  wvr  | ==> |  wvr  | ==> |  wvr  |  ==> Ad infinitum
   +---+---+     +---+---+     +---+---+     +---+---+
       |             |             |             |
       |             | 0           | 0           |  2
       |             |             |             |  0
       v             v             v             V
.IE
(For another example of whenever see the
prime number program of Section 2.3)
.SH
upon
.PP
The @whenever operator is best thought of as a filter that
selectively passes on values that it receives as its input.
The Lucid operator @upon is a dual to @@whenever&
in the sense that it selectively stretches out the values that is receives
on its input. Again we can give a concise definition of @@upon&
in terms of a recursive user defined function as follows:
.PH
upon(x,y) = x fby if first y then upon(next x,next y) else upon(x,next y) fi
.PE
If we think of @upon as a black box then we can describe its
behaviour as follows. The first output of the box is whatever
shows up as the first value of @x. If the corresponding value
for @y is @true then the box will advance looking for the next
value of @@x&. However if the corresponding value of @y is
@false the next value output will again be the current value of
@@x&. Thus our black box' output is controlled by the boolean
values sent along @@y&.
The following is an example of a program to stretch out the
stream 0,1,2,3,.. of natural numbers.
.PR
 stretch where
            x = 0 fby x+1;
            y = true fby false fby y;
            stretch = x upon y;
         end;
.PE
The following is a pictorial description of the computation:
.IB
     |   |         |   |         |   |         |   |
     |   |         |   |         |   |         |   |
   0 |   | true  1 |   | false 2 |   | true  3 |   | false
     v   v         v   v         v   v         v   V
   +-+---+-+     +-+---+-+     +-+---+-+     +-+---+-+
   |  upon | ==> |  upon | ==> |  upon | ==> |  upon |  ==> continued
   +---+---+     +---+---+     +---+---+     +---+---+
       |             |             |             |
       |             | 0           | 1           |  1
       |             |             | 0           |  1
       v             v             v             v  0
.IE
.IB
     |   |         |   |         |   |         |   |
     |   |         |   |         |   |         |   |
   4 |   | true  5 |   | false 6 |   | true  7 |   | false
     v   v         v   v         v   v         v   V
   +-+---+-+     +-+---+-+     +-+---+-+     +-+---+-+
   |  upon | ==> |  upon | ==> |  upon | ==> |  upon |  ==> Ad inifinitum
   +---+---+     +---+---+     +---+---+     +---+---+
       |             |             | 3           |  3
       | 2           | 2           | 2           |  3
       | 1           | 2           | 2           |  2
       | 1           | 1           | 1           |  2
       | 0           | 1           | 1           |  1
       |             | 0           | 0           |  1
       v             v 0           v 0           |  0
                                                 v  0
.IE
Another example of a program that uses @upon is the following
merge program. The streams to be merged (i.e @xx and @@yy&)
are assumed to be streams of numbers in increasing order. Let
define the stream associated with @xx to be
2,4,8,16,32,64..., and the stream associated with @@yy&
to be 3,9,27,81,.... Then the ordered merge of these two streams
will be will be the
stream 2,3,4,8,9,16,27,32,64,81,.... The following is the
pLucid program that computes this ordered merge:
.PR
    merge
      where
        merge = if a<b then a else b fi;
        a = xx upon a eq merge;
        b = yy upon b eq merge;
        xx = 2**i;
        yy = 3**i;
        i = 1 fby i+1;
      end;
.PE
.SH
attime
.PP
The last operator (filter) to be described
@attime. If this was not a predifined infix operator we
could define it with the following udf:
.PH
  attime(x,y) = (x asa index eq first y ) fby attime(x, next y);
.PE
Note @index has been defined in section 2.2. In fact using
@attime it is possible to give the following non-recursive definition for
@upon and @whenever.
.PH
    wvr(x,p) = x attime t2
                  where
                     t1 = if p then index else next t1 fi;
                     t2 = t1 fby t1 attime t2+1;
                  end;
.PE
.PH
    upon(x,p) = x attime t1
                  where
                    t1 = 0 fby if p then t1+1 else t1 fi;
                  end;
.PE

.SH
2.5 THE is current DECLARATION
.PP
In this section we introduce a pLucid construct that allows the
programmer to write programs which involve nested iteration.
This construct is the @is @current declaration.
.PP
Suppose we wish to write a program which repeatedly
takes pairs of non-negative integers and raises
the first to the power of the second, e.g.
2 to the power of 3 is 8.
A typical approach to designing such a program would be
firstly to write a smaller program to produce
the power of just one pair of inputs, and secondly to embed
this program in another that executes the first program
indefinitely.
In pLucid the first program could be written as follows:
.PR
p asa index eq first n
     where
       p = 1 fby p * first x;
     end
.PE
It is a simple loop program where, each time round
the loop, @p is multiplied by the first value of @@x&.
The loop continues until a variable that we have chosen
to act as the loop counter, namely @@i&, equals
the first value of @@n&, after which the value of
@p is the first value of @x raised to the
power of the first value of @@n&.
We now attempt to place
this program within a nonterminating loop program.
The resulting program has two loops,
one nested within the other; however whenever
the inner loop (i.e. the above program) is executed,
the expressions @@first n& and @@first x& would always have the same value.
Consequently the power of only the first pair
of inputs would be repeatedly computed.
Obviously this is not what we require.
We can try to resolve the situation by
replacing @@first n& and @@first x& by @n and @x respectively.
Unfortunately this does not help either since we require @n and
@x to remain constant during each execution of
the inner loop.
Thus we require that @n and @x be outer loop variables
which only change between executions of the
inner loop.
This is clearly impossible when we think
of @@x&, @@n&, @@index&, and @p as variables which are all
updated simultaneously.
To overcome this problem we use the @@is current
declaration with @n and @x in the above program.
The effect of this is to set up an outer loop
in which @x and @n only get updated
between executions of the inner loop.
The resulting program is the following:
.PR
p asa index eq first N
     where
       X is current x;
       N is current n;
       p = 1 fby p * first X;
     end
.PE
Note the informal convention used i.e. that the variables that
are introduced by an @is @current declaration use upper case.
Although any variable name could have been used, the use of upper case
letters makes programs involving nested iterations easier to understand.
The inner loop variable @X only ever has one value
which is the current value of @@x&, hence as @index and @p
get updated,
@X stays constant.
Similarly @N, the current value of @@n&, stays constant
during the execution of the inner loop.
Remember, @x and @n are outer loop variables, while
@X and @N are inner loop variables which restart
life anew at the beginning of each inner loop
execution.
In general, the effect of the @@is current&
declaration is to set up a nested iteration.
.PP
An improvement in the above program is to
replace @@first X& by @X and @@first N& by @@N&.
This does not change the meaning of the program
as @X and @N remain constant during each execution
of the inner loop.
This results in the following program:
.PR
p asa index eq N
     where
       X is current x;
       N is current n;
       p = 1 fby p * X;
     end
.PE
.PP
Now, suppose we had wanted to write a program
which took a power (the first value of @@n&) and
then raised each of a sequence of inputs to
that power e.g. it might compute the squares
of all  its inputs.
The program would then use @@is current& with
@x but not with @@n&.
.PR
p asa index eq first n
     where
       X is current x;
       p = index fby p*X;
     end
.PE
Note that the expression @@first n& cannot be replaced by @@n&
as @n is a variable that is active in the inner loop, and
we require @n to be the same for each execution
of the inner loop.
In a similar way we can write a program which takes an
integer (the first value of @@x&) and raises
it to each of a sequence of powers e.g.
it might compute powers of 2.
The program uses @@is current& with @n and not @@x&.
.PR
p asa index  eq N
     where
       N is current n;
       p = 1 fby p * first x;
     end
.PE
As with @n in the previous program, we cannot
replace @@first x&   by @@x&.
.PP
Another example of a program using @@is current&
is one to calculate the exponential $${ e sup x}& using the
series
.EQ
{ e sup x }~=~1~+~x~
+~{{x sup 2} over 2!}
~+~{{x sup 3} over 3!}~+~...
.EN
The program is
.PR
expsum asa next i eq 10
          where
            X      is current x;
            i      = next index;
            term   = 1 fby (term/i)*X;
            expsum = 0 fby expsum+term;
          end
.PE
The body of the outer loop asks for the present value
of @x to be input, and then outputs an approximation to
$${ e sup x}&.
The inner loop takes the current value of @x i.e. @@X&
and computes an approximation to
$${ e sup x}& by adding the
first ten terms of the above series.
The body of the inner loop computes the next
term of the series and adds it to @expsum.
Hence each time the inner loop is called
it executes its body ten times.
If the stream of values 1, 2, 0.5, -1 are the inputs for @x
then the stream
2.718, 7.389, 1.649, 0.369 will be output.
.PP
The next example is an improvement of
Newton's algorithm for computing a square root given
in section 2.1.
Given a stream of numbers we compute approximations
to their roots by applying Newton's algorithm to
each of them 10 times.
.PR
sqroot where
        X is current x;
        sqroot = approx asa count eq 10
                 where
                   approx = 1 fby (approx+ X/approx)/2;
                   count  = 1 fby count+1;
                 end;
       end
.DE
Inputting the numbers 2, 2.213, 26.7 produces
the stream 1.414, 1.487, 5.167 as output.
.PP
The general form of the @@is current& declaration is:
.IB
 <!!variable&> @is @current <!!expression&>
.IE
Our next example uses @@is current& in its more
general form, namely with an expression on the rhs.
It computes the roots of the polynomial
.EQ
a~{ x sup 2}~+~b~x~+~c~=~0
.EN
in the obvious way using the previous example.
The roots are output as a two element list.
.PR
r1::r2::nil
    where
      r1     =  (-b+sqroot)/(2*a);
      r2     =  (-b-sqroot)/(2*a);
      sqroot =  approx  asa  count eq 10
                 where
                   X is current b*b-4*a*c;
                   approx = 1 fby (approx+X/approx)/2;
                   count  = 1 fby count+1;
                 end ;
    end
.PE
