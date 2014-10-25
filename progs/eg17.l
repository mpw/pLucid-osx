 df(2,X)
  where
   df(I,A) = if I eq 0 then A else df(I-1,next A- A) fi;
  end
