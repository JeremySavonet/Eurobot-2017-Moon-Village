
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
param.mot.f = 0.000065;		% Nm/(rad/s) frottements

param.wheel.D	= 70;		% mm
param.wheel.l	= 200;		% mm	entraxe

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


contr = param;
contr.mot.filter = 0.00001*10;

param.fadc = 1000000000;


contr.mot.R = contr.mot.R/1.5;
% contr.mot.L = contr.mot.L*1.05;
% contr.mot.f = contr.mot.f*1.1;
% contr.mot.J = contr.mot.J*1.1;
% contr.mot.Kc= contr.mot.Kc*1.2;
% contr.mot.Ke= contr.mot.Ke*1.2;





