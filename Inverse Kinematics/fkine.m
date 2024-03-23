function T = fkine(S,M,q)
    dim = size(q);
    T = eye(4);
    %PoE formulae for Transofrmation calculations
    for i=1:dim(2)
        T = T*twist2ht(S(:,i),q(i));
    end
    T = T * M;
end