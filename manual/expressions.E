.SP 5
.SH
2 LUCID EXPRESSIONS
.SH
2.1 THE where CLAUSE
.PP
The @where clause is pLucid's means of structuring programs,
just as the !block is the means in Algol and Pascal.
As mentioned earlier a pLucid program is an expression,
and is thus a composition of subexpressions.
To avoid horrendously long and complicated expressions
we use the @where clause to replace subexpressions by
variables.
For example, the following three programs are all equivalent.
.DS
     i.   @@(x ** y) / (x**y div z)&

     ii.  @@temp / (temp div z) where temp  = x ** y; end&

     iii. @@temp1 / temp2
                          @@where
                                  @@temp1 = x ** y;
                                  @@temp2 = temp1 div z;
                          @@end
.DE
Moreover, @where clauses may be nested to arbitrary
depths. For example the following
two programs are equivalent:
.DS
    i.  @@(x-2)*(x+3)+y+z&

    ii. @@a+b
             @@where
                 @@a = w1*w2
                           @@where
                              @@w1 = x-2;
                              @@w2 = x+3;
                           @@end;
                 @@b = y+z;
             @@end&
.DE
In this last program, the expression @a+b is called
the !!subject part& of the (outermost) @where clause.
The !body of this @where clause consists of two !!definitions&,
the first defining @@a&, and the second defining @@b&.
The subject part of the innermost @where clause
is @@w1*w2&, and its body consists of two definitions,
namely those of @w1 and @@w2&.

.SH
2.2 THE OPERATORS next, fby and first
.PP
Variables in pLucid are similar to variables
in most conventional programming languages.
They are dynamic objects in the sense that
their values change as the program execution proceeds.
Often, in the execution of a particular @where clause, it is possible
to think of an execution as a
sequence of steps in which the
values of the local variables of the @where are updated
simultaneously.
For example, suppose we wish to write a program
to read in a stream of numbers and output the
partial sums i.e. after each number is read
the sum of the numbers read so far is output.
For this we can use two variables, one called @@i&,
which at any step holds the last number read in,
and another called @@s&,
which at any step holds the sum of the numbers
read so far.
At each step in the program execution the variables @i and @s
are updated.
At any step the next value of @i is the next
value to be read, while the next value
of @s is the present value of @s plus the next value
of @@i&.
In pLucid @s is defined as
follows:
.PH
s = i fby s+next i
.PE
This definition is read:
The first value of @s is the first value of @@i&,
while at any step in the program execution, the next value of @s
is the present value of @s plus the next value of @@i&.
The complete pLucid program to generate the
stream @s of partial sums is:
.PR
s where
   s = i fby s+next i;
  end
.PE
This program uses the two pLucid operators @next
and @fby (pronounced "followed by") which we will now introduce.
@next is a prefix operator which, when applied to a variable
at any step in the execution, returns the value which
that variable has after the next step of the execution.
Of course in a conventional language
we do not know what the next value of a variable
will be; however in pLucid this value can be computed from the
definition.
For example, suppose that a variable @x is defined in terms of
a variable @y by the pLucid definition,
.PH
x = next y
.PE
then at any step in the program execution
the value of @x will be the next
value of @y i.e. the value of @y after the next  execution step.
Hence if, as we go through the execution steps,
@y takes on successive values from the
stream 2,4,6,8,10,..., then @x takes on the successive values
from the stream 4,6,8,10,...
Thus, @x is 4 when @y is 2, @x is 6 when @y is 4, and so on.
.PP
As well as being able to talk about the next value
of a variable we can also talk about the next value of
an expression.
For example, suppose @x and @y are as above, then at
any step the next value of @@x+y& will be the sum of
the next values of @x and @@y&.
So, if @z is a variable such that
at any step @z is the
next value of @@x+y&, then in pLucid @z is defined by:
.PH
z = next(x+y)
.PE
.PP
Let us now turn our attention to
the infix operator @@fby&.
As described earlier, in pLucid we regard variables
as dynamic objects, dynamic in the sense that
their values change as the program execution proceeds.
In the programs we have introduced so far,  the
values of all the variables
are simultaneously updated at each computation step.
In fact, for each variable we can talk about the
"stream of values" it assumes during the course of a program
execution.
For example, we can interpret statements such as, "the variable
@x takes the values 2 followed by 4, followed by 6 etc.",
to mean that after the first execution step @x is 2,
after the second step @x is 4, after the third step
@x is 6, and so on.
.PP
In general the infix operator @fby may have the following form:
.IB
x = <!!expression1&> fby <!!expression2&>
.IE
This can be read as follows :
The stream of values of @x is the initial value
of the <!!expression1&> followed by each of the successive values of
the <!!expression2&>.
An alternative reading is:
The initial value of @x is the initial value of the
<!!expression1&>,
and at any step in the program execution, the next
value of @x is the present value of <!!expression2&>.
.PP
The final operator to be introduced in this section is the
prefix operator called @@first&.
For any expression <!!expr&>, if the variable @x is defined by
.DS
@x   @=   @first  <!!expr&>
.DE
then at any step in the program execution, the value
of @x is the first (i.e. initial) value of <!!expr&>.
For example, suppose @int is a variable having the values
0 followed by 1, followed by 2,
followed by 3, etc.
Then the expression, @@first int&,
takes the values, 0 followed 0, followed by 0 ,etc.
in other words, @@first int& is equivalent to the constant @@0&.
.PP
Now that @@next&, @fby and @first have been
introduced we consider examples of their use.
The first example
is a filter that produces as output
the stream of integers 0,1,2,..etc.
.PR
int where
     int = 0 fby 1+int;
    end
.PE
The program execution begins by producing
the initial value of @@int&, i.e. 0.
From then on, the execution repeatedly
adds 1 to @int and outputs the result.
The filter just described is so useful that the current
Unix implementation includes a predefined variable called
@index that produces the same values as the above filter. Thus
the following program:
.PR
index
.PE
is equivalent to the one above. Moreover each @where clause
has its own private @index variable.
.PP
The
next example is a filter that produces as output
the stream of squares 0,1,4,9,16,...etc.
.PR
sq where
    int = 0 fby 1+int;
    sq  = 0 fby sq+2*int+1;
   end
.PE
As in the previous program the variable
@int takes on the successive values 0,1,2,3,...
The first value of @sq (i.e. the square of 0)
is 0, while at any step the next value
of @sq is the present value of @sq plus
two times the present value of @int plus 1.
Note that we have used the fact that for any n

.IB
(n+1)*(n+1) = n*n+2*n+1
.IE
.PP
The next example is a filter that
uses Newton's algorithm to
output a stream of approximations to
the square root of the input.
Roughly speaking the algorithm goes as follows, to
calculate the square root of a number n, take the first
approximation to be 1, and thereafter take each successive approximation
to be @@(x + n/x)/2&. In pLucid we might code this up as follows:
.PR
approx
  where
    approx = 1 fby (approx+first n/approx)/2;
  end
.PE
(For improvements on this example see sections 2.4 & 2.5)
