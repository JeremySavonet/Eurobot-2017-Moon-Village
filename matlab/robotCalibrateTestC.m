clear all
close all
clc

filename = 'test3';

load(filename)

for i = 1:length(data)
	telem = [data(i).telem.r data(i).telem.theta];
	file = fopen(['build-algoRecallage-Desktop_Qt_5_8_0_MinGW_32bit-Debug/' filename '.' num2str(i-1) '.raw'],'w');
	fwrite(file,double(telem),'double');
	fclose(file);
end
