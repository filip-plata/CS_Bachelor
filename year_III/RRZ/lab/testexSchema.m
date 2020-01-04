

%script testing differential equation solving schema

solver = @(vf, a, b, x0, N) exRungeKutta(vf, a, b, x0, N);

%1d example 
al=1;N=10;a=0;b=1;x0=1;
vf=@(x,t) al*x;
%plot(t,x,";exE;",t,exp(al*t),";sol;");
%pause(3);
%plot(t,x-exp(al*t),";xh-sol;");pause(3);

%now the convergence order by halving the step 
N=20;a=0;b=1;x0=1;
r(1)=1;
for k=1:6,
 NN(k)=N;
 [x,t]=solver(vf,a,b,x0,N);
 er(k)=x(N+1)-exp(al*b);
 if(k>1) r(k)=er(k-1)/er(k);end
 N*=2;
end
printf("1st order example dx/dt=x: testing order by the halving step method \n");
[(b-a)./NN;er;r;log2(abs(r))]'
%Okay linear convergence - 2nd column close to 2; 3rd to one


%%2D example:
%vf=@(x,t) [0,1;-1,0]*x;N=100;a=0;b=8.3;x0=[1;0];[x,t]=exEuler(vf,a,b,x0,N);
%sol=@(t) [cos(t);-sin(t)];
%plot(t,x,";exE;",t,sol(t')',";sol;");pause(3);
%plot(t,x-sol(t')',";xh-sol;");pause(3);
%
%
%%now the convergence order by halving the step 
%N=70;a=0;b=7;x0=[1;0];
%sol=@(t) [cos(t);-sin(t)];
%r(1)=1;
%for k=1:7,
% NN(k)=N;
% [x,t]=exEuler(vf,a,b,sol(a),N);
% er(k)=norm(x(N+1,:)-sol(b)');
% if(k>1) r(k)=er(k-1)/er(k);end
% N*=2;
%end
%printf("2d order example: linear penduluum: testing order by the halving step method \n");
%[(b-a)./NN;er;r;log2(abs(r))]'
%plot(t,x,";exE;",t,sol(t')',";sol;");pause(3);
%plot(t,x-sol(t')',";xh-sol;");pause(3);
%%Okay 
