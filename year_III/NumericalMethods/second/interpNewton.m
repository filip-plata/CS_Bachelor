# x - points
# y - values
# c - polynomial in Newton base
function [c] = interpNewton (x, y)
  n = size(x, 1) - 1;
  c = zeros(n+1, 1);
  c(1) = y(1);
  
  for j = 1:n
    for k = 1:(n+1-j)
      y(k) = (y(k+1) - y(k)) / (x(k+j) - x(k));
    endfor
    c(j+1) = y(1);
  endfor
endfunction
