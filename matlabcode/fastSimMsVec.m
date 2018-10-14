clc;clear
Hmatrix

rng(0);
MaximumIterationCount = 10;
enc = comm.LDPCEncoder(sparse(H));                  % define encoder 
FILE_NAME = ['LDPC_MS_' datestr(now,'yyyymmdd') '.txt'];
fid = fopen(FILE_NAME,'at+');
fprintf(fid,'Information Length = %d, ',size(H,2)-size(H,1));
fprintf(fid,'Code Length = %d, MaxIter =%d\n',size(H,2),MaximumIterationCount);
fprintf(fid,'EbN0(dB)\t totalFrame\t');
fprintf(fid,'FER\t BER\n'); 
% AWGN channel
chan = comm.AWGNChannel('NoiseMethod','Signal to noise ratio (SNR)');
err = comm.ErrorRate;

%% parameter initialize 
RATE = (size(H,2)-size(H,1))/size(H,2);     % code rate
msgLen = size(H,2)-size(H,1);                 % information length
ebn0Vec = 1:0.5:3;                                         % EbN0
snrVec = ebn0Vec + 10*log10(RATE) + 10*log10(2);    % EbN0  to SNR
ber = zeros(length(snrVec),1);                 % bit error
fer = zeros(length(snrVec),1);                  % frame error
MaxErrFrame = 100;                              % stop condition
MaxFrameNum = 10^6;                        % stop condition

%% Loop for SNR
for k = 1:length(snrVec)
    % var of gaussian noise
    noiseVar = 1/10^(snrVec(k)/10);
    errorStats = zeros(1,3);
    totalFrame = 0;
    while fer(k) <=  MaxErrFrame&& totalFrame < MaxFrameNum
        totalFrame = totalFrame + 1;
        % information
        data = logical(randi([0 1],msgLen,1));
        % encoder 
        encData = step(enc,data);              
        % BPSK modulation 
        modSig = 1-2*encData;            
        
        % AWGN Channel
        rxSig = modSig + sqrt(noiseVar)*randn(size(modSig));   
        
        % BPSK demoudulation (llr)
        demodSig = rxSig'; 
        % LDPC soft information Decoder
        [ ~,rxData ] = ldpcMsVec(H,M,demodSig,MaximumIterationCount);
        errorStats = step(err,data,logical(rxData(1:length(data))'));    % Compute error stats
        fer(k) = fer(k)+(sum(abs(rxData(1:length(data))'-data))>0);
		
		%display
		if(mod(totalFrame,100)==0)
			fprintf('----------------------------\n')
			fprintf('EbN0=%f    totalFrame=%d    FER=%f    BER=%f\n',...
			ebn0Vec(k),totalFrame,fer(k)/totalFrame,errorStats(1));
		end
    end
    fer(k) = fer(k)/totalFrame;
    % Save the BER for the current Eb/No and reset the error rate counter
    ber(k) = errorStats(1);
	fprintf(fid,'%3.2g\t %5d\t',ebn0Vec(k),totalFrame);
    fprintf(fid,'%e\t %e\n',fer(k),ber(k));   
    reset(err)
end
fclose(fid);
figure;
semilogy(ebn0Vec,ber,'r-o','linewidth',2)
hold on
semilogy(ebn0Vec,fer,'r--+','linewidth',2)
grid
xlabel('SNR (dB)')
ylabel('Bit Error Rate')

EbNo = 2:11;
berBpsk = berawgn(EbNo,'psk',2,'nodiff');
semilogy(EbNo,berBpsk,'b--','linewidth',2)
legend('BER','FER','uncoded BPSK')