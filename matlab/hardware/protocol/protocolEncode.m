function raw = protocolEncode( id, raw)
% Encode data message into raw message using IOCENE protocol
%
% Sntaxe:
%     raw = protocolEncode( msg)
% Input:
%     id              message identifier
%     msg             data messages
% Output:
%     raw             raw data to send


config = hardwareGetConfig();
config = config.protocol;

if ismatrix(raw)
	raw = typecast(raw,'uint8');
elseif istable(raw)
	error
end


%msgNb
msgNb = size(raw,1);

%id
if ischar(id)
	value = hex2dec(id);
else
	value = id;
end
value = uint16( value .*ones(msgNb,1) );
raw = [typecast(value,'uint8') raw];

%checksum
value = sum(uint64(raw),2);
value = uint16( value .*ones(msgNb,1) );
raw = [typecast(value,'uint8') raw];

%size
value = size(raw,2) +config.sizeSize +1;
if config.sizeSize==2
	value = uint16( value .*ones(msgNb,1) );
	raw = [typecast(value,'uint8') raw];
elseif config.sizeSize==4
	value = uint32( value .*ones(msgNb,1) );
	raw = [typecast(value,'uint8') raw];
else
	error
end

%fanion
value = config.fanion;
value = uint8( value .*ones(msgNb,1) );
raw = [typecast(value,'uint8') raw];


raw = reshape(raw',1,[]);

end %protocolEncode



