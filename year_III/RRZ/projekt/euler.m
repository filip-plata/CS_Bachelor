
function [x,t]=euler(vf,a,b,x0,N)

%solves dx/dt=vf(x,t) x(a)=x0
%by explicit Euler scheme
%INPUT
%vf- function handle to vector field : function y=vf(x,t)... end
%a,b,- interval ends
%x0 - initial value in a column i.e. x(a)=x0
%N- nmber of discrete time steps -1 
%OUTPUT
%t - discrete time steps in vector t=[a, a+h, a+2*h,..,a+N*h]
%    h=(b-a)/N
%x - matrix N+1 x dim(x0) in (k+1)th row we have approximation of x(a+k*h)
%    computed by explicite Euler
%EXAMPLE:
%vf=@(x,t) x; N=10;a=0;b=1;x0=1;[x,t]=exEuler(vf,a,b,x0,N);plot(t,x,t,exp(t),";sol;")
%2D example:
%vf=@(x,t) [0,1;-1,0]*x;N=100;a=0;b=8.3;x0=[1;0];[x,t]=exEuler(vf,a,b,x0,N);plot(t,x);

h=(b-a)/N;
t=linspace(a,b,N+1)';
x=zeros(N+1,length(x0));
x(1,:)=x0';
for k=1:N,
 x(k+1,:)=x(k,:)+h*vf(x(k,:)',t(k));
end
end
