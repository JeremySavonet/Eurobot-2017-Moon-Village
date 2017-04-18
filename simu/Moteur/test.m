clear all
close all

R = 2.0;                % Ohms
L = 0.5;                % Henrys
Km = 0.1;               % torque constant
Kb = 0.1;               % back emf constant
Kf = 0.2;               % Nms
J = 0.02;               % kg.m^2/s^2


h1 = tf(Km,[L R]);            % armature
h2 = tf(1,[J Kf]);            % eqn of motion

dcm = ss(h2) * [h1 , 1];      % w = h2 * (h1*Va + Td)
dcm = feedback(dcm,Kb,1,1);   % close back emf loop

figure
stepplot(dcm(1));


%%
Kff = 1/dcgain(dcm(1));

t = 0:0.1:15;
Td = -0.1 * (t>5 & t<10);       % load disturbance
u = [ones(size(t)) ; Td];       % w_ref=1 and Td

cl_ff = dcm * diag([Kff,1]);    % add feedforward gain
cl_ff.InputName = {'w_ref','Td'};
cl_ff.OutputName = 'w';

figure
h = lsimplot(cl_ff,u,t);
title('Setpoint tracking and disturbance rejection')
legend('cl\_ff')

% Annotate plot
line([5,5],[.2,.3]);
line([10,10],[.2,.3]);
text(7.5,.25,{'disturbance','T_d = -0.1Nm'},...
	'vertic','middle','horiz','center','color','r');


%%

figure
h = rlocusplot(tf(1,[1 0]) * dcm(1));
setoptions(h,'FreqUnits','rad/s');
xlim([-15 5]);
ylim([-15 15]);


%%
figure

K = 5;
C = tf(K,[1 0]);            % compensator K/s

cl_rloc = feedback(dcm * append(C,1),1,1,1);
h = lsimplot(cl_ff,cl_rloc,u,t);
cl_rloc.InputName = {'w_ref','Td'};
cl_rloc.OutputName = 'w';
title('Setpoint tracking and disturbance rejection')
legend('feedforward','feedback w/ rlocus','Location','NorthWest')


%%


dc_aug = [1 ; tf(1,[1 0])] * dcm(1); % add output w/s to DC motor model

K_lqr = lqry(dc_aug,[1 0;0 20],0.01);

P = augstate(dcm);                     % inputs:Va,Td  outputs:w,x
C = K_lqr * append(tf(1,[1 0]),1,1);   % compensator including 1/s
OL = P * append(C,1);                  % open loop

CL = feedback(OL,eye(3),1:3,1:3);      % close feedback loops
cl_lqr = CL(1,[1 4]);                  % extract transfer (w_ref,Td)->w

figure
bodeplot(cl_ff,cl_rloc,cl_lqr);

figure
h = lsimplot(cl_lqr,u,t);

[y2,t2,x2] = lsim(cl_lqr,u,t);

figure
plot(t,u)
figure
plot(t,y2)
figure
plot(t,x2)
