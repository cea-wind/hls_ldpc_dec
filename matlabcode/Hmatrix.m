%% 
load MotherMatrix_2_3_A.txt;
M = MotherMatrix_2_3_A;
z = 64;
% z = 24:4:96; for different code length 
H = zeros(z*size(M,1),z*size(M,2));
for m = 1:size(M,1)
    for n = 1:size(M,2)
        if(M(m,n)>=0)
            P = zeros(z);
            for k = 1:z
                P(k,mod( M(m,n)+k-1,z)+1) = 1;
            end
            H((m-1)*z+1:m*z,(n-1)*z+1:n*z) =P;
        end
    end
end
H = sparse(H);
spy(H);