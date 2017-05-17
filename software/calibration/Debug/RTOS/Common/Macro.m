

N = 250*3;


fprintf('#define MACRO_ARG_N( \\\n\t\t');
for i = 1:N
	fprintf('_%d, ', i);
	if mod(i,10)==0
		fprintf('\\\n\t\t');
	end
end
fprintf('N,...) N\n');

fprintf('#define MACRO_RSEQ_N() \\\n\t\t');
for i = N:-1:0
	fprintf('%d, ', i);
	if mod(i,10)==0
		fprintf('\\\n\t\t');
	end
end
fprintf('\n');

fprintf('#define MACRO_DEC(x) MACRO_CAT(MACRO_DEC_, x)\n');
fprintf('#define MACRO_DEC_0 0\n');
for i = 1:N
	fprintf('#define MACRO_DEC_%d %d \n', i, i-1);
end
fprintf('\n');



