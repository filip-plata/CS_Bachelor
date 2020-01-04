# Prints and creates a chart of results

f = @(x) cos(3 * x);
g = @(x) abs(x);

ranges = [-2 * pi, pi; -10, 10];
distfun{1} = (@(x) cos(3 * x));
distfun{2} = (@(x) abs(x));
names{1} = "cos(3x)";
names{2} = "|x|";

test_n = [4, 64, 16];
POINTS_PRINT = 1000;
POINTS_DRAW = 50;

# IMPORTANT
# First column two columns coresspond to n = 4,
# next two to n = 64.
# Subsequent rows correspond to:
# - Lagraange Interpolation equaly spaced nodes
# - Bspline interpolation equaly spaced nodes
# - Lagraange Interpolation Chebshev spaced nodes
for u = 1:3
  n = test_n(u);
  printf("Nodes in interpolation: %d\n\n", n);
  
  for k = 1:2
    err = 0;
    
    name = names{k};
    fun = distfun{k};
    a = ranges(k, 1);
    b = ranges(k, 2);
    
    test_space_draw = linspace(a, b, POINTS_DRAW)';
    test_space = linspace(a, b, POINTS_PRINT)';
    
    funval = fun(test_space_draw);
    
    x = linspace(a, b, n)'; # nodes for interpolation
    y = fun(x); # value of function in those nodes
    
    printf("----------------------------------------------\n");
    printf("Results for function %s\n\n", name);
    
    poly_c = interpNewton(x,y);
    polynomial_equal = Horner(poly_c, x, test_space);
    err = max(abs(polynomial_equal - fun(test_space)));
    printf("Lagrange interpolation error for %s: %f\n", 
            name, err);
    
    if u <= 2,
      # Hack -> for one value we only print
      polynomial_equal = Horner(poly_c, x, test_space_draw);
      subplot(3, 4, k + 2*(u-1));

      plot(test_space_draw, funval, "k-", 
           test_space_draw, polynomial_equal, "ro");
      title(sprintf ("Poly. equaly, nodes: #%d, function: %s", n, name));
    end
            
    spline_c = Bsplnat(y);
    spline_equal = Bslpval(test_space, spline_c, a, b);
    err = max(abs(spline_equal - fun(test_space)));
    printf("Spline interpolation error for %s: %f\n", 
        name, err);
    
    if u <= 2,
      spline_equal = Bslpval(test_space_draw, spline_c, a, b);
      subplot(3, 4, k+4 + 2*(u-1));
      plot(test_space_draw, funval, "k-", 
           test_space_draw, spline_equal, "ro");
      title(sprintf ("Spline. equaly, nodes: #%d, function: %s", n, name));
    end 
    # Changing to Chebyshev nodes
    m = 0:(n-1);
    x = cos( (pi/2) * (2*m+1)/(n) );
    x = ((a+b)/2 + x*(b-a)/2)';
    y = fun(x); # value of function in those nodes
        
    poly_c = interpNewton(x,y);
    polynomial_chebyshev = Horner(poly_c, x, test_space);
    
    err = max(abs(polynomial_chebyshev - fun(test_space)));
    printf("Chebyshev interpolation error for %s: %f\n", 
        name, err);
    
    printf("\n");
    printf("----------------------------------------------\n");
    
    if u <= 2,
      polynomial_chebyshev = Horner(poly_c, x, test_space_draw);
      subplot(3, 4, k+8 + 2*(u-1));
      plot(test_space_draw, funval, "k-", 
           test_space_draw, polynomial_chebyshev, "ro");
      title(sprintf ("Poly. Chebyshev, nodes: #%d, function: %s", n, name));
    end
  endfor
endfor

figure(1, 'Position',[0,0,1500,800]);