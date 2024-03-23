function T = twist2ht(S,theta)
    % your code here
    v = S(4:6);
    omega = S(1:3);
    % If needed, you can calculate a rotation matrix with:
    R = axisangle2rot(omega,theta);
    omega_mat = [ 0,  -omega(3), omega(2);
                omega(3), 0, -omega(1);
                -omega(2), omega(1), 0 ];
    
    T = [R , ( eye(3) * theta + (1-cos(theta))* omega_mat + (theta - sin(theta)) * omega_mat^2)* v;
        0, 0,0,  1];
end
