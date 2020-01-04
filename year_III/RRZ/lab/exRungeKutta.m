function [x,t]=exRungeKutta(vf,a,b,x0,N)

h=(b-a)/N;
t=linspace(a,b,N+1)';
x=zeros(N+1,length(x0));
x(1,:)=x0';
for k=1:N,
  k1 = vf(x(k,:)',t(k))';
  k2 = vf(x(k,:)' + h/2*k1, t(k) + h/2)';
  k3 = vf(x(k,:)' + h/2*k2, t(k) + h/2)';
  k4 = vf(x(k,:)' + h*k3, t(k) + h)';
  x(k+1,:)=x(k,:)+h/6*(k1 + 2*k2 + 2*k3 + k4);
end
end
