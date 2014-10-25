.PP
.nh
.TL
The pLucid Programmer's manual
.AU
  A.A. Faustini
  S G Matthews (*)
  A AG Yaghi   (**)
.AI
Department of Computer Science
Arizona State University
Tempe 85287
Arizona 
USA
.FS
 (*)  Department of Computer Science, University of Victoria,
      P.O. Box 1700, Victoria BC, Canada V8W 2Y2.
 (**) Department of Computer Science, University of Warwick,
      Coventry CV4 7AL, England.
.FE
.AB
    !pLucid (pronounced "pellucid") is a member of the Lucid family of 
functional dataflow programming languages. A program in !pLucid defines 
a network of continuously operating autonomous processing stations (or filters).
Computation in this network is controlled by the flow of data along arcs that
interconnect the nodes, thus a !pLucid program defines a dataflow net. At the outermost level a !pLucid program is an expression that defines a functional relationship between the sequence of data values that correspond to the program's
entire input activity and the sequennce of values that corresponds to the program's entire output activity. Statements in !pLucid are equations, the left hand side of each equation defining the output of the functional filter defined by the expression on the right hand side of the equation. Thus the !pLucid programmer
writes programs in terms of filters and streams. The data values that make up streams are similar to those of Pop2, namely finite lists, strings, words, reals and integers.

    The current implementation of !pLucid runs under Berkley 
.UX 
4.1bsd on a VAX 11/780. The !plucid evaluator simulates a Lucid dataflow 
machine and consequently !pLucid programs do not run as efficiently on 
the Vax as those written in more conventional von Neumann languages. 
On the other hand pLucid programs can be developed and debugged quickly 
and are much easier to maintain
than programs written in conventional languages.
