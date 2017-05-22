clear all
close all
clc
addpath(genpath('hardware'));

config.ip = '192.168.10.1';
config.port = 5900;
config.odo.port = 1000;
config.telem.port = 1001;

config.cmd = [0 0
	100 0
	1000 1000
	800 0
	1000 -1000
	300 0];

%%

try
	fclose(instrfind);
catch
end


tcp.h = tcpip(config.ip, config.port, 'NetworkRole', 'client', 'InputBufferSize',1e6);
fopen(tcp.h);
if ~strcmp(get(tcp.h,'Status'),'open')
	error 'TCP closed'
end
tcp.data = uint8([]);


telem.h = tcpip(config.ip, config.telem.port, 'NetworkRole', 'client', 'InputBufferSize',1e6);
fopen(telem.h);
if ~strcmp(get(telem.h,'Status'),'open')
	error 'TCP closed'
end
telemStop(telem.h);
telemStart(telem.h);


odo.h = tcpip(config.ip, config.odo.port, 'NetworkRole', 'client', 'InputBufferSize',1e6);
fopen(odo.h);
if ~strcmp(get(odo.h,'Status'),'open')
	error 'TCP closed'
end
odo.data = [];




time = 10;
cmd = 1;
exitLoop = 0;
data = [];
while ~exitLoop
	
	pause(1);
	time = time+1;
	
	if time>10
		pos.x = config.cmd(cmd,1);
		pos.y = config.cmd(cmd,2);
		pos.theta = 0;
		fwrite(tcp.h,encodeCmdPosition(pos));
	end
	
	if tcp.h.BytesAvailable==0
		continue;
	end
	tcp.data = [tcp.data; fread(tcp.h,tcp.h.BytesAvailable,'uint8')];
	[msg,tcp.data] = protocolParse( tcp.data);
	
	msgStatus = decodeStatus( msg);
	if isempty(msgStatus)
		continue;
	end
		
	if msgStatus.progress(end)==100
		if cmd>=size(config.cmd,1)
			exitLoop = 1;
		end
		
		pause(1);
		if odo.h.BytesAvailable>0
			odo.data = [odo.data fread(odo.h,odo.h.BytesAvailable,'uint8')];
			[d,odo.data] = protocolParse(odo.data);
			d = decodeOdometry(d);
			data(cmd).odo = d;
		end

		fread(telem.h,telem.h.BytesAvailable,'uint8');
		pause(0.5);
		d = uint8(fread(telem.h,telem.h.BytesAvailable,'uint8'));
		[r,~,theta] = telemParse(d);
		r = r';
		theta = theta';
		data(cmd).telem = table(r,theta);

		cmd = cmd+1;
		pos.x = config.cmd(cmd,1);
		pos.y = config.cmd(cmd,2);
		pos.theta = 0;
		fwrite(tcp.h,encodeCmdPosition(pos));
	end
	
	fprintf('\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bcmd %d - % 3d%% - %d %d', cmd, msgStatus.progress(end), msgStatus.x(end), msgStatus.y(end));
end

fclose(tcp.h);
fclose(odo.h);
fclose(telem.h);
fprintf('\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bAcquisition done\n');

% save('test3','data')
