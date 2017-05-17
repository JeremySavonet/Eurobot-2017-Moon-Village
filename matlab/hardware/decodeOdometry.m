function [msg] = decodeOdometry( msg)
% Decode ODOMETRY messages from WESTBOT
%
% Sntaxe:
%     msg = decodeOdometry( msg)
% Input:
%     msg           parsed messages
% Output:
%     msg           decoded odometry messages
%         .time         time
%         .wL           qei left
%         .wR           qei right
%         .mL           qei left
%         .mR           qei right
%         .x            position x (mm)
%         .y            position y (mm)
%         .theta        angle theta (rad)


hardware = hardwareGetConfig();
config = hardware.protocol;
config.msgId = config.msgDest.MATLAB + config.msgId.ODOMETRY;


data = cell2mat(msg(msg.id==config.msgId,:).data);
if isempty(data)
	time		= [];
	wL      	= [];
	wR      	= [];
	mL      	= [];
	mR      	= [];
	x			= [];
	y			= [];
	theta		= [];
else
	i = 1+config.sizeSize+2+2;
	time		= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'uint32'));	i = i+4;
	wL          = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'int32'));	i = i+4;
	wR          = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'int32'));	i = i+4;
	mL          = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'int32'));	i = i+4;
	mR          = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'int32'));	i = i+4;
	x			= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	y			= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	theta		= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	
	time = time + cumsum([0;diff(time)<0]) *2^32;
    %time = unwrap(time/2^32*2*pi)/2/pi*2^32;
	time = time*20e-9;
end

msg = table(time,wL,wR,mL,mR,x,y,theta);


end

