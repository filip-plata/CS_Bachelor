% Not a function
1;

test_m = [10, 20, 100];
a = 1;
b = -5;
c = 2;

for m = test_m
  data_x = transpose (linspace(0, 10, m));
  error = rand(m, 1) / 50 - 0.01;
  
  y = a .* data_x .* data_x .+ b .* data_x .+ c .+ error;
  A = [data_x .* data_x, data_x, ones(m, 1)];
  
  [x, R, B] = Householder(A, y);
  
  qr_error = norm (A - calc_qr(R, B));
  
  printf("Results for value m = %d\n", m);
  printf("Absoulte QR decomposition error: %f\n", qr_error);
  printf("Values: a = %d, b = %d, c = %d\n\n", x(1), x(2), x(3));
endfor
