
clear all

param.mot.Pu = 15.7;		% W     puissance utile
param.mot.Um = 12;			% V     tension max
param.mot.Im = 5.8;			% A     intensité max
param.mot.n  = 0.52;		% .     rendement
param.mot.Kc = 0.0342;		% Nm/A  constante de couple
param.mot.J  = 105;			% g/cm²	inertie
param.mot.R  = 2;			% ohm   résistance
param.mot.L  = 1.8;			% mH    self
param.mot.rr = 728/45;		% .     réducteur ratio
param.mot.rn = 1;			% .		réducteur rendement
param.mot.fv = 0.000065;	% Nm/(rad/s) frottements
param.mot.fs = 0.065;	% Nm    frottements sec

param.wheel.D	= 70;		% mm
param.wheel.l	= 200;		% mm	entraxe
param.codeur.resolution	= (32*pi)/1024 /1000;		% m/points
% param.codeur.resolution	= (param.wheel.D*pi)/param.mot.rr/(4000*4) /1000;		% m/points

param.robot.M   = 5;		% kg
param.robot.J   = 0.01;		% kg/m²
%param.robot.J = 116961914/1000/1000^2;

param.mot.Ke = param.mot.Kc;	% V/rad/s
param.mot.J  = param.mot.J/1000/100^2;
param.mot.L  = param.mot.L/1000;

param.wheel.D = param.wheel.D/1000;
param.wheel.l = param.wheel.l/1000;



% param.mot.R = param.mot.R/1000;
% param.mot.L = param.mot.L/1000;
% param.mot.J = param.mot.J/1000;
% param.mot.f = param.mot.f/1000;

param.adc.retard = 78/2/144444 ;

contr = param;
contr.mot.filter = 0.0001;


% contr.mot.L = contr.mot.L/1.1;
% param.mot.R = param.mot.R/1.4;

% contr.mot.fv = contr.mot.fv/2;
% contr.mot.fs = contr.mot.fs/2;
% contr.mot.J = contr.mot.J*1.5;
% contr.mot.Kc= contr.mot.Kc/1.5;
% contr.mot.Ke= contr.mot.Ke*1.5;
% contr.adc.retard = contr.adc.retard +0.0001;





