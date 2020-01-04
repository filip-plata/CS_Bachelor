% Arguments are matrix R of QR decomposition
% and vectors B in Householder inversions.
% There is no guarantee about norm of vectors
% in B.

function [qr_m] = calc_qr(R, B)
  [m, n] = size(B);
  R = [R; zeros(m-n,n)];
  qr_m = R;
  
  for i = 1:n
    h = B(:, n+1-i);
    qr_m = qr_m - (2/(norm(h) ** 2) * h) * (transpose(h) * qr_m);
  endfor
endfunction
