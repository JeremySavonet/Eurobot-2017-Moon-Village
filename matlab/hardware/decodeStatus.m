function [msg] = decodeStatus( msg)
% Decode STATUS messages from WESTBOT
%
% Sntaxe:
%     msg = decodeStatus( msg)
% Input:
%     msg           parsed messages
% Output:
%     msg           decoded odometry messages
%         .progress     time
%         .x            position x (mm)
%         .y            position y (mm)
%         .theta        angle theta (rad)


hardware = hardwareGetConfig();
config = hardware.protocol;
config.msgId = config.msgDest.MATLAB + config.msgId.STATUS;


data = cell2mat(msg(msg.id==config.msgId,:).data);
if isempty(data)
	progress   	= [];
	x			= [];
	y			= [];
	theta		= [];
else
	i = 1+config.sizeSize+2+2;
	progress	= double(typecast( reshape(uint8(data(:,i+(1)))',[],1), 'uint8'));	i = i+1;
	x			= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	y			= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	theta		= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	
end

msg = table(progress,x,y,theta);


end

