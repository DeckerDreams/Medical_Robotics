clc
startup_rvc


%% measurements in mm
a1=96; % distance from floor to shoulder 
a2z=92; % distance from shoulder to elbow
a2x=32; % distance from shoulder to outside elbow
a3=90; % distance from elbow to wrist
a4=100; % distance from wrist to gripper‐tip
a4pen= 0; % 92; % distance from gripper‐tip to pen‐tip



%%     "DH" configuration 


q1 = 0; %rotate base
q2 = 0; % rotate shoulder
q3 = 0; % rotate elbow
q4 = 0; %rotate wrist
%note radians (angle /180 * pi)


%dh at origin
% [ Rz(theta), Tz(d), Tx(a), Rx(alpha)]


dh = [0 a1 0 -pi/2; % 90 rotate about the x axis (joint Rz)
      q2 0 0 pi/2; %orient z-axis link perpendicular to ground 
      0 a2z a2x -pi/2; %align again so (joint Rz)
      0 0 a3 0; %distance to next joint 
      0 0 a4 pi/2; 
      0 -a4pen 0 0];



r=SerialLink(dh);
r.fkine([q1 q2 0 q3 q4 0])
Q = double(r.fkine([q1 q2 0 q3 q4 0]));
angles= (r.ikine(Q))*180/pi; %inverse kinematics





% testing angles 
P = [130,-122,0];
tr = transl(P);

pose = tr; 
a = (r.ikine(pose))*180/pi; %inverse kinematics




%% Create trajectory of trace line path

P1 = [130,-125, 0];
P2 = [222, -125, 0];
P3 = [222, 122, 0];
P4 = [130, 122, 0];


fwd = ctraj(transl(P1), transl(P2), 50);
f = (r.ikine(fwd))*180/pi; %inverse kinematics
a_fwd = round([f(:,1:2),-(f(:,4:5))]*10);

lft = ctraj(transl(P2), transl(P3), 100);
l = (r.ikine(lft))*180/pi; %inverse kinematics
a_l = round([l(:,1:2),-(l(:,4:5))]*10);


dwn = ctraj(transl(P3), transl(P4), 50);
d = (r.ikine(dwn))*180/pi; %inverse kinematics
a_dwn = round([d(:,1:2),-(d(:,4:5))]*10);








%% Create Trajectory of circle


% % create a circle.
% points = 90;
% 
% angles = linspace(0, 2*pi, points); %total number of points
% radius = 50;
% xCenter = 200;
% yCenter = 0;
% x = radius * cos(angles) + xCenter; 
% y = radius * sin(angles) + yCenter;
% 
% % Plot circle.
% figure(1)
% plot(x, y, 'b-', 'LineWidth', 2);
% % Plot center.
% hold on;
% plot(xCenter, yCenter, 'k+', 'LineWidth', 2, 'MarkerSize', 16);
% grid on;
% axis equal;
% xlabel('X', 'FontSize', 16);
% ylabel('Y', 'FontSize', 16);
% 
% 
% % Now get locations along the circle.
% z = 0;
% ps =[];
% q_values = [];
% for i = 1:points
%     ps = [ps; [x(i), y(i), z]];
%     P = ps(i,:);
%     pose = transl(P);
%     q_values = [q_values; (r.ikine(pose))*180/pi];
% end
% 
% arduio = ([q_values(:,1:2),-(q_values(:,4:5))]*10);
% a = round(arduio);
% 
% 
%% cross lines through circle
% 
% p_top = [radius + xCenter, 0, 0];
% p_bottom = [xCenter - radius, 0 , 0];
% 
% p_left = [xCenter, yCenter - radius, 0];
% p_right = [xCenter, radius + yCenter, 0];

%%interpolate poses
%%top to bottom

% T0 = transl(p_top);
% T1 = transl(p_bottom);
% 
% vert = ctraj(T0,T1,75);
% v = (r.ikine(vert))*180/pi; %inverse kinematics
% a_vert = round([v(:,1:2),-(v(:,4:5))]*10);
% 
%%horizontal
% Tl = transl(p_left);
% Tr = transl(p_right);
% 
% lr = ctraj(Tl, Tr, 75);
% h = (r.ikine(lr))*180/pi;
% a_h = round([h(:,1:2),-(h(:,4:5))]*10);
% 

