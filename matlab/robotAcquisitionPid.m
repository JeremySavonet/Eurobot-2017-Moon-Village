clear all
close all
% clc
addpath(genpath('hardware'));

config.ip = '192.168.10.1';
config.port = 5900;
config.pidA.port = 1000;
config.pidD.port = 1001;

config.cmd = [
% 	0 0 0
% 	0 0 0.1
	800 800 0
	1600 0 0
	800 -800 0
	0 0 0
% 	0 0 0.1
% 	1000 1000
% 	800 0
% 	1000 -1000
% 	300 0
	];

%%

try
	fclose(instrfind);
catch
end


tcp.h = tcpip(config.ip, config.port, 'NetworkRole', 'client', 'InputBufferSize',10e3);
fopen(tcp.h);
if ~strcmp(get(tcp.h,'Status'),'open')
	error 'TCP closed'
end
tcp.data = uint8([]);


pidA.h = tcpip(config.ip, config.pidA.port, 'NetworkRole', 'client', 'InputBufferSize',3e6);
fopen(pidA.h);
if ~strcmp(get(pidA.h,'Status'),'open')
	error 'TCP closed'
end
pidA.data = [];

pidD.h = tcpip(config.ip, config.pidD.port, 'NetworkRole', 'client', 'InputBufferSize',3e6);
fopen(pidD.h);
if ~strcmp(get(pidD.h,'Status'),'open')
	error 'TCP closed'
end
pidD.data = [];

pause(1);
fread(pidD.h,pidD.h.BytesAvailable,'uint8');
fread(pidA.h,pidA.h.BytesAvailable,'uint8');


time = 10;
cmd = 1;
exitLoop = 0;
data = [];

pos.x = config.cmd(cmd,1);
pos.y = config.cmd(cmd,2);
pos.theta = config.cmd(cmd,3);
fwrite(tcp.h,encodeCmdPosition(pos));

while ~exitLoop
	
	pause(0.01);
	time = time+1;
	
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
		
% 		pause(1);
% 		if pidA.h.BytesAvailable>0
% 			pidA.data = [pidA.data; fread(pidA.h,pidA.h.BytesAvailable,'uint8')];
% 			[d,pidA.data] = protocolParse(pidA.data);
% 			d = decodePid(d);
% 			data(cmd).pidA = d;
% 		end
% 		if pidD.h.BytesAvailable>0
% 			pidD.data = [pidD.data; fread(pidD.h,pidD.h.BytesAvailable,'uint8')];
% 			[d,pidD.data] = protocolParse(pidD.data);
% 			d = decodePid(d);
% 			data(cmd).pidD = d;
% 		end
		
		if ~exitLoop
			cmd = cmd+1;
			pos.x = config.cmd(cmd,1);
			pos.y = config.cmd(cmd,2);
			pos.theta = config.cmd(cmd,3);
			fwrite(tcp.h,encodeCmdPosition(pos));
		end
	end
	
	fprintf('\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bcmd %d - % 3d%% - %d %d', cmd, msgStatus.progress(end), msgStatus.x(end), msgStatus.y(end));
end

pidA.h.BytesAvailable
pidD.h.BytesAvailable

cmd = 1;
if pidA.h.BytesAvailable>0
	pidA.data = [pidA.data; fread(pidA.h,pidA.h.BytesAvailable,'uint8')];
	[d,pidA.data] = protocolParse(pidA.data);
	d = decodePid(d);
	data(cmd).pidA = d;
end
if pidD.h.BytesAvailable>0
	pidD.data = [pidD.data; fread(pidD.h,pidD.h.BytesAvailable,'uint8')];
	[d,pidD.data] = protocolParse(pidD.data);
	d = decodePid(d);
	data(cmd).pidD = d;
end


fclose(tcp.h);
fclose(pidA.h);
fclose(pidD.h);
fprintf('\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bAcquisition done\n');

load testNumber
testNumber = testNumber+1;
save('testNumber','testNumber')
fileName = sprintf('data\\pid%03d', testNumber);
save(fileName,'data')


robotCalibratePid;

