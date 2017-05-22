clear variables
close all
clc


load('test3')

R = 1;
entraxe = 0.188 ;
resolution = 0.032*pi/1024 ;
telemTheta0 = -0.118;
telemX = 0.20608;
telemY = 0.1011;
telemTheta = atan2(telemY,telemX);
telemR = sqrt(telemX^2+telemY^2);

x0 = 0.035;
y0 = 0;
theta0 = 0;

xLast = x0;
yLast = y0;
thetaLast = theta0;

table = [0 0 -1500 0 1500
	1 0 1500 2000 1500
	0 2000 1500 2000 -1500
	1 2000 -1500 0 -1500
	0 360+22 -1500 360+22 -1500+710
	0 360+22 1500 360+22 1500-710];
table = table/1000;
table = array2table(table,'VariableNames',{'dir' 'ax' 'ay' 'bx' 'by'});

color = ['b' 'r' 'k' 'g' 'm' 'y'];


for i = 1%:length(data)
	
	if ~isempty(data(i).odo)
		ol = data(i).odo.wL;
		or = data(i).odo.wR;

		theta = zeros(size(or),'single');
		x = zeros(size(or),'single');
		y = zeros(size(or),'single');
		theta(1) = thetaLast;
		x(1) = xLast;
		y(1) = yLast;

		for j = 2:length(or)
			sl = ( ol(j)-ol(j-1) ) *resolution;
			sr = ( or(j)-or(j-1) ) *resolution;
			theta(j) = theta(j-1)+(sr-sl)/entraxe;
			d = (sl+sr)/2;
			x(j) = x(j-1) + d*cos((theta(j)+theta(j-1))/2);
			y(j) = y(j-1) + d*sin((theta(j)+theta(j-1))/2);
		end
		thetaLast = theta(end);
		xLast = x(end);
		yLast = y(end);
	end
	
	figure(1)
	hold all
	if ~isempty(data(i).odo)
		plot( x,y )
		plot( data(i).odo.x/1000+x0, data(i).odo.y/1000+y0 , 'r')
		axis equal
		grid on
	end
	
	% 	figure
	% 	if ~isempty(data(i).odo)
	% 		subplot(3,1,1)
	% 		hold on
	% 		plot(x-x0)
	% 		plot( data(i).odo.x/1000,'r')
	% 		subplot(3,1,2)
	% 		hold on
	% 		plot(y-y0)
	% 		plot( data(i).odo.y/1000,'r')
	% 		subplot(3,1,3)
	% 		hold on
	% 		plot(theta)
	% 		plot( data(i).odo.theta,'r')
	% 	end
	
	tr = data(i).telem.r/1000;
	ttheta = data(i).telem.theta;
	
	% drop false measures
	ttheta = ttheta(tr>0.1);
	tr = tr(tr>0.1);
	
	% add error
% 	xLast = xLast +0.05;
% 	thetaLast = thetaLast +0.05;
	
	% changement de repère
	ttheta = thetaLast + telemTheta0 + ttheta;
	tx = xLast + telemR*cos(thetaLast+telemTheta) + tr.*cos(ttheta);
	ty = yLast + telemR*sin(thetaLast+telemTheta) + tr.*sin(ttheta);
	
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
	% figure
	for k = 1:height(table)
		if ~isempty(t(k).dot)
			if table.dir(k)
				x = t(k).dot(:,1);
				y = t(k).dot(:,2);
			else
				y = t(k).dot(:,1);
				x = t(k).dot(:,2);
			end
			ind = 1:length(x);
			
			threshold = 0.007;%[0.03 0.01 0.007];
			for iter = 1:length(threshold)
				d = [x(ind) ones(size(x(ind)))] \ y(ind);
				u = [-d(1) 1];
				u = u/norm(u);
				p = [0 d(2)];
				h = abs(dot( [x y] - ones(size(x))*p , ones(size(x))*u  , 2 ));
				ind = find( h<threshold(iter) );
			end
			t(k).dot = t(k).dot( ind , : );
			% hold on
			% plot(t(k).dot(:,1),t(k).dot(:,2),'x')
			% if table.dir(k)
			%	plot([0 2],[d(2) 2*d(1)+d(2)],'r')
			% else
			% 	plot([d(2) d(1)+d(2)],[0 1],'r')
			% end
			% plot(t2(k).dot(:,1),t2(k).dot(:,2),'k+')
		end
	end
	
	% recherche erreur odo
	mes = [];
	ref = [];
	for k = 1:height(table)
		if ~isempty(t(k).dot)
			x = t(k).dot(:,1);
			y = t(k).dot(:,2);
			xref = table.ax(k);
			yref = table.ay(k);
			if ~table.dir(k)
				mes = [mes ; x y ones(size(x)) zeros(size(x)) ];
				ref = [ref; xref*ones(size(x))];
			else
				mes = [mes ; y -x zeros(size(x)) ones(size(x)) ];
				ref = [ref; yref*ones(size(x))];
			end
		end
	end
	A = mes\ref;
	X = A(3);
	Y = A(4);
	R = [A(1) A(2);-A(2) A(1)];
	R = R*inv(chol(R'*R));
	theta = asin(R(1,2));
	% XX  = inv(R)*[A(3);A(4)];
	% XX  = [A(3);A(4)];
	[X Y theta]
	
	xLast = xLast +X;
	yLast = yLast +Y;
	thetaLast = thetaLast -theta;
	
	clear x y
	x = [];
	y = [];
	for k = 1:height(table)
		if ~isempty(t(k).dot)
		x=[x;t(k).dot(:,1)];
		y=[y;t(k).dot(:,2)];
		end
	end
	
	YY = [R,[0 ;0];[0 0  1]]*[[eye(2),[X;Y]];0 0 1]*[x';y';ones(size(x'))];
	figure
	hold on
	axis equal
	plot(YY(1,:),YY(2,:),'.')
% 	plot( tx, ty,'kx')
	for k = 1:height(table)
		plot( [table.ax(k);table.bx(k)], [table.ay(k);table.by(k)] , color(k) )
		if ~isempty(t(k).dot)
			plot( t(k).dot(:,1) , t(k).dot(:,2) , ['.' color(k)] )
% 			plot( t2(k).dot(:,1) , t2(k).dot(:,2) , ['x' color(k+1)] )
		end
	end
	
	
	continue;
	
	
	
	% recherche droite
	figure
	hold on
	axis equal
	for k = 1:height(table)
		plot( [table.ax(k);table.bx(k)], [table.ay(k);table.by(k)] , color(k) )
		if ~isempty(t(k).dot)
			plot( t(k).dot(:,1) , t(k).dot(:,2) , ['.' color(k)] )
			if table.dir(k)
				d = [t(k).dot(:,1) ones(size(t(k).dot(:,1)))] \ t(k).dot(:,2);
				plot( t(k).dot(:,1) , t(k).dot(:,1)*d(1) + d(2) , color(k) );
			else
				d = [t(k).dot(:,2) ones(size(t(k).dot(:,1)))] \ t(k).dot(:,1);
				plot( t(k).dot(:,2)*d(1) + d(2) , t(k).dot(:,2) , color(k) );
			end
			droite(k,:) = d;
			droiteTheta(k) = asin(d(1));
			droiteNb(k) = size(t(k).dot,1);
			droiteX(k) = mean(t(k).dot(:,1)) - table.ax(k);
			droiteY(k) = mean(t(k).dot(:,2)) - table.ay(k);
		end
	end
	
	errTheta = sum( droiteTheta .* (-2*(table.dir==0)+1)' .* droiteNb ) / sum(droiteNb);
	k = find(table.dir);
	errY = sum( droiteY(k) .* droiteNb(k) ) / sum(droiteNb(k));
	k = find(~table.dir);
	errX = sum( droiteX(k) .* droiteNb(k) ) / sum(droiteNb(k));
	
	[errX errY errTheta]
	
	xLast = xLast -errX;
	yLast = yLast -errY;
	thetaLast = thetaLast +errTheta;

end


%%
break;

figure
hold on
axis equal
for k = 1:height(table)
	plot( [table.ax(k);table.bx(k)], [table.ay(k);table.by(k)] ,color(k) )
end
plot(tx(j),ty(j),'+')

