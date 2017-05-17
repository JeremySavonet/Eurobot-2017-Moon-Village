function [msg,raw] = protocolParse( raw)
% Parse raw data to messages using IOCENE protocol
%
% Sntaxe:
%     msg = protocolParse( raw)
%     [msg,raw] = protocolParse( raw)
% Input:
%     raw             raw data
% Output:
%     msg             parsed messages
%         .id             identifiant de message
%         .len            longueur du message
%         .data           header + payload
%     raw             unused raw data


config = hardwareGetConfig();
config = config.protocol;

if size(raw,2)~=1
	raw = raw';
end
raw = uint8(raw);

i = find( raw(1:end-config.sizeSize-2-2)==config.fanion );

i = i(i+1+config.sizeSize+2+2<length(raw));

if config.sizeSize==4
    len = double(typecast(reshape(raw(i*ones(1,config.sizeSize)+ones(size(i))*(1:config.sizeSize))',[],1),'uint32'));
else
    len = double(typecast(reshape(raw(i*ones(1,config.sizeSize)+ones(size(i))*(1:config.sizeSize))',[],1),'uint16'));
end
j = find(i+len-1<=length(raw));
i = i(j);
len = len(j);
j = find(len<config.sizeMax);
i = i(j);
len = len(j);

j = cumsum(uint64(raw));
j = find( mod(j(i+len-1)-j(i+config.sizeSize+2),65536) ...
	== uint64(typecast(reshape(raw(i*ones(1,2)+ones(size(i))*(config.sizeSize+(1:2)))',[],1),'uint16')) );
i = i(j);
len = len(j);

if length(i)>4
	j = find( [1; i(2:end)>=i(1:end-1)+len(1:end-1)] | [0; 0; i(3:end)==i(1:end-2)+len(1:end-2)] | [0; 0; 0; i(4:end)==i(1:end-3)+len(1:end-3)] );
	i = i(j);
	len = len(j);
elseif length(i)>3
	j = find( [1; i(2:end)>=i(1:end-1)+len(1:end-1)] | [0; 0; i(3:end)==i(1:end-2)+len(1:end-2)] );
	i = i(j);
	len = len(j);
elseif length(i)>2
	j = find( [1; i(2:end)>=i(1:end-1)+len(1:end-1)] );
	i = i(j);
	len = len(j);
end


id = typecast(reshape(raw(i*ones(1,2)+ones(size(i))*(config.sizeSize+2+(1:2)))',[],1),'uint16');

data = cell(length(id),1);

if ~isempty(i)
	for l = unique(len)'
		j = find(len==l);
		if length(j)==1
			data(j) = {raw( ones(size(i(j)))*(0:len(j(1))-1) + i(j)*ones(1,len(j(1))) )'};
		else
			data(j) = mat2cell(raw( ones(size(i(j)))*(0:len(j(1))-1) + i(j)*ones(1,len(j(1))) ),ones(length(j),1));
		end
	end
	raw = raw( i(end)+len(end) : end );
end

msg = table(id,len,data);

end %msgParse



