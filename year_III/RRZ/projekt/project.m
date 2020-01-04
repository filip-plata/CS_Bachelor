# Start constants

x0 = 1;
v0 = 0;
t0 = 0;
T = 10;
a = 1;
b = 0.12;
A = 1;
B = 0.15;
C = 1;

N = 10000;
y0 = [x0, v0]';

time = linspace(t0, T, N+1)';

# End constants

# Start equation

eq = @(y, t) [y(2), -a*y(2)-b*y(1)-A*y(1)*y(1)+B*cos(C*t)];

# End equation

results = rungeKutta(eq, t0, T, y0, N);

results_x = results(:,1);
results_v = results(:,2);

figure('Position',[0,0,1000,800]);

# Plotting v versus time
subplot(4, 1, 1);
plot(time, results_v, "k-");
title ("v(t)");

# Plotting x versus time
subplot(4,1,2);
plot(time, results_x, "k-");
title ("x(t)");

# Plotting v versus x
subplot(4,1,3);
plot(results_x, results_v, "k-");
title ("v(x)");

# Comparing numerical results for A, B = 0
eq_0 = @(y, t) [y(2), -a*y(2)-b*y(1)];
exact_solution_eq_0 = @(t) (-0.193375 * exp(-0.860555 * t) + 
                           1.19338 * exp(-0.139445 * t));
results_0 = rungeKutta(eq_0, t0, T, y0, N)(:,1);
results_exact_0 = exact_solution_eq_0(time);

# Plotting for A, B = 0
subplot(4,1,4);
plot(time, results_exact_0, "g-", time, results_0, "r-");
title ("Solution for A, B = 0");
