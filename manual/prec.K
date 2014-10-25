.SP 43 
.SH
6 Tables and Rules
.SH
6.1 Tables of Operators
.SH
Numeric operators :
.IB
operation           syntax     type of operands  type of result

addition              +            numeric          numeric

subtraction           -            numeric          numeric

multiplication        *            numeric          numeric

integer division      div          integer          integer

real division          /           numeric          numeric

exponentiation        **           numeric          numeric

modulus               mod          integer          integer

numeric              isnumber      anything         boolean

less than              <           numeric          boolean

greater than           >           numeric          boolean

equal                 eq           numeric          boolean

less than
or equal              <=           numeric          boolean

greater than
or equal              >=           numeric          boolean

not equal              ne          numeric          boolean

sin                   sin          numeric          numeric

cos                   cos          numeric          numeric

log                   log          numeric          numeric

tan                   tan          numeric          numeric

sqrt                  sqrt         numeric          numeric

abs                   abs          numeric          numeric

log10                 log10        numeric          numeric
.IE
.SH
Word Operators:
.IB
   operation          syntax      operand type   type of result

make a word out
of a string           mkword      string          word

recognize a word      isword      anything        boolean
.IE
.SH
Boolean operators :
.IB
operation           syntax      type of operands

conjunction          and          boolean

disjunction          or           boolean

negation             not          boolean
.IE

.SH
String Operators:
.IB
operation       syntax     type of operands    type of result

make a string
out of a word   mkstring      word              string

string
recognition     isstring      anything          boolean

string
concatination      ^          string            string

form a                      string, integer,
substring     substr   (string,integer        string
.IE
.SH
List operators :
.IB
operation      syntax   operand1      operand2    result

the head of
a list           hd       list           --       anything

the tail of
a list           tl       list           --       list

appending two
lists            <>       list          list      list

construction
operator         ::       anything      list      list

is the list
empty ?         isnull    list           --       boolean

is it an atom
(not a list)    isatom    anything       --       boolean
.IE
.SH
Conditional expressions :
.SH
if expression :
.IB
  if
boolean expression
then
expression
else
expression
fi
.IE
.SH
nested if expression:
.IB
if
boolean expression
then
expression
          elseif
boolean expression
then
expression
else ... fi
.IE
.SH
case expression :
.IB
case
expression
of
        case 1  : expression 1 ;
        case 2  : expression 2 ;
        ....
        case n  : expression n ;
        default : expression  ;
        end
.IE
.SH
Lucid operators :
.IB
operator         syntax    operand 1   operand 2     result

first            first     anything      ---         anything

next             next      anything      ---         anything

followed by       fby         "        anything         "

whenever         whenever     "        boolean          "

as soon as        asa         "        boolean          "

upon              upon        "        boolean          "

.IE
.SH
current declaration :
.IB
        identifier is current expression
.IE

6.2
.ul
Associativity and Precedence Rules

.ul
Associativity of Operators:
.PP
An infix operator is said to be 'right associative',
e.g fby, if for any expressions E1, E2, and E3, the
expression

                X fby Y fby Z

is always interpreted as

                X fby ( Y fby Z )  .

Similarly, an infix operator is said to be 'left associative',
e.g asa, if for the expressions E1, E2, and E3, the
expression

                E1 asa E2 asa E3

is always interpreted as

                ( E1 asa E2 ) asa E3

The following table gives the associativity
of infix operators in pLucid:

.ul
Associativity         Operators

left                  + , - , * , / , div , mod , or , and ,
                      asa , whenever , wvr, upon , if then else,
                      case

right                 :: , <> , fby , ^


.ul
Precedence Rules :
.PP
These are rules to avoid clogging up programs with
unnecessary brackets.

e.g:
  If we say that '* has higher precedence than +'
then an expression like '2 + 4 * 5'
is always interpreted as '2 + ( 4 * 5 )'.

We list here the hierarchy of precedences amongst
pLucid operators. Operators with lowest precedences
are at the top of the list, and ones with highest
precedences are at the bottom.

        fby
        asa , upon , whenever, wvr
        if then else fi, case
        or
        and
        not
        eq, ne, lt, le, gt, ge, < , <= , > , >=
        + , -
        * , div , / , mod
        :: , <>
        first, next, sin, cos, log, hd, tl, isnull, isnumber,
               isatom, isword, isstring, mkword, mkstring,
               substr, iserror,iseod,
        ^


The
.ul
where-clause
has the lowest precedence amongst other constructs in pLucid,
so if E1, E2, and E3 are expressions, then for any operators
in pLucid, say fby and next, the expression

        E1 fby E2 fby next E3 where
                                ...
                                ...
                                ...
                              end

is always interpreted as

        (E1 fby E2 fby next E3) where
                                  ...
                                  ...
                                  ...
                                end

6.3
.ul
Reserved Words:
.PP
These identifiers are reserved as keywords

.DS 0
if                      hd                      true
then                    tl                      false
else                    isatom                  sin
elseif                  isnumber                cos
fi                      isnull                  log
case                    nil                     is
of                      div                     current
default                 mod                     eod
where                   and                     error
end                     not                     isword
first                   or                      isstring
next                    eq                      iserror
fby                     ne                      iseod
asa                     lt                      substr
whenever                le                      mkstring
wvr                     gt                      mkword
upon                    ge                      arg
attime                  log10                   tan
length                  abs                     sqrt

.DE -0
