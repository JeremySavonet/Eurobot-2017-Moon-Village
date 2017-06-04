function config = hardwareGetConfig()
% Get hardware configuration
%
% Sntaxe:
%     config = hardwareGetConfig()
% Input:
% Output:
%     config        hardware configuration
%         .protocol     Protocol header configuration


config.protocol.fanion = 165;			% fanion début trame
config.protocol.sizeSize = 2;			% taille du champ taille (16/32bits)
config.protocol.sizeMax = 1000;         % taille maximale d'une trame

name = {'ODOMETRY' 'PID' 'CMD_POSITION' 'STATUS'};
for i = 1:length(name)
	config.protocol.msgId.(cell2mat(name(i))) = i-1;
end
config.protocol.msgId = struct2table(config.protocol.msgId);

name = {'MATLAB' 'ARM'};
for i = 1:length(name)
	config.protocol.msgDest.(cell2mat(name(i))) = (i-1)*2^(8+4);
end
config.protocol.msgDest = struct2table(config.protocol.msgDest);


end %hardwareGetConfig