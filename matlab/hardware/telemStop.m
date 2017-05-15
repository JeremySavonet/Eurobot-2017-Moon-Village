function telemStop(tcp)

d = uint8([]);
d(end+1) = hex2dec('A5');
d(end+1) = hex2dec('25');
chk = uint8(0);
for i = 1:length(d)
	chk = bitxor(chk,d(i));
end
d(end+1) = chk;

fwrite(tcp,d);
pause(0.1);

k = tcp.BytesAvailable;
if(k>0)
	uint8(fread(tcp,k,'uint8'));
end