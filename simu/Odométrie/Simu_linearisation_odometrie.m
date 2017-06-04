
clear all
close all

%% conclusion
% il est possible de négliger la différence de distance entre un arc de cercle et une ligne droite
% il est nécessaire d'utiliser un angle moyen (moyenne de teta(i) et teta(i-1))
% dt = 0.01s, codeur 1024points, erreur diamètre roue codeuse : .1mm => 1%erreur

%% paramètres

% trajectoire : 1/2 cercle de rayon R
R = 1;
v = 1;
dt = 0.01;
t = 0:dt:2*pi*R/v /2;
L = 0.300;
resolution = 0.032*pi/1024;


%% robot position réelle

p.d = v.*t;				% abscise curviligne
p.r = t.*0+R;			% coordonnée polaire
p.a = p.d./p.r -pi/2;
p.x = p.r.*cos(p.a);	% coordonnée carthésienne
p.y = p.r.*sin(p.a)+R;

% roue gauche
pl.r = p.r-L/2;
pl.a = p.a;
pl.d = pl.r.*pl.a;
pl.x = pl.r.*cos(pl.a);	
pl.y = pl.r.*sin(pl.a)+R;

% roue droite
pr.r = p.r+L/2;
pr.a = p.a;
pr.d = pr.r.*pr.a;
pr.x = pr.r.*cos(pr.a);	
pr.y = pr.r.*sin(pr.a)+R;

figure(1)
plot(p.x,p.y)
axis equal
hold on
plot(p.x(1),p.y(1),'+')
plot(pr.x(1),pr.y(1),'r+')
plot(pl.x(1),pl.y(1),'+')
% plot(pr.x,pr.y)
% plot(pl.x,pl.y)


%% Codeur
pr.d = floor(pr.d/resolution)*resolution;
pl.d = floor(pl.d/resolution)*resolution;


%% Defauts
% pr.d = pr.d/32*32.1;


%% Méthode 1 : calcul sans approximation en utilisant une trajectoire circulaire

teta = zeros(size(t));
x = zeros(size(t));
y = zeros(size(t));
teta(1) = 0;%pi/2;
x(1) = 0;
y(1) = 0;

for i = 2:length(t)
	sl = pl.d(i)-pl.d(i-1);
	sr = pr.d(i)-pr.d(i-1);
	teta(i) = teta(i-1)+(sr-sl)/L;
	%teta(i) = (pr.d(i)-pl.d(i))/L;
	d = L*(sl+sr)/(sr-sl)*sin((sr-sl)/2/L);
	x(i) = x(i-1)+d*cos((teta(i)+teta(i-1))/2);
	y(i) = y(i-1)+d*sin((teta(i)+teta(i-1))/2);
end

figure(1)
plot(x,y,'g')
plot(x(1),y(1),'+g')

figure(2)
hold all
plot(p.d,sqrt((x-p.x).^2+(y-p.y).^2))


%% Méthode 2 : méthode 1 avec simplification angle final

teta = zeros(size(t));
x = zeros(size(t));
y = zeros(size(t));
teta(1) = 0;%pi/2;
x(1) = 0;
y(1) = 0;

for i = 2:length(t)
	sl = pl.d(i)-pl.d(i-1);
	sr = pr.d(i)-pr.d(i-1);
	teta(i) = teta(i-1)+(sr-sl)/L;
	%teta(i) = (pr.d(i)-pl.d(i))/L;
	d = L*(sl+sr)/(sr-sl)*sin((sr-sl)/2/L);
	x(i) = x(i-1)+d*cos((teta(i)+teta(i))/2);
	y(i) = y(i-1)+d*sin((teta(i)+teta(i))/2);
end

figure(1)
plot(x,y,'g--')
plot(x(1),y(1),'+g')

figure(2)
hold all
plot(p.d,sqrt((x-p.x).^2+(y-p.y).^2))


%% Méthode 3 : méthode 1 avec simplification distance arc de cercle

teta = zeros(size(t));
x = zeros(size(t));
y = zeros(size(t));
teta(1) = 0;%pi/2;
x(1) = 0;
y(1) = 0;

for i = 2:length(t)
	sl = pl.d(i)-pl.d(i-1);
	sr = pr.d(i)-pr.d(i-1);
	teta(i) = teta(i-1)+(sr-sl)/L;
	%teta(i) = (pr.d(i)-pl.d(i))/L;
	d = (sl+sr)/2;
	x(i) = x(i-1)+d*cos((teta(i)+teta(i-1))/2);
	y(i) = y(i-1)+d*sin((teta(i)+teta(i-1))/2);
end

figure(1)
plot(x,y,'g-.')
plot(x(1),y(1),'+g')

figure(2)
hold all
plot(p.d,sqrt((x-p.x).^2+(y-p.y).^2))



%% Méthode 3 : méthode 1 avec simplification distance arc de cercle + float

pl.d = single(pl.d);
pr.d = single(pr.d);
L = single(L);

teta = zeros(size(t),'single');
x = zeros(size(t),'single');
y = zeros(size(t),'single');
teta(1) = 0;%pi/2;
x(1) = 0;
y(1) = 0;

for i = 2:length(t)
	sl = pl.d(i)-pl.d(i-1);
	sr = pr.d(i)-pr.d(i-1);
	teta(i) = teta(i-1)+(sr-sl)/L;
	%teta(i) = (pr.d(i)-pl.d(i))/L;
	d = (sl+sr)/2;
	x(i) = x(i-1)+d*cos((teta(i)+teta(i-1))/2);
	y(i) = y(i-1)+d*sin((teta(i)+teta(i-1))/2);
end

figure(1)
plot(x,y,'k--')
plot(x(1),y(1),'+k')

figure(2)
hold all
plot(p.d,sqrt((x-p.x).^2+(y-p.y).^2),'k--')










