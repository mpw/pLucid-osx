.SP 29
.SH
4 Scope Rules
.PP
The scope of an occurrence of a variable, in a pLucid
@where clause, is either !local or !!global&. It is local
if it is either defined or declared in that clause.
The only declaration
we have in pLucid is the @@is current& declaration.
The variable occurring to the left of the declaration
is local to that clause, while any variable occurring in the
expression to the right is global. Moreover, if the variable
is neither declared nor defined in the clause then its
occurrence is global.
.PP
If an occurrence is global to a clause
its value is expected to come from an outer
clause, the first outer clause in which that variable is local.
This also applies on the outermost clause. If
a variable is left global in that clause, its
value is supposed to come from an outer clause,
which is the user environment. Consequently, the
machine asks for that value as an input. For example:
.PR
       X + y + z where
                  X is current x + y ;
                  y = 12 + z ;
                 end
.PE
The occurrences of @X and @y in @@X + Y + Z&  are local to the
@where clause
as is the first occurrence of @X in @@X is current x + y&.
Also @y in @@Y = 12 + z& is local.
The occurrence of @y and the second occurrence of @x in
@@X is current x + y& are both global to the clause,
as is @z in @@y = 12 + z&. Note pLucid binds variables dynamically.
