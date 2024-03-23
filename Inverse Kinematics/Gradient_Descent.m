% RBE 501 - Robot Dynamics - Spring 2024
% Swati Shirke
% Worcester Polytechnic Institute
% Last modified: 02/09/2024

clear, clc, close all
addpath('utils');

plotOn = true;
nTests = 30; % number of random test configurations
n      = 6;  % degrees of freedom

%% Create the manipulator
% Link length values (meters)
H1 = 0.320;
H2 = 0.225;
H3 = 0.225;
H4 = 0.065;
W  = 0.035;

robot = SerialLink([Revolute('d', H1, 'a', 0,  'alpha', -pi/2, 'offset', 0), ...
    Revolute('d', 0,  'a', H2, 'alpha', 0,     'offset', -pi/2), ...
    Revolute('d', W,  'a', 0,  'alpha', pi/2,  'offset', pi/2), ...
    Revolute('d', H3, 'a', 0,  'alpha', -pi/2, 'offset', 0), ...
    Revolute('d', 0,  'a', 0,  'alpha', pi/2,  'offset', 0), ...
    Revolute('d', H4, 'a', 0,  'alpha', 0,     'offset', 0)], ...
    'name', 'Staubli TX-40');

% Joint limits
qlim = [-180  180;  % q(1)
    -125  125;  % q(2)
    -138  138;  % q(3)
    -270  270;  % q(4)
    -120  133.5;% q(5)
    -270  270]; % q(6)

% Display the manipulator in the home configuration
q = zeros(1,n);
robot.teach(q);
hold on;

%% Part A - Calculate the screw axes
% Let us calculate the screw axis for each joint
% Put all the axes into a 6xn matrix S, where n is the number of joints

S = [0 0 1 0 0 0;
     0 1 0 -0.32 0 0;
     0 1 0 -0.545 0 0;
     0 0 1 0.035 0 0;
     0 1 0 -0.770 0 0;
     0 0 1 0.035 0 0;
     ]';


%% Part B - Calculate the forward kinematics with the Product of Exponentials formula
% First, let us calculate the homogeneous transformation matrix M for the
% home configuration

M = [1 0 0 0;0 1 0 0.035;0 0 1 0.835; 0 0 0 1];

fprintf('---------------------Forward Kinematics Test---------------------\n');
fprintf(['Testing ' num2str(nTests) ' random configurations.\n']);
fprintf('Progress: ');
nbytes = fprintf('0%%'); 
 
% Test the forward kinematics for 100 random sets of joint variables
for ii = 1 : nTests
    fprintf(repmat('\b',1,nbytes));
    nbytes = fprintf('%0.f%%', ceil(ii/nTests*100));
    
    % Generate a random configuration
    q = [qlim(1,1) + (qlim(1,2) - qlim(1,1)) * rand(), ...
         qlim(2,1) + (qlim(2,2) - qlim(2,1)) * rand(), ...
         qlim(3,1) + (qlim(3,2) - qlim(3,1)) * rand(), ...
         qlim(4,1) + (qlim(4,2) - qlim(4,1)) * rand(), ...
         qlim(5,1) + (qlim(5,2) - qlim(5,1)) * rand(), ...
         qlim(6,1) + (qlim(6,2) - qlim(6,1)) * rand()];
    
    % Calculate the forward kinematics
    T = fkine(S, M, q);
    
    if plotOn
        robot.teach(q);
        title('Forward Kinematics Test');
    end
    
    assert(all(all(abs(double(robot.fkine(q)) - T) < 1e-10)));
end
 
fprintf('\nTest passed successfully.\n');


%% Part C - Calculate the Space Jacobian of the manipulator
fprintf('-------------------Differential Kinematics Test------------------\n');
fprintf(['Testing ' num2str(nTests) ' random configurations.\n']);
fprintf('Progress: ');
nbytes = fprintf('0%%'); 

% Test the correctness of the Jacobian for 100 random sets of joint
% variables
for ii = 1 : nTests
    fprintf(repmat('\b',1,nbytes));
    nbytes = fprintf('%0.f%%', ceil(ii/nTests*100));
    
    % Generate a random configuration
    q = [qlim(1,1) + (qlim(1,2) - qlim(1,1)) * rand(), ...
         qlim(2,1) + (qlim(2,2) - qlim(2,1)) * rand(), ...
         qlim(3,1) + (qlim(3,2) - qlim(3,1)) * rand(), ...
         qlim(4,1) + (qlim(4,2) - qlim(4,1)) * rand(), ...
         qlim(5,1) + (qlim(5,2) - qlim(5,1)) * rand(), ...
         qlim(6,1) + (qlim(6,2) - qlim(6,1)) * rand()];
    
    % Calculate the Forward Kinematics
    T = fkine(S,M,q);
    
    % Calculate the Jacobian
    J = jacob0(S, q);
    
    if plotOn
        robot.teach(q);
        title('Differential Kinematics Test');
    end
    
    % Test the correctness of the Jacobian
    Jcoords = [-skew(T(1:3,4))*J(1:3,:)+J(4:6,:); J(1:3,:)];
    assert(all(all(abs(double(robot.jacob0(q)) - Jcoords) < 1e-10)));
end

fprintf('\nTest passed successfully.\n');


%% Part D - Inverse Kinematics
fprintf('----------------------Inverse Kinematics Test--------------------\n');
fprintf(['Testing ' num2str(nTests) ' random configurations.\n']);
fprintf('Progress: ');
nbytes = fprintf('0%%');

% Calculate the twist representing the robot's home pose
currentPose = MatrixLog6(M);
currentPose = [currentPose(3,2) currentPose(1,3) currentPose(2,1) currentPose(1:3,4)']';

% Set the current joint variables
currentQ = zeros(1,6);

if plotOn
    robot.teach(currentQ);
    h = triad('matrix', M, 'tag', 'Target Pose', 'linewidth', 2.5, 'scale', 0.5);
end
     
% Generate the test configurations
q = [linspace(0,qlim(1,2),nTests);
     linspace(0,qlim(2,2),nTests);
     linspace(0,qlim(3,2),nTests);
     linspace(0,qlim(4,2),nTests);
     linspace(0,qlim(5,2),nTests);
     linspace(0,qlim(6,2),nTests)];

for ii = 1 : nTests
    fprintf(repmat('\b',1,nbytes));
    nbytes = fprintf('%0.f%%', ceil(ii/nTests*100));
    
    % Generate the robot's pose
    T = fkine(S,M,q(:,ii)');
    targetPose = MatrixLog6(T);
    targetPose = [targetPose(3,2) targetPose(1,3) targetPose(2,1) targetPose(1:3,4)']';
    
    if plotOn
        set(h, 'matrix', T);
        title('Inverse Kinematics Test');
        drawnow;
    end
    
    % Inverse Kinematics
   
    lambda = 5;
    while norm(targetPose - currentPose) > 1e-2
        
        J = jacob0(S, currentQ);
        %deltaQ = J' * pinv(J*J' + lambda^2 * (eye(6))) * (targetPose - currentPose);
        deltaQ = J' * (targetPose - currentPose);
        currentQ = currentQ + deltaQ';
        
        T = fkine(S,M,currentQ);
        currentPose = MatrixLog6(T);
        currentPose = [currentPose(3,2) ...
                       currentPose(1,3) ...
                       currentPose(2,1) ...
                       currentPose(1:3,4)']';
        
       

        if plotOn
            try
                robot.teach(currentQ);
                drawnow;
            catch e
                continue;
            end
        end
    end
end

fprintf('\nTest passed successfully.\n');



