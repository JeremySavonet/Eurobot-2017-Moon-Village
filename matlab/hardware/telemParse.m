function [r,theta,thetaInterp] = telemParse(data)

r = [];
thetaD = [];
thetaS = [];

while 1
	if length(data)<85*2
		break;
	end
	
	sync = bitand(data(1),hex2dec('F0')) + bitand(data(2),hex2dec('F0'))/16;
	if sync~=hex2dec('A5')
		%warning('sync')
		data = data(2:end);
		continue;
	end
	
	checksum = bitand(data(1),hex2dec('0F')) + bitand(data(2),hex2dec('0F'))*16;
	chk = uint8(0);
	for i = 3:84
		chk = bitxor(chk,data(i));
	end
	if checksum~=chk
		warning('chk')
		data = data(2:end);
		continue;
	end
	
	angle = double(data(3)) + double(bitand(data(4),hex2dec('7F')))*256;
	data = data(5:end);
	
	d = zeros(32,1);
	a = zeros(32,1);
	for i = 0:15
		d(2*i+1) = double(bitand(data(i*5+1),252))/4 + double(data(i*5+2))*64;
		d(2*i+2) = double(bitand(data(i*5+3),252))/4 + double(data(i*5+4))*64;
		a(2*i+1) = double(typecast( bitand(data(i*5+1),3)*16*4 + bitand(data(i*5+5),15)*4 ,'int8'))/4;
		a(2*i+2) = double(typecast( bitand(data(i*5+3),3)*16*4 + bitand(data(i*5+5),240)*4 ,'int8'))/4;
	end
	
	data = data(16*5+1:end);
	r = [r,d];
	thetaS = [thetaS,angle];
	thetaD = [thetaD,a];
end

NN = length(thetaS);

thetaS = thetaS/2^6;
thetaD = thetaD/2^3;

thetaSD = diff(thetaS);
thetaSD(thetaSD<0) = thetaSD(thetaSD<0)+360;

theta = ones(32,1)*thetaS(1:end-1) + (1:32)'/32*thetaSD - thetaD(:,1:end-1);

r = reshape(r(:,1:end-1),1,[]);
theta = reshape(theta,1,[]) /180*pi;

% thetaInterp = ones(32,1)*thetaS(2:end) + (1:32)'/32*thetaSD;
thetaInterp = ones(32,1)*thetaS(1:end-1) + (1:32)'/32*thetaSD;
thetaInterp = reshape(thetaInterp,1,[]) /180*pi;


