# Evaluates basis B-spline at point x
function [y] = BsplnEval (x)
  x = abs(x) ;
  if x>2,
    y = 0;
  else
    if x>1,
      y=(2 .- x) .^ 3 ./ 6;
    else
      if x < -2,
        y = 0
      else
       y=2/3 .- x .^ 2 .* (1 .- x/2);
      end
    end
  end
endfunction
