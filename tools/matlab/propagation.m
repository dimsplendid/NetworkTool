% Schlau-Cohen, G. S., Calhoun, T. R., Ginsberg, N. S., Read, E. L., Ballottari, M., Bassi, R., … Fleming, G. R. (2009). Pathways of energy flow in LHCII from two-dimensional electronic spectroscopy. Journal of Physical Chemistry B, 113(46), 15352–15363. http://doi.org/10.1021/jp9066586
% Wu, J., Tang, Z., Gong, Z., Cao, J., & Mukamel, S. (2015). Minimal Model of Quantum Kinetic Clusters for the Energy-Transfer Network of a Light-Harvesting Protein Complex. The Journal of Physical Chemistry Letters, 1240–1245. http://doi.org/10.1021/acs.jpclett.5b00227


clear;
% time (ps)
t_i = 0;
t_f = 20;
t_step = (t_f-t_i)*1000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% rate matrix
% LHCII monomer
R = dlmread('data/LHCII_monomer_wu_20161202_rate.dat');
% change unit from fs^-1 to ps^-1
R = R*1000;

% eigenvector,value
H = load('data/LHCII_H_wu.dat');
[Eigenv,eigen_energy] = eig(H);
eigen_energy = diag(eigen_energy);

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

for i=1:size_t
    P(i,1) = 1/size_t;
end

for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sumabs(P(:,i));
end

P_site = P*0;

% % total pop plot
figure
hold on;
for i = 1:size(R,1);
    plot(t,P(i,:));
end

legend('1','2','3','4','5','6','7','8','9','10','11','12','13','14');
title('Propogation - exiton');
hold off;
% 

% C = {c_1 c_2 c_3 c_4};

% LHCII monomer
c_1 = [1,2,5];
c_2 = [3];
c_3 = [4,7];
c_4 = [6,11,12];
% c_4 = 4;
% % c_4 = [6,11,12];
c_5 = [8 9];
c_6 = [10,13];
c_7 = 14;
% c_8 = 10;
% c_9 = 7;
% c_10 = 12;
% c_11 = 13;

% 
C = {c_1 c_2 c_3 c_4 c_5 c_6 c_7};

% reconstruct rate matrix
R_F = zeros(length(C),length(C));
for i = 1:length(C)
    for j = 1:length(C)
%         R_F(i,j) = fun_build_cluster_k(C{i},C{j},R);
        R_F(i,j) = fun_build_cluster_k_adv(C{i},C{j},R,eigen_energy);  
    end
end

% % cluster exact pop
figure
hold
% P1 = P(8,:) + P(3,:) + P(6,:);
% P2 = P(1,:)+ P(7,:)+P(2,:)+P(4,:)+P(5,:);
P_C = zeros(length(C),length(t));
for i = 1:length(C)
    for j = C{i}
        P_C(i,:) = P_C(i,:) + P(j,:);
    end
end



for i = 1:length(C)
    plot(t,P_C(i,:),'LineWidth',4);
end
% title('Propogation - exact');
% xlabel('ps-1');
%% cluster Ford Fulkerson pop
% figure
% hold

P_F = zeros(length(C),1);
P_F(:,1) = P_C(:,1);
for i = 1:t_step-1
    P_F(:,i+1) = (eye(size(R_F,1)) + R_F*dt)*P_F(:,i)/sumabs(P_F(:,i));
end

for i = 1:length(C)
    plot(t,P_F(i,:),'--','LineWidth',3);
end
title('Propogation - reconstruct');
xlabel('ps-1');
% legend('C1 exact','C2 exact','C3 exact','C4 exact','C1 reconstruct','C2 reconstruct','C3 reconstruct','C4 reconstruct');
% title('Propogation - cluster - 1 cut');

% [xi,yi] = polyxpoly(t,P1,t,P2);
% mapshow(xi,yi,'DisplayType','point','Marker','o');
% str1 = num2str(xi,3);
% text(xi,yi+0.05,str1,'HorizontalAlignment','left');
% 
% [xi,yi] = polyxpoly(t,P_F(1,:),t,P_F(2,:));
% mapshow(xi,yi,'DisplayType','point','Marker','o');
% str2 = num2str(xi,3);
% text(xi,yi-0.05,str2,'HorizontalAlignment','left');