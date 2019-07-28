% Schlau-Cohen, G. S., Calhoun, T. R., Ginsberg, N. S., Read, E. L., Ballottari, M., Bassi, R., … Fleming, G. R. (2009). Pathways of energy flow in LHCII from two-dimensional electronic spectroscopy. Journal of Physical Chemistry B, 113(46), 15352–15363. http://doi.org/10.1021/jp9066586
% Wu, J., Tang, Z., Gong, Z., Cao, J., & Mukamel, S. (2015). Minimal Model of Quantum Kinetic Clusters for the Energy-Transfer Network of a Light-Harvesting Protein Complex. The Journal of Physical Chemistry Letters, 1240–1245. http://doi.org/10.1021/acs.jpclett.5b00227


clear;
% time (ps)
t_i = 0;
t_f = 60;
t_step = (t_f-t_i)*1000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% rate matrix
% LHCII monomer
R = dlmread('data/LHCII_MON.K');
% change unit from fs^-1 to ps^-1
R = R*1000;
R(R<0)=0;
% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end

% eigenvector
H = load('data/LHCII_MON.H');
[Eigenv,EVal] = eig(H);
eigen_energy=diag(EVal);

%% Cranck-Nicolson

% initial state
size_t = size(R,1);
P = zeros(size_t,t_step);

% P(size_t,1) = 1; % exiton 14
% P(13,1) = 1; % site 6
P(:,1) = 1/14;

for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sumabs(P(:,i));
end

% % total pop plot
% figure
% hold
% for i = 1:size(R,1);
%     plot(t,P(i,:),'linewidth',2);
% end
% 
% legend('1','2','3','4','5','6','7','8','9','10','11','12','13','14');
% title('Propogation - exiton');
% 
% figure
% hold
% 
% P_site = Eigenv.^2*P;
% 
% for j = 1:size(R,1)
%     plot(t,P_site(j,:),'linewidth',2);
% end
% 
% legend('1','2','3','4','5','6','7','8','9','10','11','12','13','14');
% title('Propogation - site');



% LHCII monomer 4 cluster
c_1 = [14];
c_2 = [6,9:13];
c_3 = [3];
c_4 = [7];
c_5 = [4];
c_6 = [5,2,1];
% c_7 = 14;
% c_8 = 10;
% c_9 = 7;
% c_10 = 12;
% c_11 = 13;
% 
% 
C = {c_1 c_2 c_3 c_4 c_5 c_6};
% 
% reconstruct rate matrix
R_F = zeros(length(C),length(C));
for i = 1:length(C)
    for j = 1:length(C)
        R_F(i,j) = fun_build_cluster_k_adv(C{i},C{j},R,eigen_energy);  
    end
end
% 
% cluster exact pop
figure
hold

P_C = zeros(length(C),length(t));
for i = 1:length(C)
    for j = C{i}
        P_C(i,:) = P_C(i,:) + P(j,:);
    end
end



for i = 1:length(C)
    plot(t,P_C(i,:),'-','LineWidth',2);
end
% cluster Ford Fulkerson pop

P_F = zeros(length(C),1);
P_F(:,1) = P_C(:,1);
for i = 1:t_step-1
    P_F(:,i+1) = (eye(size(R_F,1)) + R_F*dt)*P_F(:,i)/sumabs(P_F(:,i));
end

for i = 1:length(C)
    plot(t,P_F(i,:),'--','LineWidth',2);
end
title('Propogation - exact and reconstruct - LHCII monomer');
xlabel('ps');
legend('C1','C2','C3','C4','C5','C6','C1-reconstruct','C2-reconstruct','C3-reconstruct','C4-reconstruct','C5-reconstruct','C6-reconstruct');

% LHCII monomer 7 cluster
c_1 = [14];
c_2 = [8:13];
c_3 = [6];
c_4 = [3];
c_5 = [7];
c_6 = [4];
c_7 = [5,2,1];
% c_8 = 10;
% c_9 = 7;
% c_10 = 12;
% c_11 = 13;
% 
% 
C = {c_1 c_2 c_3 c_4 c_5 c_6 c_7};
% 
% reconstruct rate matrix
R_F = zeros(length(C),length(C));
for i = 1:length(C)
    for j = 1:length(C)
        R_F(i,j) = fun_build_cluster_k_adv(C{i},C{j},R,eigen_energy);  
    end
end
% 
% cluster exact pop
figure
hold

P_C = zeros(length(C),length(t));
for i = 1:length(C)
    for j = C{i}
        P_C(i,:) = P_C(i,:) + P(j,:);
    end
end



for i = 1:length(C)
    plot(t,P_C(i,:),'-','LineWidth',2);
end
% cluster Ford Fulkerson pop

P_F = zeros(length(C),1);
P_F(:,1) = P_C(:,1);
for i = 1:t_step-1
    P_F(:,i+1) = (eye(size(R_F,1)) + R_F*dt)*P_F(:,i)/sumabs(P_F(:,i));
end

for i = 1:length(C)
    plot(t,P_F(i,:),'--','LineWidth',2);
end
title('Propogation - exact and reconstruct - LHCII monomer');
xlabel('ps');
legend('C1','C2','C3','C4','C5','C6','C7','C1-reconstruct','C2-reconstruct','C3-reconstruct','C4-reconstruct','C5-reconstruct','C6-reconstruct','C7-reconstruct');

%% LHCII monomer modified 5 cluster
c_1 = [14];
c_2 = [4,7];
c_3 = [1,2,5,6,8:13];
c_4 = [3];
% c_5 = [3 6 9];
% c_6 = [6];
% c_7 = [9];
% c_8 = [10];
% c_9 = [11 12];
% c_10 = [8];
% c_11 = 13;
% 
% 
C = {c_1 c_2 c_3 c_4};
% 
% reconstruct rate matrix
R_F = zeros(length(C),length(C));
for i = 1:length(C)
    for j = 1:length(C)
        R_F(i,j) = fun_build_cluster_k_adv(C{i},C{j},R,eigen_energy);  
    end
end
% 
% cluster exact pop
figure
hold

P_C = zeros(length(C),length(t));
for i = 1:length(C)
    for j = C{i}
        P_C(i,:) = P_C(i,:) + P(j,:);
    end
end



for i = 1:length(C)
    plot(t,P_C(i,:),'-','LineWidth',2);
end
% cluster Ford Fulkerson pop

P_F = zeros(length(C),1);
P_F(:,1) = P_C(:,1);
for i = 1:t_step-1
    P_F(:,i+1) = (eye(size(R_F,1)) + R_F*dt)*P_F(:,i)/sumabs(P_F(:,i));
end

for i = 1:length(C)
    plot(t,P_F(i,:),'--','LineWidth',2);
end
title('Propogation - exact and reconstruct - LHCII monomer');
xlabel('ps');
% legend('C1','C2','C3','C4','C5','C6','C7','C1-reconstruct','C2-reconstruct','C3-reconstruct','C4-reconstruct','C5-reconstruct','C6-reconstruct','C7-reconstruct');


% 
% % compute the cost
% cut_off = 0.1 ;% ps-1
% cut_off_time = 1/cut_off; % ps
% m = length(t);
% n = size(P_C,1);
% Diff = P_C(:,m:10*m)-P_F(:,m:10*m);
% Diff = P_C(:,1:m) - P_F(:,1:m);
% Diff = P_C - P_F;
% J = 1/2/m/n*sum(sum(Diff.^2));



