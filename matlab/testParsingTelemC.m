clear variables
close all
clc

addpath(genpath('hardware'));




file = fopen('scanData.raw');
data = fread(file);

len = length(data)/8/2;
data = typecast(uint8(data),'double');

theta = data(1:len);
distance = data(len+1:2*len);


tx = distance.*cos(theta);
ty = distance.*sin(theta);

figure
plot(tx,ty,'.')
axis equal



%%


telemTheta0 = -0.118;
telemX = 0.20608;
telemY = 0.1011;
telemTheta = atan2(telemY,telemX);
telemR = sqrt(telemX^2+telemY^2);

robotX = 0.036;
% robotY = 0.581;
robotY = -0.610;
robotTheta = 0;


table = [0 0 -1500+600 0 1500-600
	1 380 1500 2000 1500
	0 2000 1500 2000 -1500
	1 2000 -1500 380 -1500
	0 360+22 -1500 360+22 -1500+710
	0 360+22 1500 360+22 1500-710
	2 700    1420 1150   1420
	2 700   -1420 1150  -1420
	];
table(:,2:end) = table(:,2:end)/1000;
table = array2table(table,'VariableNames',{'dir' 'ax' 'ay' 'bx' 'by'});

color = ['b' 'r' 'k' 'g' 'm' 'y' 'c' 'b' 'r' 'k' 'g' 'm' 'y' 'c'];

startPosition = [];


tr = distance/1000;
ttheta = theta;

% drop false measures
ttheta = ttheta(tr>0.1);
tr = tr(tr>0.1);

% changement de repère
ttheta = robotTheta + telemTheta0 + ttheta;
tx = robotX + telemR*cos(robotTheta+telemTheta) + tr.*cos(ttheta);
ty = robotY + telemR*sin(robotTheta+telemTheta) + tr.*sin(ttheta);

figure
plot(tx,ty,'.')
axis equal

% recherche élément le plus proche
for k = 1:size(table,1)
	t(k).dot = zeros(size([tx ty]));
	t(k).len = 0;
end
for j = 1:length(tx)
	for k = 1:size(table,1)
		A = [table.ax(k) table.ay(k)];
		B = [table.bx(k) table.by(k)];
		P = [tx(j) ty(j)];
		
		% AB.AP<0 => distance = AP
		if (table.bx(k)-table.ax(k))*(tx(j)-table.ax(k)) + (table.by(k)-table.ay(k))*(ty(j)-table.ay(k)) < 0
			d(k) = sqrt( (tx(j)-table.ax(k))^2 + (ty(j)-table.ay(k))^2 );
		elseif (table.ax(k)-table.bx(k))*(tx(j)-table.bx(k)) + (table.ay(k)-table.by(k))*(ty(j)-table.by(k)) < 0
			d(k) = sqrt( (tx(j)-table.bx(k))^2 + (ty(j)-table.by(k))^2 );
		else
			% d(k) = norm(cross([B-A 0],[P-A 0]))/norm(B-A);
			d(k) = (table.bx(k)-table.ax(k))*(ty(j)-table.ay(k)) + (table.by(k)-table.ay(k))*(tx(j)-table.ax(k));
			d(k) = abs(d(k));
			d(k) = d(k)/sqrt((table.ax(k)-table.bx(k))^2+(table.ay(k)-table.by(k))^2);
		end
	end
	
	[m,k] = min(d);
	if m<0.15
		t(k).len = t(k).len+1;
		t(k).dot(t(k).len,:) = [tx(j) ty(j)];
	end
end
for k = 1:size(table,1)
	t(k).dot = t(k).dot(1:t(k).len,:);
end

figure
hold on
axis equal
for k = 1:height(table)
	plot( [table.ax(k);table.bx(k)], [table.ay(k);table.by(k)] , color(k) )
	if ~isempty(t(k).dot)
		plot( t(k).dot(:,1) , t(k).dot(:,2) , ['.' color(k)] )
	end
end

% suppression points
figure
for k = 1:height(table)
	if ~isempty(t(k).dot)
		if table.dir(k)==0
			y = t(k).dot(:,1);
			x = t(k).dot(:,2);
		elseif table.dir(k)==1
			x = t(k).dot(:,1);
			y = t(k).dot(:,2);
		else
			t(k).dot = [];
			continue;
		end
		ind = 1:length(x);
		
		threshold = 0.02;%[0.03 0.01 0.007];
		for iter = 1:length(threshold)
			d = [x(ind) ones(size(x(ind)))] \ y(ind);
			u = [-d(1) 1];
			u = u/norm(u);
			p = [0 d(2)];
			h = abs(dot( [x y] - ones(size(x))*p , ones(size(x))*u  , 2 ));
			ind = find( h<threshold(iter) );
		end
		t(k).dot = t(k).dot( ind , : );
		hold on
		plot(t(k).dot(:,1),t(k).dot(:,2),'x')
		if table.dir(k)
			plot([0 2],[d(2) 2*d(1)+d(2)],'r')
		else
			plot([d(2) d(1)+d(2)],[0 1],'r')
		end
		plot(t(k).dot(:,1),t(k).dot(:,2),'k+')
	end
end

mes = [];
ref = [];
for k = 1:height(table)
	if ~isempty(t(k).dot)
		x = t(k).dot(:,1);
		y = t(k).dot(:,2);
		xref = table.ax(k);
		yref = table.ay(k);
		if table.dir(k)
			mes = [mes ; -x zeros(size(x)) ones(size(x)) ];
			ref = [ref; yref*ones(size(x))-y];
		else
			mes = [mes ; y ones(size(x)) zeros(size(x)) ];
			ref = [ref; xref*ones(size(x))-x];
		end
	end
end
A = mes\ref;
X = A(2);
Y = A(3);
theta = asin(A(1));
R = [cos(theta) sin(theta);-sin(theta) cos(theta)];
[X Y theta]

robotXnew =  robotX*cos(theta) +robotY*sin(theta) +X;
robotYnew = -robotX*sin(theta) +robotY*cos(theta) +Y;
robotThetanew = robotTheta -theta;

[robotX robotY robotTheta]
[robotXnew robotYnew robotThetanew]

clear x y
x = [];
y = [];
for k = 1:height(table)
	if ~isempty(t(k).dot)
		x=[x;t(k).dot(:,1)];
		y=[y;t(k).dot(:,2)];
	end
end

YY = [R,[0 ;0];[0 0 1]]*[[eye(2),[X;Y]];0 0 1]*[x';y';ones(size(x'))];
% YY = [A(1) A(2) A(3);-A(2) A(1) A(4);0 0 1]*[x';y';ones(size(x'))];
% YY = [cos(theta) sin(theta) A(3);-sin(theta) cos(theta) A(4);0 0 1]*[x';y';ones(size(x'))];
figure
hold on
axis equal
plot(YY(1,:),YY(2,:),'c.')
% 	plot( tx, ty,'kx')
for k = 1:height(table)
	if table.dir(k)==2
		plot( [table.ax(k);table.bx(k)], [table.ay(k);table.by(k)] , [color(k) '--'] )
	else
		plot( [table.ax(k);table.bx(k)], [table.ay(k);table.by(k)] , [color(k)] )
	end
	if ~isempty(t(k).dot)
		plot( t(k).dot(:,1) , t(k).dot(:,2) , ['.' color(k)] )
		% 			plot( t2(k).dot(:,1) , t2(k).dot(:,2) , ['x' color(k+1)] )
	end
end










%%

break;


file = fopen('scanDataOriginal.raw');
data = fread(file);
[r,~,thetam] = telemParse(uint8(data));


tx = r.*cos(thetam);
ty = r.*sin(thetam);

figure
plot(tx,ty,'.')
axis equal




