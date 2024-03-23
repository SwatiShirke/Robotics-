function R = axisangle2rot(omega,theta)
    % your code here
    
    omega_mat = skew_mat(omega);
    R = eye(3,3) + sin(theta) * omega_mat + (1 - cos(theta)) * omega_mat^2;      
end

function omega_mat = skew_mat(omega)
omega_mat = [ 0,  -omega(3), omega(2);
                omega(3), 0, -omega(1);
                -omega(2), omega(1), 0 ];

end

