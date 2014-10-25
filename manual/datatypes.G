.SP 15
.SH
3
THE pLucid DATA TYPES
.PP
So far we have described the Lucid operators which
are available in pLucid. Such operators are common
to all members of the Lucid family of languages.
Each particular
member of the family is distinguished by the
choice of
data types on which the Lucid operators are
applied.
However, the members of the Lucid family
are typeless, in the sense that there are no
type declarations and no type checking at compile-time.
.PP
The data types of pLucid are integers, reals, words, strings,
finite lists, together with the two special data types !error and
!!eod&.
In this section we give a brief informal introduction
to pLucid expressions using these types
(for more formal details see Section 6).

.SH
3.1 Numeric Expressions
.PP
Numeric expressions in pLucid are basically the
same as in most other programming languages supporting
mixed integer and real arithmetic.
Numbers, i.e. reals and integers, are represented by numeric
constants.
As with APL, pLucid
distinguishes between the sign of a number, and the operator
applied on that number. The binary
subtraction operator on numeric expressions is represented
by the symbol @@-&; but
the negative sign of
a numeric constant is represented by the
symbol @@~&. The symbol @+ is the binary addition
operator on numerics, but the positive sign for numeric constants
should not be written, as it is assumed by default.
For example the following are numeric constants
.IB
   1   42   ~6  1.0    ~5.324     42.127
.IE
the following are not numeric constants
.IB
   +2   -4   -1.0   .12
.IE
At present there is no provision in pLucid
for the exponent representation of reals.
.PP
Apart from the additional operator @@isnumber&,
pLucid's numeric operators are essentially the same
as those in most other programming languages.
.PP
@isnumber is an operator which tests its argument
to see if it is numeric.
It returns the word @true if its argument
is either an integer or a real,
and returns the word @false otherwise.
For example, the following program checks whether
its input is a number. If it is, it outputs the square
of that number, then asks for the next input, and so on.
If the input is not a number the program outputs an error
message:
.PR
   if isnumber(x) then x*x else `The input was not numeric\\\\n' fi
.PE
To finish this section on numeric expressions
we will, by way of examples, describe the pLucid
division and modulus operators.
The remaining numeric operators need no explaination
beyond that given in Section 6.
.PP
pLucid has two division operators;
@div for integer division, and @/ for real division.
For any numbers !n and !m, !n @div !m is the largest
integer  such that  (!!n& @div !!m&)*!!m& is not greater than
!!n&.
The following are example pLucid expressions together with their
equivalent integer constants.
.DS
    12 @div 5        is equivalent to        2
    60 @div ~5       is equivalent to       ~12
.DE
The second division operator is for real division.
@/ accepts numeric operands and yields a numeric
result, e.g.
.DS
    123 @/ 5         is equivalent to       24.6
    0.123 @/ 0.123   is equivalent to        1
    ~1.0 @/ 3        is equivalent to       ~0.33333
.DE
Remember that the accuracy of @/ is dependent upon
the accuracy of the pLucid implementation.
.PP
The modulus operator @mod is the mathematical modulus function.
For any numbers !n and !!m&,

!n @mod !m equals !n @-!!m&@@*&(!!n& @div !!m&)

For example,
.DS
     9 @mod 5        is equivalent to        4
    ~9 @mod 5        is equivalent to        ~4
   4.5 @mod 1.2      is equivalent to        0.9
.DE
.SH
3.2 Non-Numeric Data Processing
.PP
Besides numeric computation, pLucid allows non-numeric
data processing. This is done using the types word,
string and finite list.
These types, which are similar to those in POP-2, are
discussed in this section.
.SH
3.2.1 Word Processing:
.PP
A word is determined by a sequence of characters of one of
the following forms,
.DS
      - an arbitrarily long sequence of alphanumerics
        starting with a letter
          (e.g.  this  t23r  Warwick )
      - an arbitrarily long sequence of signs, where
        a sign is one of
          + - * / $ & = < > : # ^
      - a bracket, where a bracket is one of,
           (    )    (%    %)    [%    %]
      - a separator, where a separator
        is either ; or ,
      - a period
      - a double quote
.DE
The following are examples of words
.IB
   yes    true    .    "    %]  ##$$&   , false
.IE
A word constant is simply
a word enclosed in double quotes.
For example, the pLucid word constants representing
the above words are
.IB
  "yes"  "true"  "."  """  "%]"   ","  "##$$&"
.IE
The distinction between words and word constants
is important and should must be clearly understood.
The following example is instructive:
.IB
  "fred"
.IE
is a word constant representing the word fred. On the other
hand
.IB
  fred
.IE
is an identifier (i.e. a variable).
.PP
Word constants, like other constants, are pLucid expressions,
and can be used to define values for variables in programs. For
example, the definition
.PH
     x  =  "dog";
.PE
defines the value of the variable @x to be the word @@dog&.
.PP
The boolean values in pLucid are the words @true and @false,
i.e they obey the rules for the type word
i.e. @@isword (true)=true& and @@isword(false)=true&.
Note the programmer may assume that the variables @true and @false have been
predefined to the words @true and @@false&. Thus it is as if every pLucid
program has the following definitions included automatically:
.PH
  true = "true";
  false = "false";
.PE
However, for the sake of clarity, we shall talk about
these two special words separately in section 3.2.2.
.PP
Beside those operators described in section 3.2.2, which
apply to the words @true and @@false&, there
are four other operators applicable to words in general.
These operators are @@isword&,
@mkword and the polymorphic operators @eq and @ne which are used
to compare words.
.PP
The operator @isword
recognizes words, it yields @"true" if its argument is a
word, and @"false" otherwise. For example :
.DS
   @@isword ("pLucid")&  is the word @@"true"&,
   @@isword (123)&       is the word @@"false"&, as the argument is a
                      number.
.DE
.PP
The operator
@@mkword&,
read !!make word&, takes a
string as an argument. If the characters in this string make up
a legal word it returns that word.
Otherwise, it returns the error object.
For the definition of what a string is see section 3.2.3.
The following are some examples of the use of the operator
@@mkword&:
.DS
  @@mkword (`this')&  has the same value as the word constant @@"this"&,
  @@mkword (`this ')& returns the error object,
  @@mkword (`123')&   returns the error object,
.DE
The binary operators
@@eq& and @ne
are for comparing two words. @eq returns the word @true if its
arguments are equal, and returns @false otherwise. @ne
returns @true if the arguments are different, and @false
otherwise. It is worth mentioning here that these operators
can be applied to any data objects in pLucid data types.
For example :
.PR
       (z eq "dog") or not (isword (z))
.PE
can be viewed as a filter with three inputs.
.IB
                                 |
                                 V z
            +--------------------+
            V                    V
      +----------+          +----+----+
      | eq "dog" |          | isword  |
      +----------+          +---------+
           |                     |
           |                  +--+---+
           +-------+          |  not |
                   |          +------+
                   V              V
                   |   +----------+
                   |   |
                 +-+---+-+
                 |  or   |
                 +-------+
                     |
                     V
.IE
The above machine continuously asks for inputs. If the input
is neither a word  nor the word @dog, then the output is the
word @@true&, otherwise the output is @@false&.
.SH
3.2.2  Boolean Expressions and Predefined Variables
.PP
It is assumed in each pLucid program that there are five
variables with predefined values.
These variables are @@true&, @@false&, @@nil&, @error and
@@eod&.
The variables @true and @false have as values the words
@true and @false respectively.
The value of the variable @nil is the empty list.
It is as if the pLucid programmer has the following definitions
included in his program.
.PH
      true   =  "true"
      false  =  "false"
      nil    =   []
.PE
The variable @error has as its value the error object
and the variable @eod has as its value the eod (end of data)
object. For
the definitions of these last two objects see section 3.3.
.PP
PLucid has the usual boolean operators, namely
@@or&, @@not& and @and. These operators
yield their usual boolean results when applied to
the words @@true& and @@false&. However when one of their
arguments is neither @true nor @false the operators may yield
the error object. For further details see
section 3.3.
.PP
Apart from many of the usual
logical operators found in other languages,
pLucid includes the operators @@isnumber&,
@@isword&, @@isstring&, @@isnull&, @@isatom&, @@iserror&,
and @@iseod&. These will
be defined throughout the definition of the different data
types of pLucid.
.SH
3.2.3 String Processing:
.PP
A string, in pLucid, is determined by a sequence of zero or more
ASCII characters.
Strings must not be confused with words, they
form a completely different data type. Any attempt to
apply a string operator on a word, or vice versa will
yield the error object.
.PP
Strings in pLucid are
string constants.
The string constant representing a
given string is obtained by first
replacing each character by its
pLucid representation, and secondly
enclosing the resulting sequence between @` and @@'&.
For example, the following are
string constants:
.PH
           `:: yes, ~ #'
           `this is a string ? '
           `pLucid is non procedural '
.PE
.PP
For string processing in pLucid, we have the following
operators:
@isstring takes  one  argument and returns the word @true
whenever this argument is of type string;
otherwise it returns
the word @@false&.
.SH
^ (string concatenation)
.PP
The operator @^ is the string  concatenation  operator.  It takes
two strings and forms a new  one  by  concatenating
the second to the end of the first. However, if one
of its arguments is not a string it yields the error
object.
.SH
substr
.PP
The operator @substr
read as !!substring&, takes three arguments. The
first of which is a string, and the other two are
integers. These two integers determine a range
for extracting a substring from the first argument
of @substr (the first argument must be a
string). Hence, the first integer should be less
than or equal to the second which should be less
than or equal to the length of the string.
Mathematically, the operator @substr can be defined as follows:
.DS
   If the value of the string S is the sequence of
   characters  $${ S sub 1 }~{S sub 2 }~...{ S sub n-1 }&
   where k and m are integers such that !k <= !m <= !n , then
        @@substr&(!!S&,!!k&,!!m&) = $${ S sub k }...{ S sub m }&
        Otherwise, the result will be the error object.
.DE
.SH
mkstring
.PP
The operator @mkstring, read as !!make string&, takes its argument,
which should be a word, and makes a string out of it.
If the argument is not a word, the result will be the
error object. Consider the following example pLucid
expressions and their equivalent string constants
.DS
     @@mkstring("hello")&  gives   @@`hello'&
     @@mkstring("t1239")&  gives   @@`t1239'&
     @mkstring(123)      yields the error object
.DE
.SH
length
.PP
The operator @length returns the lenght of a string, that is the
number of characters contained in the string. Note that internally
characters are stored unescaped. That is to say that \\b, backspace
is not stored as two characters backslash and b it is stored as the
character backspace.
.PP
NOTE @length is polymorphic with respect to types, that is given
an expression that evaluates to a type that is measurable in length
then @length will return its length otherwise it will return the
error object.
.SH
Examples
.PP
The following is an example program that takes one input @@x&, and checks
whether or not it is a string.
.PR
      if isstring x
         then   x ^ ` is a string well done'
         else   `Sorry its not a string,  try again'
      fi
.PE
If @x is a string the program outputs @x concatenated
to the string
.PR
   ` is a string well done'
.PE
Otherwise the program outputs
.PR
   `Sorry its not a string,  try again'
.PE
.SH
3.2.4 List Processing:
.PP
The third non-numeric type in pLucid are the finite lists. A
list
in pLucid is essentially a sequence of zero or more items,
each of which
is either a number, a word, a string, or a list. Syntactically,
there are two ways to represent a list in pLucid; as a
list constant, and as a list expression.
.SH
list constant
.PP
A list constant is a sequence of items enclosed in square brackets. Each
of these items is either a numeric constant, a word
constant less the quotes, a string constant, or a list constant.
Quotes, single and double, around string and word constants act
as delimiters. However it is obligatory in pLucid to write the
string quotes, it is necessary to drop
the quotes of the word constants when written as items
in a list constant. Whenever ambiguity occurs a
space can be used to seperate
two successive words. So the following
two list constants have the same value
.DS
     i.   @@[" dog "]
     ii.  @@["dog"]
.DE
but the following do not
.DS
     i.   @@["dog"]
     ii.  @@[dog]
.DE
Moreover, the following two list constants are different
.DS
     i.   @@[ this is pLucid ]
     ii.  @@[`this' is pLucid]
.DE
because the sequence of letters, this, occurs in the first
list as a word constant, while in the second it is a string
constant.
.PP
The second way for representing lists is by
list expressions.
A list expression is either an expression build up using the
@:: (cons) operator or a
sequence of zero or more
pLucid expressions enclosed
in %-square brackets (@@[%& and @@%]&) and separated by commas.
To make this clearer, consider the following examples:

1- The followings are expressions in pLucid, as they are all
   constants,
.PH
        "dog"   `c a t'   [this is pLucid]
.PE
The list expression, built from these expressions, can be
written as
.PH
        "dog" :: `c a t' :: ("this" :: "is" :: "pLucid" :: nil) :: nil
        [% "dog", `c a t' , [this is pLucid] %]
.PE
The value of either list expression is equal to the value of the list
constant
.PH
        [ dog `c a t' [this is pLucid] ]
.PE

2- The value of the list expression
.PH
        [% i+3 , tl (y) , `S T R' , [% "programming" %] %]
.PE
depends on the values of @@i&, and @@y&. Assuming that @i
equals 2, and
@y is the list @@[ hello world ]&,
then the list expression above will be equivalent to the constant
.PH
        [ 5 [world] `S T R' [programming] ]
.PE

3- The sequence of characters @@[% "dog" %]& in
the list constant
.PH
        [ the [% "dog" %] sat on the mat ]
.PE
is not a list expression, it is a sequence of five word
constants, each of which is considered as an item in the main list.
Note that every item in a list constant should be a constant.
.PP
pLucid facilitates list processing by means of the
following four operators.
.SH
hd
.PP
The prefix operator @@hd&,
read as !!head&, yields as its result
first element of the argument list. This may be a
word, a  number,  a string,  or  a  list. When the argument to @hd is
a list  of  one element the result will be
that element. If the argument to @hd is either the empty list
or a non-list then @hd
yields the error object.
.SH
tl
.PP
The prefix operator @@tl&,
read as !!tail&,  yields as its result
the argument list but with its head (i.e the first object in
the list) removed. Unlike
@@hd&, the tail operator yields the  empty list  when
its argument is a one object list. If the argument is either the empty
list or a non-list @tl yields the error object.
.SH
<> (append)
.PP
The infix operator @@<>&,
read as !!append&,
takes two lists and  yields  a new list constructed by
appending the second list to the end of the first. If  one
of the arguments is a non list it yields the error object.
.SH
:: (cons)
.PP
the infix operator  @@::&,
read !!construct&, yields (constructs) a new list from its two
arguments, the right argument must be a
list. The new list has as its head the left hand argument and
as its tail the right hand argument. If the  right argument is not a
list the result will be the error object.

@eq and @ne
.PP
The operators @eq and @ne
are for comparing data items in pLucid, and have
been explained before.
.SH
isnull
.PP
The operator @isnull
which takes a list and returns the word @true if the
list is the empty list, and @false if it is not. It
returns the error object if the argument is not a list.
.SH
isatom
.PP
The operator @isatom takes an expression as its argument, if
the expression evaluates to an object of type number, string or
word the the value returned by @isatom is @true otherwise it is
the value @false.
.SH
islist
.PP
The operator @islist takes an expression as its argument, if the
expression evaluates to an object of type list then the value returned
by @islist is @true otherwise it is the value @false. 
.SH
Examples
.PP
The following examples illustrate the use of @hd
.DS
@@hd([hello world])&   has the same value as the word constant @@"hello"&

@@hd([% [this 'is`] pLucid %])&
                    has the same value as the list constant @@[this `is']&

@@hd([ [[a b] c] d [e] ])&
                    has the same value as the list constant @@[[a b] c]&
.DE
The following expressions return the error object
.DS
        @hd([])         @hd is applied to the empty list,
        @hd("this")     the argument is not a list,
        @hd(nil)        the argument is the empty list.
.DE
The following examples illustrate the use of @tl
.DS
@@tl([hello world])&   has the same value as the list constant @[world]

@@tl([hello [world] ])&   has the same value as the constant @@[[world]]&

@@tl([ programming ])&    returns the empty list nil

@@tl( [% 1+3, `S T R', [% "programming" %]&
                       has the same value as the list constant
                       @@[`S T R' [programming]]&
.DE
The following expressions return the error object
.DS
        @tl("what")     the argument is not a list
        @tl(nil)        the argument is the empty list
.DE
The following examples illustrate the use of the operator @<>
.DS
@@[[pLucid] is a] <> [ [non] procedural language]&
                        has the same value as the list constant
                @@[[pLucid] is a [non] procedural language]&

@@[Lucid ] <> `with POP2'&  returns the error object as one of the
                         arguments is not a list
.DE
The following examples illustrate the use of the construction
operator @::
.DS
@@[programming] :: [languages]&
                has the same value as
                                @@[ [programming] languages ]&

@@`pLucid is'::[ Lucid]&  has the same value as  @@[`pLucid is' Lucid]&

@@[the language] :: [% "Iswim" ,700 , `iteration' %]&
              has the same value as
                        @@[[the language] Iswim 700 `iteration']&

@@`##87' :: nil&                returns the list constant @[`##87']

@@[pascal] :: `triangle'&       returns the error object, because
                             the second argument is not a list.
.DE
.SH
Example
.PP
Any expression above whether it is a constant or
an operator with some operands can be written as a program by
itself, for example the expression
.PR
        hd(x)
.PE
is a program which repeatedly asks for values for the variable
@@x&,
expecting each to be a list, and producing as output the head
of each. It yields
the object error if the input is not a list.
The program could be thought of as a filter for which a typical
computation sequence might be as follows:
.IB
       |            |           |           |
[[] 3] |         [] |   [`hi' 3]|           |
       |            |           |           |
       v            v           v           v
    +--+--+      +--+--+     +--+--+     +--+--+
    |  hd |  ==> |  hd | ==> |  hd | ==> |  hd |   ==> AD INFINITUM
    +--+--+      +--+--+     +--+--+     +--+--+
       |            |           |           | `hi'
       |            | []        |  error    | error
       |            |           |  []       | []
       v            v           v           v
.IE
.SH
Example
.PP
When the program
.PR
rotate  where

        t = x fby tl t <> z ;
        z = hd(t) :: nil ;
        rotate =if t eq first x and counter ne 0 then eod
                                                 else t  fi;
        counter = 0 fby counter + 1 ;
   end
.PE
is run on the machine,
it will ask for a value for the variable @@x&, expecting it to
be a list. Then it produces all the possible rotations for
this list. Instead of asking for the next value of @@x&, it
gives the object eod, which terminates the whole program.
So supplying the list constant @@[a b c d]& as an input,
the program will give the result
.PH
        [a b c d]
        [b c d a]
        [c d a b]
        [d a b c]
.PE
and then terminates.
.SH
3.3  The objects eod and error :
.PP
Among the data types of pLucid there are two special
types which are completely distinct.
These are the types error and eod.
.SH
The object error
.PP
We have mentioned, while defining the data types of pLucid,
the cases where
an operator
yields the object error. Informally speaking, the error
object results from applying an operator of certain type
to operands which are of types different than the ones
which such an operator expects. For example, trying to
add two words,
find the head of a string, or divide a number by a list.
.PP
In a program we represent the object error by
the predefined identifier @@error&. Hence, the value of
the expression
.PR
        error
.PE
is the stream of error objects.
It is worth noting here that, the above expression differs
from the expression
.PR
        "error"
.PE
The first represents the error object, while the second
is the word constant representing the word error.
.PP
While the object error is represented, in a program, by
the predefined identifier @@error&, its representation as an
input, or as an output, is dependent on the implementation
of the language. In the Warwick implementation it is
represented by the symbol @@?&.
(for further details see section 5).
.SH
the object eod
.PP
In a program, this object will be represented by the predefined
identifier @@eod&.
Unlike the object error, trying to output the object
eod will cause the machine to terminate the execution of
the program. For example, the program
.PR
        1 fby 2 fby eod
.PE
will output the numbers 1, 2 and then terminates.
The representation of the eod object as an input is again dependent
on the inplementation. In the Warwick implementation it is
represented by the character control-D or the !at character
(See section 5 for more details).
Again, the distinction between the eod object represented by
the reserved identifier @@eod&, and the word eod represented
by the word constant @@"eod"&, should be clear.
.PP
Furthermore, the special objects
error
and
eod
are considered as expressions in pLucid. They are represented
by predefined identifiers, but they do not have constant
representations in the language. Hence, they cannot
be written as items in list constants, as such items
must be constants. So the items in the list constants
.PH
        [ error ]   [ eod ]
.PE
are, respectively, the words error and eod. On the other
hand being expressions, they can be written as items in
list expressions.
.SH
Examples
.PP
The value of the list expression
.PR
        [% error %]
.PE
is the object error. This is because the above list expression
is really an abbreviation of the expression
.PR
        error :: nil
.PE
which evaluates to the object error. However, the value of
the list expression
.PR
        [% "error" %]
.PE
is a list of one item. This item is the word error.
.PP
Similarily, the value of the list expression
.PR
        [% eod %]
.PE
is the object eod.
.PR
        [% "eod" %]
.PE
is a list of one item. This item is the word @@"eod"&.
.PP
If the argument to an operator is the object eod (or the object
error) then, except for certain operators, the object yielded by
that operator will be the object eod (or the object error).
The following are the only prefix operators that yield
values other than @eod or @error when one of their arguments is
@eod or @error are
@@iserror& and  @@iseod&.
These operators are for recognizing the objects error
and eod. When the argument of the operator
@iserror is the object error, it yields the word @@true&.
If the argument is the object eod it yields the object
eod. Otherwise it yields
the word @@false&. On the other hand, when
the operator @iseod is applied to the object eod
it yields the word @@true&. Otherwise it yields the word
@@false&.
.PP
The boolean operators
@and  and  @@or& can also return values other than @eod and
@error when given these two objects as arguments. The
result of applying these operators to the objects
error and eod are given in the following table. It is worth
mentioning here that these operators are commutative, i.e.
the following two rules hold for any P and Q
.IB
             P or Q  = Q or P
             P and Q = Q and P


    P        Q       P and Q         P or Q
 ----------------------------------------------

  true     error      error           true

  false    error      false           error

  true     eod        eod             true

  false    eod        false           eod

  error    eod        eod             eod

  error    error      error           error

  eod      eod        eod             eod

.IE
.SH
The operator if then else fi
.PP
The operator @if @then @else @fi is the usual.
For any expressions !!X&, !!Y&, and !!Z&, the value of the expression
.DS
        @if !X @then !Y @else !Z @fi
.DE
is defined as follows. If the value of !X is the word @@true&
then the value of the expression is equivalent to the value
of !!Y&, no matter what the value of !Z is. If the value of !X
is @@false&, the value of the expression is the value of !!Z&, no
matter what the value of !Y is. However, if
the value of !X is the object
error then the value of the expression is the object
error. Similarly, if the value of !X is the object eod, then
the value of the expression is the object eod.
For example when the program
.PR
   if iseod x then `I will terminate \\\\n' fby eod
             elseif isnumber x
              then `it is a number \\\\n'
               elseif islist x
                then `it is a list \\\\n'
                 else "itisaword"        fi
.PE
is run on a machine then
the program will
ask for values for the identifier @@x&. If the input is the
input representation of the object eod in that implementation,
the execution of the program will terminate after
outputting the string @@`I will terminate'& followed by a new
line. If @x is a number, it will output the string
@@`it is a number'& as  a message, then asks for the
next input value of @@x&. If it is a list, or a word,
it will notify that by similar messages as the ones before,
then asks for the next value of @@x&, and so on.
.SH
3.4 pLucid Conditional Expressions :
.PP
The simplest conditional expression in pLucid is
the expression 
@if @then @@else&. 
The expression

.IB
        if !!<expr1>& then !!<expr2>& else !!<expr3>& fi
.IE
expects the values of !!<expr1>& to be of type boolean, while
the values of !!<expr2>& and !!<expr3>& could be of any type.
The pLucid conditional expression works as the
logical function @if @then @@else&. It evaluates
the expression !!<expr1>&, if it is the word @true the result
will be the value of the expression !!<expr2>&, and if it is @false
the result is the value of !<expr3>&. If the expression
!!<expr1>
is not a boolean, i.e its value is
not a truth value, the @if @then @else expression will output
the error object. The word @fi appearing at the end in
the above expression is a terminator for the @if @then @else expression.
.SH
Example
.PP
The pLucid program
.PR
        if X <= Y then Y else X fi
.PE
asks for the values of  @X and @Y as inputs. It evaluates
the  expression  @@X <= Y&, if it is @true it returns the
value of @@Y&, if @false it returns the value of @@X&.
If one of the inputs is not numeric it outputs the error
object.
Then it asks for new values for @X and @@Y&, and so on. This
expression could be used to define a function which takes
two arguments and yields their maximum by writing
.PH
        max (X,Y) = if X <= Y then Y else X fi ;
.PE
.PP
pLucid conditional expressions could be nested to many levels
by using the reserved word @elseif instead of @@else&. The
word @elseif acts as @else for the outer @if expression and
as @if for the new one. i.e there is no need to write the word
@if after @elseif. Hence the expression
.PR
        if x then y elseif
           z then m elseif
           n then q else  r fi
.PE
is equivalent to the expression
.PR
        if x then y
             else if z then m
                       else if n then q
                                 else r
                            fi
                  fi
        fi
.PE
.SH
Example
.PP
The following is a legitimate expression in pLucid, it
expresses a simple salaries system,
.PR
     if ( status  eq "single" ) then  basicsalary
  elseif ( status  eq "married") then  basicsalary * 1.30 +
                                      children * 40
         else specialsalary  fi
.PE
When this program is run the evaluator asks for a
value for the variable @@status&. If the value of @status is
the word @@single&, it asks for the value of @basicsalary
and outputs it. If the value of @status is the word @@married&,
it asks for the values of @basicsalary and @@children&, then
outputs the value of the expression
@@basicsalary * 1.30 + children * 40&.
Otherwise, it asks for the value @specialsalary
and outputs it. Then it asks for the next value of @@status&,
and so on.
.SH
pLucid case Expression :
.PP
Another way of writing conditional expressions in pLucid is by
using the pLucid @case expression. The @case expression provides a
clearer way for writing long nested @if expressions.
The example above could be written using the
@case expression as follows:
.PR
     case status of
        "single"  :  basicsalary ;
        "married" :  basicsalary * 1.30 + children * 40 ;
        default   :  specialsalary ;
          end
.PE
The @case expression consists of two parts, the
!selector !part and the @case body. The selector part is the expression
which comes between the words @case and @@of&,
see the above example. The @case body is a list of
switches, terminated by the word end. Each switch is of the form
.IB
        expression  :  expression
.IE
and must be terminated by a semicolon. One of these switches
should be the default switch, which is a switch whose left
hand side is the word @default. Evaluation of the @case expression
proceeds as follows: the selector part, which is an expression, is
evaluated, then its value is compared with the value of the
left hand side expression of each switch in the @case body. If there
is an expression which is equal to the selector, then the
value of the right hand side expression of that switch
is the value of the @case expression. Otherwise, the value of
the @case expression is the value of the expression to
the right of the word default.
In the event of two expressions in the @case body
with the same selector value, the first one to
appear in the list is chosen.
.PP
The case expression, like the if expression, could be nested
to many levels. This is done by writing another case expression
as the right hand side of a switch in the outer case expression.
For example, suppose we want to split the case of being
married in the above example, so we write
.PR
     case status of
        "single"  :  basicsalary ;
        "married" :  case  arechildren of
                             "none"  :  basicsalary * 1.20 ;
                             "many"  :  basicsalary * 1.30 +
                                        numchildren * 40 ;
                             default :  `there is an error in your data\\\\n';
                     end;
        default   :  specialsalary ;
      end
.PE
.SH
pLucid cond Expression :
.PP
Another way of writing conditional expressions in pLucid is by
using the pLucid @cond expression. The @cond expression provides a
clean mechanism for the writing of long nested if-expressions.
The example above could be written in terms of the
@cond expression:
.PR
     cond
        status eq "single"  :  basicsalary ;
        status eq "married" :  basicsalary * 1.30 + children * 40 ;
        default             :  specialsalary ;
     end
.PE
The @cond expression consists of a body which is a list of
switches, terminated by the word end. Each switch is of the form
.IB
        expression  :  expression
.IE
and must be terminated by a semicolon. One of these switches
should be the default switch, which is a switch whose left
hand side is the word @default. Evaluation of the @cond expression
proceeds as follows: the expressions to the left of the @: are
evaluated from top to bottom and if one is found to be true then
the value of the @cond is the expression on the right hand side
of the @: and if no expression to the left of a @: is true then
the value of the @cond is the default value.
In the case of there being two expressions in the @cond body
that are true then the first encountered is chosen.
.PP
The @cond expression, like the @if and @case expressions, can be nested
to many levels.
