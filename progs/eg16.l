 find(2,X) - 2 * find(1,X) + find(0,X)
  where
   find(I,A) = if I eq 0 then A else find(I-1,next A) fi;
  end
