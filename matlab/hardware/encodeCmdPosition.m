function msg = encodeCmdPosition( data)
% Encode CMD POSITION messages from WESTBOT device
%
% Sntaxe:
%     msg = encodeCmdPosition( data)
% Input:
%     data          message to send
%         .x            distance (mm)
%         .y            distance (mm)
%         .theta        angle (rad)
% Output:
%     msg           encoded message


hardware = hardwareGetConfig();
config = hardware.protocol;
config.msgId = config.msgDest.ARM + config.msgId.CMD_POSITION;


if istable(data)
	data = table2struct(data);
elseif ~isstruct(data)
	error
end

if ~isfield(data,'x')
	error 'x'
end
if ~isfield(data,'y')
	error 'x'
end
if ~isfield(data,'theta')
	warning 'theta'
	data.theta = 0;
end


msg = single([]);
msg(end+1) = data.x;
msg(end+1) = data.y;
msg(end+1) = data.theta;

msg = protocolEncode( config.msgId, msg);


end

