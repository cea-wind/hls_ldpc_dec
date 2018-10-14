function [ iter,decoderData ] = ldpcMsVec(H,M,receiveLLR,MAX_ITER_NUM)
%%decoder algorithm
[m,n] = size(H);
[m0,n0] = size(M);
BL = m/m0;
BN = sum(sum(M>=0));
col_idx = cell(1,24);
row_idx = cell(1,8);
row_offset = cell(1,8);
Midx = M;
cnt = 1;
for i=1:n0;
    for j=1:m0
        if(Midx(j,i)>=0)
            Midx(j,i)=cnt;
            cnt = cnt+1;
        end
    end
end
for i=1:n0
    midxi = Midx(:,i);
    col_idx{i}=midxi(midxi>0);
end
for i=1:m0
    midxi = Midx(i,:);
    mi = M(i,:);
    row_offset{i} = mi(midxi>0);
    row_idx{i}=midxi(midxi>0);
end
decoderData = zeros(size(receiveLLR));
qn0_1 = decoderData;
minfo = zeros(BN,BL);
for i=1:24
    for j = 1:size(col_idx{i})
        minfo(col_idx{i}(j),:) = receiveLLR(i*BL-BL+1:i*BL);
    end
end
minfo2 = minfo;

for iter=1:MAX_ITER_NUM
    %check nodes information process
    for i = 1:8
        um = zeros(BL,length(row_idx{i}));
        for j=1:length(row_idx{i})
            um(:,j) = circshift(minfo(row_idx{i}(j),:)',-row_offset{i}(j));
        end
        vm = rowUpdateMSVec(um);
        for j=1:length(row_idx{i})
            minfo2(row_idx{i}(j),:) = circshift(vm(:,j),row_offset{i}(j));
        end        
    end
    %variable nodes information process
    for i = 1:24
        vm = zeros(BL,length(col_idx{i}));
        for j=1:length(col_idx{i})
            vm(:,j) = minfo2(col_idx{i}(j),:);
        end
        vm = vm';
        [um,qn0_1(i*BL-BL+1:i*BL)] = colUpdateVec(vm,receiveLLR(i*BL-BL+1:i*BL));
        for j=1:length(col_idx{i})
            minfo(col_idx{i}(j),:) =  um(j,:);
        end
    end    
    % decision decoding
    decoderData(qn0_1>=0) = 0;
    decoderData(qn0_1<0) = 1;
%     if(mod(H*decoderData',2)==0)
%         break;
%     end
end


