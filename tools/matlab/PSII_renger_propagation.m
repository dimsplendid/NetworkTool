
clear;
% time (ps)
t_i = 0;
t_f = 200;
t_step = 100000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% rate matrix
% LHCII monomer
% R = dlmread('PSII_core_cal_rate.dat');
R = dlmread('Rate_renger.dat');
% change unit from ns^-1 to ps^-1
R = R*1e-3;

% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end


%% Cranck-Nicoson

% initial state
size_t = size(R,1);
P = zeros(size_t,1);

% Init: average
for i=1:size_t
    P(i,1) = 1/size_t;
end

% Init: Highest energy state
% P(size_t,1)=1;

for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sumabs(P(:,i));
end


% total pop plot
figure;
hold on;

for i = 1:size(R,1);
    plot(t,P(i,:));
end
xlabel('ps');
ylabel('propagation');
title('Propogation - Renger cluster dynamics');

