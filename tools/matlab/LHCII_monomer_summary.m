% LHCII monomer analysis
%% Deloalized_length
H = load('data/LHCII_H_wu.dat');
[Eigenv,eigen_energy] = eig(H);
eigen_energy = diag(eigen_energy);
state = 1:14;
DelocalizedLength = zeros(14,1);
for i = state
    DelocalizedLength(i) = delocalized_length(i,Eigenv);
end

%% propagation
% rate matrix
% LHCII monomer
R = dlmread('data/LHCII_monomer_wu_20161202_rate.dat');
% change unit from fs^-1 to ps^-1
R = R*1000;

% time (ps)
t_i = 0;
t_f = 20;
t_step = (t_f-t_i)*1000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end

% initial state
size_t = size(R,1);
P = zeros(size_t,t_step);
P(:,1) = 1/size_t;

for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sum(abs(P(:,i)));
end

P_site = P*0;

% total pop plot
figure
hold on;
for i = 1:size(R,1);
    plot(t,P(i,:),'LineWidth',2);
end

legend('1','2','3','4','5','6','7','8','9','10','11','12','13','14');
title('LHCII monomer propagation');
xlabel('ps');
ylabel('polulation');
hold off;
% 

%% cut-off vs # of cluster
% normalized tree
data=[0.0109
    0.0742
    0.0984
    0.1277
    0.6268
    0.7820
    0.8363
    1.6416
    1.7590
    2.1110
    4.5950
   13.7220
   38.9310];
% rate > cut-off --> cluster

% rate > 0.011
G2 = {[1 2 3 5 6 8 9 10 11 12 13 14] [7 4]};
% rate > 0.074
G3 = {[3 6 8 9 10 11 12 13 14] [5 2 1] [7 4]};
% rate > 0.098
G4 = {[3 6 8 9 10 11 12 13] [14] [5 2 1] [7 4]};
% rate > 0.128
G5 = {[6 8 9 10 11 12 13] [3] [14] [5 2 1] [7 4]};
% rate > 0.627
G6 = {[6 8 10 11 12 13] [9] [3] [14] [5 2 1] [7 4]};
% rate > 0.782
G7 = {[8 10 11 12 13] [6] [9] [3] [14] [5 2 1] [7 4]};
% rate > 0.836
G8 = {[8 11 12 13] [10] [6] [9] [3] [14] [5 2 1] [7 4]};
% rate > 1.642
G9 = {[11 12 13] [8] [10] [6] [9] [3] [14] [5 2 1] [7 4]};
% rate > 1.759
G10 = {[11 12 13] [8] [10] [6] [9] [3] [14] [5 2 1] [7] [4]};
% rate > 2.111
G11 = {[11 12] [13] [8] [10] [6] [9] [3] [14] [5 2 1] [7] [4]};
% rate > 4.595
G12 = {[11] [12] [13] [8] [10] [6] [9] [3] [14] [5 2 1] [7] [4]};
% rate > 13.722
G13 = {[11] [12] [13] [8] [10] [6] [9] [3] [14] [5] [2 1] [7] [4]};
% rate = 38.931
G14 = {[1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14]};

G = {G2 G3 G4 G5 G6 G7 G8 G9 G10 G11 G12 G13 G14};
cost1 = zeros(length(G),1);
R_F={};
for i=1:length(G)
    [cost1(i),R_F{i}] = plot_LHCII_cluster(G{i},H,R);
%     close all;
end

%% cut-off # of clusters
% origin data

data=[38.9310
   27.4440
    8.2080
    4.6920
    4.5950
    4.2220
    2.5090
    2.5070
    2.2260
    1.7590
    1.2790
    1.1490
    0.1530];
% rate > cut-off --> cluster

% rate > 0.153
G2 = {[1 2 3 5 6 8 9 10 11 12 13 14] [7 4]};
% rate > 1.149
G3 = {[1 2 5 6 8 9 10 11 12 13 14] [3] [7 4]};
% rate > 1.279
G4 = {[1 2 5 6 8 9 10 11 12 13] [14] [3] [7 4]};
% rate > 1.759
G5 = {[1 2 5 6 8 9 10 11 12 13] [14] [3] [7] [4]};
% rate > 2.226
G6 = {[6 8 9 10 11 12 13] [5 2 1] [14] [3] [7] [4]};
% rate > 2.507
G7 = {[6 8 10 11 12 13] [9] [5 2 1] [14] [3] [7] [4]};
% rate > 2.509
G8 = {[6 8 11 12 13] [10] [9] [5 2 1] [14] [3] [7] [4]};
% rate > 4.222
G9 = {[6 8 13] [11 12] [10] [9] [5 2 1] [14] [3] [7] [4]};
% rate > 4.595
G10 = {[6 8 13] [11] [12] [10] [9] [5 2 1] [14] [3] [7] [4]};
% rate > 4.692
G11 = {[8 13] [6] [11] [12] [10] [9] [5 2 1] [14] [3] [7] [4]};
% rate > 8.208
G12 = {[13] [8] [6] [11] [12] [10] [9] [5 2 1] [14] [3] [7] [4]};
% rate > 27.444
G13 = {[13] [8] [6] [11] [12] [10] [9] [5] [2 1] [14] [3] [7] [4]};
% rate = 38.931
G14 = {[1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14]};

G = {G2 G3 G4 G5 G6 G7 G8 G9 G10 G11 G12 G13 G14};
cost2 = zeros(length(G),1);
R_F={};
for i=1:length(G)
    [cost2(i),R_F{i}] = plot_LHCII_cluster(G{i},H,R);
    close all;
end
figure;hold on;
plot(2:14,cost1,'-o','LineWidth',2);
plot(2:14,cost2,'-o','LineWidth',2);
legend('normalize tree','original tree');
hold off;

