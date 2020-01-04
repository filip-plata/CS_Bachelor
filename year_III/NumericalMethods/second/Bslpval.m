# z - points in which to calculate estimation
# c - coefficients of qubc spline
# a - start of the interval
# b - end of the interval
function [v] = Bslpval (z, c, a, b)
  t = (z .- a) ./ (b .- a) .* (length(c)-1) + 1;

  v = arrayfun(@(x) BslpvalSingle(x, c, a, b), t);
endfunction
