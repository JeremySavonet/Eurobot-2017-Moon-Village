function telemStart(tcp)

% stop
telemStop(tcp);
pause(1);

% health
d = uint8([]);
d(end+1) = hex2dec('A5');
d(end+1) = hex2dec('52');
chk = uint8(0);
for i = 1:length(d)
	chk = bitxor(chk,d(i));
end
d(end+1) = chk;

fwrite(tcp,d);
pause(0.1)

k = tcp.BytesAvailable;
if(k>0)
	data = uint8(fread(tcp,k,'uint8'));
end
if( k==0 || sum(data(1:7)~=[165 90 3 0 0 0 6]')~=0)
	error('health')
end


% express scan
d = uint8([]);
d(end+1) = hex2dec('A5');
d(end+1) = hex2dec('82');
d(end+1) = 5;	% payload size
d(end+1) = 0;
d(end+1) = 0;
d(end+1) = 0;
d(end+1) = 0;
d(end+1) = 0;
chk = uint8(0);
for i = 1:length(d)
	chk = bitxor(chk,d(i));
end
d(end+1) = chk;

fwrite(tcp,d);
pause(0.1)

k = tcp.BytesAvailable;
if(k>0)
	data = uint8(fread(tcp,k,'uint8'));
end
if( k==0 || sum(data(1:7)~=[165 90 84 0 0 64 130]')~=0)
	error('scan')
end


