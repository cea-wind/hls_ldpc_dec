function [ y,p ] = colUpdateVec( x,vl )
%ldpc col update
%   input  £ºM*N x update col
%   output : result y
[m,n]=size(x);
p = sum(x) + vl;
pmat = repmat(p,m,1);
y = pmat - x;
end

