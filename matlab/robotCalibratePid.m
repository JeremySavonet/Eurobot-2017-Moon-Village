clear variables
% close all
clc


load testNumber
fileName = sprintf('data\\pid%03d', testNumber);
load(fileName)


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

startPosition = [];

for i = 1:length(data)
	
	figure
	ax = [];
	for j = 1:2
		if j==1
			pid = data(i).pidA;
		else
			pid = data(i).pidD;
		end
		
		if isempty(pid)
			continue;
		end

		
		ax(end+1) = subplot(2,2,j);
		hold on
		plot(pid.time,pid.measure,'R')
		plot(pid.time,pid.target,'B')
		plot(pid.time,pid.filteredTarget,'k')
		
		plot(pid.time,pid.speed,'G')
		plot(pid.time,pid.acc,'y')

		ax(end+1) = subplot(2,2,j+2);
		plot(pid.time,pid.output)
		
	end
	linkaxes(ax,'x')
	
	

end


