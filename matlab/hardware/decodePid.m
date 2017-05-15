function [msg] = decodePid( msg)
% Decode PID messages from WESTBOT
%
% Sntaxe:
%     msg = decodeOdometry( msg)
% Input:
%     msg           parsed messages
% Output:
%     msg           decoded odometry messages
%         .time         time
%         .speed        
%         .acc        
%         .target        
%         .filteredTarget        
%         .measure        
%         .output        


hardware = hardwareGetConfig();
config = hardware.protocol;
config.msgId = config.msgDest.MATLAB + config.msgId.PID;


data = cell2mat(msg(msg.id==config.msgId,:).data);
if isempty(data)
	time		= [];
	speed      	= [];
	acc      	= [];
	target     	= [];
	filteredTarget= [];
	measure		= [];
	output		= [];
else
	i = 1+config.sizeSize+2+2;
	time            = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'uint32'));	i = i+4;
	speed           = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	acc             = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	target          = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	filteredTarget  = double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	measure			= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	output			= double(typecast( reshape(uint8(data(:,i+(1:4)))',[],1), 'single'));	i = i+4;
	
    time = unwrap(time/2^32*2*pi)/2/pi*2^32;
	time = time*20e-9;
end

msg = table(time,speed,acc,target,filteredTarget,measure,output);


end

