% Guan, Huihua

% This program will display the following responses for gain-lead-lag compensator
% for the parameters found in lab experiment: K [gain], zd [zeros of the lead transfer function], 
% zg [zeros of the lag transfer function], pd [poles of the lead transfer function], 
% and pg [poles of the lag transfer function]

% Transfer function of gain-lead-lag compensator:
% G_c = K * G_d * G_g
%
% G_d = (pd/zd)*((s+zd)/(s+pd))
% G_g = (s+zg)/(s+pg)


% Responses:
% 1. Root Locus Plot
% 2. Bode Plot
% 3. Step Response
% 4. Closed Loop Step Response


syms s;
K = 5; zd = 4; zg = 1; pd=30;pg=0.01;
G_numerator = sym2poly(expand(K * pd* (s+zd)*(s+zg)));
G_denominator = sym2poly(expand(zd*(s+pd)*(s+pg)));

sys =tf(G_numerator, G_denominator);

figure(1)
bode(sys)
figure(2)
rlocus(sys)
figure(3)
step(sys)

closed_sys = feedback(open_sys, 1);
figure(4)
step(closed_sys)
