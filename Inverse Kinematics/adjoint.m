function Vtrans = adjoint(V,T)
    % your code here
    R = T(1:3, 1:3);
    p = T(1:3,4);
    p_mat = [ 0, -p(3), p(2);
                p(3), 0, -p(1);
               -p(2), p(1), 0 ];
    
    adjoint = [R, zeros(3);
                p_mat*R , R ];
    Vtrans = adjoint * V;
    
    
end
