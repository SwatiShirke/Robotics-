lqr(SYS,Q,R,N)close all;
 
% initialize state close all;
%  
% initialize state 
x0= [0.2;     % 200 mA
    0.0872]   % rad/sec = 300 rpm
 
%motor dynamics 
A = [-2 -0.02;
    1   -0.01]
 
B = [2;
    0]
 
C = [0 1]
 
D = [0]

Q = [1 0;
    0 1]

R = [1]
 
 
%LQR command to find feeback gain K  ()
K = lqr(A,B,Q,R);
% close loop system 
SYS= ss(A-B*K,B,C,D);
% creating time stamps
t= 0: 0.005 : 30;
% stimulate the system 
[y,t,x] = initial(SYS,x0,t);
plot (t,y);

