
clear;
% time (ps)
t_i = 0;
t_f = 20;
t_step = 100000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% rate matrix
% LHCII monomer
% R = dlmread('PSII_core_cal_rate.dat');
R = dlmread('PSII_M1_md_20000_rate.dat');
% change unit from fs^-1 to ps^-1
R = R*1000;

% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
plot_coupling(R);
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end


%% Cranck-Nicoson

% initial state
size_t = size(R,1);
P = zeros(size_t,1);

% Init: average
P(:,1) = 1/size_t;

% Init: Highest energy state
% P(size_t,1)=1;

% Markovian process
O = eye(size_t) + R*dt;
for i = 1:1:t_step-1
    P(:,i+1) = O*P(:,i)/sumabs(O*P(:,i));
end

% total pop plot
figure;
hold on;

plot(t,P(1,:),'r-','LineWidth',2);
% plot(t,P(35,:),'b-','LineWidth',2);
% plot(t,P(18,:),'g-','LineWidth',2);
% legend('1','35','18');
for i = 1:size(R,1);
    plot(t,P(i,:));
end
xlabel('ps');
ylabel('propagation');
title('Propogation - 10000');

