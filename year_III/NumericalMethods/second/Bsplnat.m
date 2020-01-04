# c - coefficients of B-spline
# y - values in points
function [c] = Bsplnat (y)
  # Assumes consecutive points 1, 2, 3,...
  N = length(y);
  A = zeros(N);

  if N == 1, 
    c = 1.5*y; 
  else
    # From some paper
    A(1,1:2) = [2/3, 1/6];
    A(N, N-1:N) = [1/6, 2/3];
    
    for i = 2:N-1,
      A(i, i-1:i+1) = [1/6, 2/3, 1/6];
    end

    c = A \ y ;
    c = c';
  end
endfunction
