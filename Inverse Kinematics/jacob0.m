function J = jacob0(S,q)
    % your code here    
    % If necessary, you calculate the homogeneous transformation associated with a twist V and displacement omega with:
    % T = twist2ht(V,omega);    
    dim = size(S);
    n = dim(2);
    T_matrix_list = cell(1, n);    
     
    J = S(:,1);
    T1 =  twist2ht(S(:,1),q(1));
    product = T1;
    for i = 2:n
        T_mat = twist2ht(S(:,i),q(i)) ;        
        J_i = adjoint(S(:,i), product);
        product = product * T_mat;
    J = [J J_i];          
    % You can also calculate the adjoint transformation of a twist V w.r.t. a homogeneous transformation matrix T with:
    % adjoint(V,T)   
    
end