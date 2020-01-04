# Evaluates B-spline at single point
# Reference implementation: http://cmp.felk.cvut.cz/~kybic/old/msplines/list.html

function [y] = BslpvalSingle (x, c, a, b)
  lenc = length(c) ;
  xf = floor(x)-1 ; 

  if xf >= 1 && xf <= lenc,
    # first spline
    y = c(xf) * BsplnEval(x-xf) ;
  else 
    y = 0;
  end

  if xf+1 >= 1 && xf+1 <= lenc,
    # second
    y = y + c(xf+1) * BsplnEval(x-xf-1);
  end

  if xf + 2 >= 1 && xf + 2 <= lenc,
    # third
    y = y + c(xf+2)*BsplnEval(x-xf-2);
  end

  if xf + 3 >= 1 && xf + 3 <= lenc,
    # fourth
    y = y + c(xf+3) * BsplnEval(x-xf-3);
  end
endfunction
