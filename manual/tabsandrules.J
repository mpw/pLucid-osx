.SP 39
.SH
6 Tables and Rules
.SH
6.1 Tables of Operators
.SH
Numeric operators :
.IB
!operation           !syntax     !type !of !operands  !type !of !result

!addition              +            !numeric          !numeric

!subtraction           -            !numeric          !numeric

!multiplication        *            !numeric          !numeric

!integer !division      div          !integer          !integer

!real !division          /           !numeric          !numeric

!exponentiation        **           !numeric          !numeric

!modulus               mod          !integer          !integer

!numeric              isnumber      !anything         !boolean

!less !than              <           !numeric          !boolean

!greater !than           >           !numeric          !boolean

!equal                 eq           !numeric          !boolean

!less !than
!or !equal              <=           !numeric          !boolean

!greater !than
!or !equal              >=           !numeric          !boolean

!not !equal              ne          !numeric          !boolean

!sine                  sin          !numeric          !numeric

!cosine                cos          !numeric          !numeric

!logarithm             log          !numeric          !numeric

!tangent               tan          !numeric          !numeric

!square !root           sqrt         !numeric          !numeric

!absolute !value        abs          !numeric          !numeric

!log10                 log10        !numeric          !numeric
.IE
.SH
Word Operators:
.IB
   !operation          !syntax      !operand !type   !type !of !result

!make !a !word !out
!of !a !string           mkword      !string          !word

!recognize !a !word      isword      !anything        !boolean
.IE
.SH
Boolean operators :
.IB
!operation           !syntax      !type !of !operands

!conjunction          and          !boolean

!disjunction          or           !boolean

!negation             not          !boolean
.IE

.SH
String Operators:
.IB
!operation       !syntax     !type !of !operands    !type !of !result

!make !a !string
!out !of !a !word   mkstring      !word              !string

!string
!recognition     isstring      !anything          !boolean

!string
!concatenation      ^          !string            !string

!form !a !substring substr (!string,integer,integer) !string

!length           length        !anything          !number 
.IE
.SH
.IB

@List @operators :

!operation      !syntax   !operand1      !operand2    !result

!the !head !of
!a !list           hd       !list           --       !anything

!the !tail !of
!a !list           tl       !list           --       !list

!appending !two
!lists            <>       !list          !list      !list

!construction
!operator         ::       !anything      !list      !list

!is !the !list
!empty?         isnull    !list           --       !boolean

!is !it !an !atom
!(not !a !list)    isatom    !anything       --       !boolean

!is !it !a !list   islist    !anything       --       !boolean

.IE
.SH
Conditional expressions :
.SH
if expression :
.IB
  if
!boolean !expression
then
!expression
else
!expression
fi
.IE
.SH
nested if expression:
.IB
if
!boolean !expression
then
!expression
          elseif
!boolean !expression
then
!expression
else ... fi
.IE
.IB

@case @expression :

case
!expression
of
        !expression !  : !expression  ;
        !expression !  : !expression  ;
        !....
        !expression !  : !expression  ;
        default : !expression  ;
        end
.IE
.SH
cond expression :
.IB
cond
        !boolean !expression : !expression ;
        !boolean !expression : !expression ;
        !....
        !boolean !expression : !expression ;
        default : !expression  ;
        end
.IE
.SH
Lucid operators :
.IB
!operator         !syntax    !operand 1   !operand 2     !result

!first            first     !anything      ---         !anything

!next             next      !anything      ---         !anything

!followed !by       fby         "        !anything         "

!whenever         whenever     "        !boolean          "

!whenever         wvr          "        !boolean          "

!attime           attime       "        !integer          "

!as !soon !as        asa         "        !boolean          "

!upon              upon        "        !boolean          "

.IE
.SH
current declaration :
.IB
        !identifier is current !expression
.IE


.SH
6.2 Associativity and Precedence Rules
.SH
Associativity of Operators:
.PP
An infix operator is said to be 'right associative',
e.g @@fby&, if for any expressions E1, E2, and E3, the
expression
.PR
                X fby Y fby Z
.PE
is always interpreted as
.PR
                X fby ( Y fby Z )  .
.PE
Similarly, an infix operator is said to be 'left associative',
e.g @@asa&, if for the expressions E1, E2, and E3, the
expression
.IB
                !E1 asa !E2 asa !E3
.IE
is always interpreted as
.IB
                ( !E1 asa !E2 ) asa !E3
.IE
The following table gives the associativity
of infix operators in pLucid:
.IB
!Associativity         !Operators

!left                  + , - , * , / , div , mod , or , and , ** ,
                      asa , attime, whenever , wvr, upon , if then else,
                      case

!right                 :: , <> , fby , ^

.IE
.SH
Precedence Rules :
.PP
These are rules to avoid clogging up programs with
unnecessary brackets. For example if
we say that '* has higher precedence than +'
then an expression like @@2 + 4 * 5&
is always interpreted as @@2 + ( 4 * 5 )&.
.PP
We list here the hierarchy of precedences amongst
pLucid operators. Operators with lowest precedences
are at the top of the list, and ones with highest
precedences are at the bottom.
.IB
    1    asa , upon , whenever, wvr , attime
    2    fby
    3    if then else fi, case, cond
    4     :: , <>
    5    or
    6    and
    7    not
    8    eq, ne, < , <= , > , >=
    9    + , -
   10    * , div , / , mod
   11     **
   12     ^
   13     first, next, sin, cos, tan, log, log10, hd, tl, isnull, isnumber,
          isatom, isword, isstring, mkword, mkstring,
          iserror,iseod, sqrt, abs, arg , islist
.IE
.SH
The where-clause
.PP
The where-clause
has the lowest precedence amongst other constructs in pLucid,
so if E1, E2, and E3 are expressions, then for any operators
in pLucid, say fby and next, the expression
.IB
        !E1 fby !E2 fby next !E3 where
                                ...
                                ...
                                ...
                              end
.IE
is always interpreted as
.IB
        (!E1 fby !E2 fby next !E3) where
                                  ...
                                  ...
                                  ...
                                end
.IE
.SH
6.3 Reserved Words:
.PP
These identifiers are reserved as keywords
.IB
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
asa                     log10                   substr
whenever                tan                     mkstring
wvr                     abs                     mkword
upon                    sqrt                    length
cond                    filter                  attime
arg                     include                 islist
.IE
