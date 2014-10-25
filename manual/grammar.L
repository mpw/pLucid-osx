.SP 45
.SH
7 Miscellaneous
.SH
7.1 Plucid grammar
.PP
We define here the pLucid syntax using the BNF formalism,
where

      ::=  is read as <meta variable> is defined as <meta variable>,

       |   is read as <meta variable> or <meta variable> ,

     { }   denotes possible repetition zero or more times
           of the enclosed construct .


<program> ::= <expression>

<expression> ::=  <constant>
                | <identifier>
                | error
                | eod
                | <prefix operator> <expression>
                | <expression> <infix operator> <expression>
                | filter ( <expression> , <expression> , < expression> )
                | substr ( <expression> , <expression> , < expression> )
                | length <expression>
                | arg <expression>
                | <list expression>
                | <if expression>
                | <case expression>
                | <cond expression>
                | <function call>
                | <where clause>

<constant> ::=    <numeric constant>
                | <word constant>
                | <string constant>
                | <list constant>

<numeric constant> ::=   <integer constant>
                       | <real constant>

<integer constant> ::=    <digit> { <digit> }
                        | <n-sign> <integer constant>

<real constant>    ::=  <integer constant> . { <digit> }

<n-sign> ::=  ~

<word constant>::= <quote> <word constant less the quotes> <quote>

<word constant less the quotes> ::=   <letter> { <alphanumeric> }
                                   | <sign> {<sign>}
                                   | <bracket>
                                   | <period>
                                   | <separator>
                                   | <quote>


<sign> ::= + | - | * | $ | & | = | < | > | : | # | ^
<quote>     ::=  "
<bracket>  ::=  ( | ) | [% | %] | (% | %)
<period>   ::=  .
<separator> ::=  , | ;

<string constant> ::=  `{<character>}'

<character> ::= <Any ASCII character except the closing single quote ' >

<list constant>  ::=   nil | []
                     | [ {<list constant element>} ]

<list constant element>  ::=   <numeric constant>
                             | <word constant less the quotes>
                             | <string constant>
                             | <list constant>

<alphanumeric>  ::=  <digit> | <letter>

<digit> ::=  0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

<letter> ::=  A | B | C | D | E | F | G | H | I | J | K | L | M
            | N | O | P | Q | R | S | T | U | V | W | X | Y | Z
            | a | b | c | d | e | f | g | h | i | j | k | l | m
            | n | o | p | q | r | s | t | u | v | w | x | y | z

<identifier>::= <letter> { <alpahnumeric> }

<prefix operator> ::=   <p-numeric operator>
                      | <p-word operator>
                      | <p-string operator>
                      | <p-list operator>
                      | <p-lucid operator>
                      | <p-special operator>

<p-numeric operator> ::= sin | cos | tan | sqrt | abs | log10| log | isnumber

<p-word operator>  ::= isword | not | mkstring

<p-string operator> ::= isstring | mkword

<p-list operator> ::=  hd | tl | isatom | isnull | islist

<p-lucid operator> ::=  first | next

<p-special operator> ::= iseod | iserror

<infix operator> ::=     <i-numeric operator>
                       | <i-word operator>
                       | <i-string operator>
                       | <i-list operator>
                       | <i-lucid operator>

<i-numeric operator> ::= + | - | ** | * | div | mod | /
                       | eq | ne | <= | < | > | >=

<i-word operator>   ::=  and | or | eq | ne

<i-string operator>  ::=  ^  | eq | ne

<i-list operator> ::=    <> | ::  | eq | ne

<i-lucid operator> ::=  fby | whenever | wvr | upon | asa | attime

<list expression> ::=   [%%]
                      | [% {<expressions list>} %]

<expressions list> ::=   <expression item>
                       | <expression item> , {<expressions list>}

<expression item> ::=    <expression>
                       | <list expression>

<if expression> ::=  if <expression> then <expression> <endif>

<endif> ::=   else <expression> fi
              | elseif <expression> then <expression> <endif>

<case expression> ::= case <expression> of <cbody> end

<cond expression> ::= cond <cbody> end

<cbody> ::= {<expression> : <expression> ;} <defaultcase>

<defaultcase> ::=  default : <expression>;

<function call> ::= <identifier> ( <actuals list> )

<actuals list> ::=     <expression>
                     | <expression> , <actuals list>

<where clause> ::= <expression> where <body> end

<body> ::= <declarations list> <definitions list>

<declarations list> ::= { <current declaration> ; }

<current declaration> ::= <identifier> is current <expression>

<definitions list> ::=  { <definition> ; }

<definition> ::=   <simple definition>
                 | <function definition>

<simple definition> ::=  <identifier>  =  <expression>

<function definition>::=
        <identifier> ( <formals list> ) = <expression>

<formals list> ::=    <identifier>
                    | <identifier> , <formals list>
.SH
7.2 Syntax diagrams
.PP
In this  section we  introduce the  syntax diagrams for pLucid.
The reserved !words of the language appear underlined. However signs,
numbers, and brackets are not, as this might lead to ambiguity
.IB
!program :

---->--- !expression ---->--
.IE
.IB
!expression :

-->--+-->---   !constant   -------------------->------------------+-->--
     |                                                           |
     +-->---   !identifier     ---------------->------------------+
     |                                                           |
     +-->---    error    --------------------->------------------+
     |                                                           |
     +-->---     eod     --------------------->------------------+
     |                                                           |
     +-->--  !prefix !operator  --->---  !expression  ------>-------+
     |                                                           |
     +-->-- !expression -->-- !infix !operator -->-- !expression -->-+
     |                                                           |
     +-->-- !list !expression -------------------->----------------+
     |                                                           |
     +-->---  !if !expression   ------------------>----------------+
     |                                                           |
     +-->---  !case !expression ------------------>----------------+
     |                                                           |
     +-->---  !cond !expression ------------------>----------------+
     |                                                           |
     +-->---   !function !call   --------------->------------------+
     |                                                           |
     +-->---  !where !clause  -------------------->----------------+
     |                                                           |
     +-->---  substr -->-- ( -->-- !3expression  -->-- ) ------->-+
     |                                                           |
     +-->---  filter -->-- ( -->-- !3expression  -->-- ) ------->-+
     |                                                           |
     +-->-- length  ------->---- !expression --------->-----------+
     |                                                           |
     +-->-- arg ----------->---- !expression --------->-----------+
.IE
.IB
!constant :

--->--+-->-- !numeric !constant --->---+--->---
      |                              |
      +--->-- !word !constant --->-----+
      |                              |
      +-->-- !string !constant --->----+
      |                              |
      +--->-- !list !constant --->-----+
.IE
.IB
!numeric !constant :

--->--+-->-- !integer !constant -->--+-->--
      |                            |
      +--->-- !real !constant --->---+
.IE
.IB
!integer !constant :

--->--+-->------------+-->---+--->-- !digit ----+--->--
      |               |      |                 |
      +-->--  ~  -->--+      +----<------------+
.IE
.IB
!real !constant :

--->-- !integer !constant -->--  .  -->--+-->-- !digit ----+--->--
                                       |                |
                                       +----<-----------+
.IE
.IB
!word !constant :

--->-- " --+-->-- !letter -->--+----------->---------+----+-->-- " -->---
           |                  |                  |    |
           |                  +-- !alphanumeric --<--+    |
           |                                             |
           +-->--+-->-- !sign --->--+------------->-------+
           |     |                 |                     |
           |     +-----------<-----+                     |
           |                                             |
           +-->--+-----+-----+-----+-----+-----+         |
           |     |     |     |     |     |     |         |
           |     (     )    (%     %)    [%    %]        |
           |     |     |     |     |     |     |         |
           |     +-----+-----+-----+-----+-----+--->-----+
           |                                             |
           +---->----+-----+-----+-----+                 |
                     |     |     |     |                 |
                     .     ,     ;     "                 |
                     |     |     |     |                 |
                     +-----+-----+-----+----------->-----+
.IE
.IB
!sign:

-->--+----+----+----+----+----+----+----+----+----+----+----+
     |    |    |    |    |    |    |    |    |    |    |    |
     +    -    *    /         &    =    <    >    :    #    ^
     |    |    |    |    |    |    |    |    |    |    |    |
     +----+----+----+----+----+----+----+----+----+----+----+--->--
.IE
.IB
!string !constant :

--->-- ` -->--+------------->-------------------+--- ' --->--
              |                                 |
              +--- !ascii !character !except  --<--+
                 !the !closing !single !quote
.IE
.IB
!list !constant :

--->--+--->---  nil  ------------------>----------------+-->--
      |                                                 |
      |                                                 |
      +-->--- [ ---+--------->--------------+-->-- ] ---+
                   |                        |
                   +--<-- !list !element --<--+
.IE
.IB
!list !element :

--->--+--->-- !numeric !constant -->---+-->--
      |                              |
      +--->-- !word !constant   --->---+
      |       !less !the !quotes        |
      |                              |
      +--->-- !string !constant --->---+
      |                              |
      +--->-- !list !constant ----->---+
.IE
.IB
!alphanumeric :                         !letter:

-->--+--->-- !digit --->--+--->--       -->---+--+--+--+--+--+
     |                   |                   |  |  |  |  |  |
     +--->-- !letter -->--+                   ANY UPPER CASE OR
                                             LOWER CASE LETTER
                                             |  |  |  |  |  |
                                             +--+--+--+--+--+--->--
.IE
.IB
!digit:

-->---+---+---+---+---+---+---+---+---+---+
      |   |   |   |   |   |   |   |   |   |
      0   1   2   3   4   5   6   7   8   9
      |   |   |   |   |   |   |   |   |   |
      +---+---+---+---+---+---+---+---+---+--->--
.IE
.IB
!identifier:

--->-- !letter -->--+---------->-----------+--->--
                   |                      |
                   |                      |
                   +--- !alphanumeric --<--+
.IE
.IB
!prefix !operator :

--->--+-->-- !p-numeric !operator --->--+--->--
      |                               |
      +-->-- !p-word !operator ---->----+
      |                               |
      +-->-- !p-string !operator --->---+
      |                               |
      +-->-- !p-list !operator ----->---+
      |                               |
      +-->-- !p-lucid !operator ---->---+
      |                               |
      +-->-- !p-special !operator --->--+
.IE
.IB
!p-numeric !operator :

-->--+--->-- sin ----->---+-->--
     |                    |
     +--->-- cos ----->---+
     |                    |
     +--->-- tan ----->---+
     |                    |
     +--->-- sqrt ---->---+
     |                    |
     +--->-- abs ----->---+
     |                    |
     +--->-- log ----->---+
     |                    |
     +--->-- log10 --->---+
     |                    |
     +-->-- isnumber -->--+
.IE
.IB
!p-word !operator:

 --->--+-->--  not  ---->-----+-->--
       |                      |
       |                      |
       +-->--  isword  ---->--+
       |                      |
       |                      |
       +-->--  mkstring  -->--+
.IE
.IB
!p-string !operator:

-->--+-->-- isstring --->--+-->--
     |                     |
     +-->--  mkword   -->--+
.IE
.IB
!p-list !operator :                       !p-lucid !operator:

--->--+-->-- hd ----->----+-->--        -->--+-->-- first -->--+-->--
      |                   |                  |                 |
      +-->-- tl  ---->----+                  +-->-- next --->--+
      |                   |
      +-->-- islist --->--+
      |                   |  
      +-->-- isatom --->--+
      |                   |
      +-->-- isnull --->--+
.IE
.IB
!p-special !operator

--->---+--->--  iseod   ---->--+--->--
       |                       |
       +--->--  iserror  --->--+
.IE
.IB
!infix !operator :

--->--+-->-- !i-numeric !operator --->--+--->--
      |                               |
      +-->-- !i-word !operator ----->---+
      |                               |
      +-->-- !i-string !operator --->---+
      |                               |
      +-->-- !i-list !operator ----->---+
      |                               |
      +-->-- !i-lucid !operator ---->---+
.IE
.IB
!i-numeric !operator :                    !i-word !operator:

--->--+-->----   +    --->---+-->--     --->--+-->-- or  --->--+--->--
      |                      |                |                |
      +---->---  -    --->---+                +-->-- and --->--+
      |                      |                |                |
      +---->---  *    --->---+                +-->-- eq --->---+
      |                      |                |                |
      +---->---  div  --->---+                +-->-- ne --->---+
      |                      |
      +---->---  mod  --->---+
      |                      |
      +---->---   /  --->----+
      |                      |          !i-string !operator:
      +---->---  **  --->----+
      |                      |          --->--+-->--  ^  -->--+-->--
      +---->---  eq  --->----+                |               |
      |                      |                +-->--  eq -->--+
      +---->---  ne  --->----+                |               |
      |                      |                +-->--  ne -->--+
      +---->---  <   --->----+
      |                      |          !i-list !operator:
      +---->---  >   --->----+
      |                      |          -->--+-->--  <>  -->--+-->--
      +---->---  >=  --->----+               |                |
      |                      |               +-->--  ::  -->--+
      +---->---  <=  --->----+               |                |
                                             +-->--  eq  -->--+
                                             |                |
                                             +-->--  ne  -->--+
.IE
.IB
!i-lucid !operator :

--->--+-->-- fby ----->-----+-->--
      |                     |
      +-->-- whenever --->--+
      |                     |
      +-->-- wvr ------>----+
      |                     |
      +-->-- upon  ---->----+
      |                     |
      +-->-- asa ------>----+
      |                     |
      +-->-- attime --->----+
.IE
.IB
!3expression

--->- !expression -->- , -->- !expression -->- , -->- !expression -->-
.IE
.IB
!list !expression:
                
               +---->------------------------------>--------+ 
               |                                            |                   
--->-- [% -->--+--+-------->- !expression !item  ----+-->---+-->- %] ->-
                  |                                  |      
                  +-----------<-- , --------<--------+
.IE
.IB
!expression !item:

-->----+--->-- !expression ---->----+-->---
       |                           |
       +-->-- !list !expression -->--+
.IE
.IB
!if !expression :

--->-- if -->-- !expression -->-- then -->-- !expression -->-- !endif -->--
.IE
.IB
!endif:

--->--+-->-- else -->-- !expression -->-- fi -------->--------------+-->--
      |                                                            |
      |                                                            |
      V                                                            |
    elseif                                                         ^
      |                                                            |
      |                                                            |
      +-->-- !expression -->-- then -->-- !expression -->-- !endif ---+
.IE
.IB
!case !expression:

-->-- case -->-- !expression --->-- of -->-- !case !body -->-- end -->--
.IE
.IB
!case !body:

-->--+----------------->-------------------------------------+-->--+
     |                                                       |     |
     +--<-- ; --<-- !expression --<-- : --<-- !expression --<--+     |
                                                                   V
                                                                   |
  --<--  end  --<-- ; --<-- !expression --<-- : --<-- default  --<--+
.IE
.IB
!cond !expression:

-->-- cond ---------->--------------------- !cond !body -->-- end -->--
.IE
.IB
!cond !body:

-->--+----------------->-------------------------------------+-->--+
     |                                                       |     |
     +--<-- ; --<-- !expression --<-- : --<-- !expression --<--+     |
                                                                   V
                                                                   |
  --<--  end  --<-- ; --<-- !expression --<-- : --<-- default  --<--+
.IE
.IB
!function !call :

-->-- !identifier -->-- ( -->-- !actuals !list -->-- ) --->--
.IE
.IB
!actuals !list :

--->-- !expression ---+-------------->----------------+-->--
                     |                               |
                     |                               |
                     +--- !expression --<--  ,  --<---+
.IE
.IB
!where !clause :

--->-- !expression --->-- where --->-- !body --->-- end --->--
.IE
.IB
!body :

--->--+---->------------------+-->--+---->------------------+--->--
      |                       |     |                       |
      +--<-- !declaration --<--+     +--<-- !definition ---<--+
.IE
.IB
!declaration :

-->-- !identifier -->-- is current -->-- !expression -->-- ; -->--
.IE
.IB
!definition :

--->--+--->-- !simple  !definition ----->--+--->--
      |                                 |
      +--->-- !function  !definition --->--+
.IE
.IB
!simple  !definition :

--->-- !identifier --->-- = --->-- !expression --->-- ; --->--
.IE
.IB
!function  !definition :

-->-- !identifier -->-- ( -->--+-->-- !identifier -->--+-->-- ) -->--+
                              |                      |             |
                              +----<-----  ,  ---<---+             |
                                                                   V
                                                                   |
                       ---<-- ; --<--- !expression --<---  =  --<---+
.IE

.bp
.SH
7.3 Programming Example
.PP
The following is an example of how a large program would
be organised in pLucid. The example is of a screen editor
and it illustrates many of the features of the pLucid system.

The macro used to run the screen editor
.IB
trap 'reset' 2
stty -echo cbreak;
luval -t10 -c -s screen.i Sl 2>prompts;
stty echo -cbreak
.IE
.PR

// viscid - a vi-like-screen-editor-in-Lucid

chargen(cdecode(C))

where

cdecode(C) =     //turn raw stream of chars into stream of commands
cmd
 where
  include "cdecoder";
 end;

chargen(cmd) =      //generate the control chars from the commands
chars
 where
  include "chargen";
  include "workingtext"; //variables representing current state of working text
  include "escseqs";     // control sequences for the pt100b
  include "strings";     // useful string operations
  include "gather";      // functions for turning lists into streams
                         // and vice versa
 end;

end
.PE
.bp
The file @cdecoder&.
.PR


 namedcmd =                           //the command named by C
               case C of 
                 `i':"beginsert";  //begin inserting text
                 `h':"left";       //move cursor left
                 ` ':"right";      //move cursor right
                 `k':"up";         //move cursor up one line
                 `j':"down";       //move cursor down one line
                 `o':"open";       //open a line below the cursor
                 `x':"chdel";      //delete character
                 `w':"write";      //write to file
                 `X':"linedel";    //delete current line
                 `D': "ldel";      //delete line to right of cursor
                 `Z':"finish";     //write and exit
                 default:"error"; 
               end;

 cmd = namedcmd fby
                 case cmd of
                  "beginsert": if next C eq `\33' then "endinsert"
                                   else "moreinsert" fi;
                  "moreinsert": if next C eq `\33' then "endinsert"
                                   else "moreinsert" fi;
                  "open"     : "moreinsert";
                  "finish"   : "quit";
                  default : next namedcmd;
                 end;

.PE 
.bp
The file @chargen&.
.PR
//generate the stream of screen control characters

chars =  //stream of strings sent to terminal
        CLEAR^HOME^lineconc(initialtext)^HOME
        fby
        case cmd of
         "moreinsert": C^rhs^back(length(rhs));
         "right":RIGHT;
         "left":LEFT;
         "up":UP;
         "down":DOWN;
         "chdel":stl(rhs)^` '^back(length(rhs));
         "open": DSCROLL(lineno+1);
         "ldel":space(length(rhs))^POSITION(lineno,colno-1);
         "linedel":USCROLL(lineno) ^ if llines eq [] then UP else `' fi;
         "write": if iseod output then BEEP else BEEP fi;
         "quit":if iseod output then eod else eod fi;
         "close":NULL;
         "finish":BOTTOM;
         default: `';
        end;

 lineno = length(ulines)+1;

 colno = 1 fby case cmd of
                "left": colno-1;
                "right": colno+1;
                "endinsert":colno+length(insert);
                "ldel":colno-1;
                "open":1;
                "linedel": if llines eq [] then lineno-1 else lineno fi;
                default:colno;
               end;

 insert = `' fby if cmd eq "moreinsert" then insert^C else `' fi;

 rhs = srear(cline,colno-1) ;

 initialtext = if filename eq `' then [] else linesof(filename) fi;

 filename = arg(1);

 output = first filter(`cat  >'^filename,TEXT fby eod,`s')
            where TEXT is current text;end;

 text = lineconc(ulines<>[%cline%]<>llines);
.PE

.bp
The file @workingtext&.
.PR
//the definitions of the variables which represent the
//the current working text

ulines =  //list of lines above the cursor
         [] fby
         case cmd of
          "up":tl(ulines);
          "down":cline :: ulines;
          "open":cline :: ulines;
          "linedel":if llines ne [] then ulines else tl(ulines) fi;
          default : ulines;
         end;

llines =  //list of lines below the cursor
         tl(initialtext)
         fby
         case cmd of
          "up": cline :: llines;
          "down":tl(llines);
          "linedel": if llines ne [] then tl(llines) else [] fi;
          default : llines;
         end;

cline =  // the line the cursor is on
         hd(initialtext)
           fby case cmd of
                "up": hd(ulines);
                "down":hd(llines);
                "endinsert":sfront(cline,colno-1)^insert^srear(cline,colno-1);
                "chdel":sfront(cline,colno-1)^srear(cline,colno);
                "ldel":sfront(cline,colno-1);
                "linedel":if llines ne [] then hd(llines) else hd(ulines) fi;
                "open":`';
                default:cline;
               end;
.PE

.bp
The file @escseqs&.
.PR
//the "escape sequences" for the pt100b

BEEP = `\\\\7';

CLEAR = `\\\\33[2J';     //clear the screen

HOME = `\\\\33[;H';     // send cursor to upper right hand corner

RIGHT = `\\\\33[C';     //move right

LEFT = `\\\\b';       //move left

UP = `\\\\33M';       //up one row

DOWN = `\\\\33D' ;    //down one row

BOTTOM = `\\\\33[24;1H'; //go to bottom left hand corner

DSCROLL(i) = // control chars to scroll down lines i thru 24
              // cursor left at beginning of line i (now blank)
        `\\\\33[' ^ n ^ `;24r\\\\33[' ^ n ^ `;1H\\\\33M\\\\33[1;24r\\\\33[' ^ n ^ `;1H'
           where
            n = numeral(i);
           end;

USCROLL(i) = // control chars to scroll up lines i thru 24
              // cursor at beginning of line i
        `\\\\33[' ^ n ^ `;24r\\\\33[24;1H\\\\33D\\\\33[1;24r\\\\33[' ^ n ^ `;1H'
           where
            n = numeral(i);
           end;

POSITION(r,c) =   // move cursor to the rth row and cth column
                    `\\\\33[' ^ numeral(r) ^ `;' ^ numeral(c) ^ `H';
.PE
.bp
The file @strings&.
.PR
 //a package of functions for manipulating strings

 stl(s) = substr(s,2,length(s));
 sfront(s,n) = substr(s,1,n);
 srear(s,n) = substr(s,n+1,length(s));

 lconc(h) = first s           //concat a list of strings
             where
              H is current h;
               eltH = element(H);
               s = if iseod eltH then `' else eltH^next s fi;
             end;

 space(n) = substr(`                                              ',1,n);
 back(n) = substr(`\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b\\\\b,1,n);
 //(the strings of spaces and backspaces have been shortened for this listing)

 numeral(n) = s asa k eq 0
               where
                N is current n;
                 k = N fby k div 10;
                 r = (k mod 10) +1;
                 digit = substr(`0123456789',r,r);
                 s = `' fby digit^s;
               end;

 linesof(f) =   //a list of the lines in the file f, each line a string
              first linelist
                where
                 F is current f;
                  c = filter(`cat -u ' ^ F,0,`ci');
                  ll = if c eq `\\\\n' then `' else c ^ next ll fi;
                  line = ll whenever true fby c eq `\\\\n' ;
               linelist = if iseod line then [] else line :: next linelist fi;
                end;

 concup(s) = first t //concatenate a stream of strings
              where
               t = if iseod s then `' else s ^ next t fi;
              end;

 lineconc(l) = if iseod l then eod else //concatenate a list of strings
                (concup( element(L)^`\\\\n' ) //with newlines between
                where L is current l; end) fi;
.PE
.bp
The file @gather&.
.PR
//filters for transforming finite lists into streams and vie versa,
//streams 'terminated' by eod

element(x) = hd(tx)      //the stream of elements of the list x
              where
                tx = if first x eq [] then eod else first x fi fby
                     if tl(tx) ne [] then tl(tx) else eod fi;
              end;

gather(x) = first L     //a list of the values in the stream x
             where
               L = if iseod x then [] else x :: next L fi;
             end;

linkup(x) = first L     //concatenate a stream of lists
             where
               L = if iseod x then [] else x <> next L fi;
             end;

ever(p) = first q      //true if p is ever true
              where
                q = if iseod p then false elseif
                             p then true  else
                        next q fi;
              end;
.PE
