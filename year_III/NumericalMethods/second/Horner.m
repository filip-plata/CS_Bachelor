# w - values of polynomial
# c - coefficients in Newton base
# x - nodes of interpolation
# z - points in which calculate value
function [w] = Horner (c, x, z)
  n = size(z, 1);
  m = size(c, 1);
  w = c(length(c)) * ones(n, 1);
  
  for j = 1:(m-1)
    w = w .* (z - x(m-j)) + c(m-j);
  endfor
endfunction
