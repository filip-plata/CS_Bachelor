function [x, R, B] = Householder (A, y)
  [m, n] = size(A);
  
  R = A;
  B = [];
  
  % Calculate Householder decomposition
  
  for j = 1:n
    base = [norm(R(j:end, j)); zeros(m-j, 1)];
    
    mult = sign(R(j, j));
    if (mult == 0)
      mult = 1;
    endif
    
    u = R(j:end, j) + mult .* base;
    u = u ./ norm(u);
    B = [B, [zeros(j-1, 1) ;u]];
    R(j:end,:) = R(j:end,:) - (2/(norm(u) ** 2) * u) *(transpose(u) * R(j:end,:));
    R(j+1:end,j) = 0;
  endfor
  R = R(1:n, 1:n);
  
  % Calculate y1 = transpose(Q) * y
  y1 = y;
  for h = B
    y1 = y1 - (2 / (norm(h) ** 2) .* dot(transpose (h), y1)) .* h;
  endfor
  
  % Solve Rx = y1
  % The matrix and vector are truncated
  x = ones(n, 1);
  
  for i = 1:n
    prev = y1(n+1-i);
    for j = 1:(i-1)
      prev -= R(n+1-i, n+1-j) * x(n+1-j);
    endfor
    x(n+1-i) = prev / R(n+1-i, n+1-i);
  endfor
endfunction
