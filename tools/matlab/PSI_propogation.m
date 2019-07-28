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
R = dlmread('data/PSI_rate.dat');
% change unit from fs^-1 to ps^-1
% R = R*1000;
% eigenvector eigenvalue
H = load('data/PSI_Hamiltonial.dat');
[Eigenv, eigen_energy]  = eig(H);
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

% P(size_t,1) = 1;
P(:,1) = 1/size_t;

for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sumabs(P(:,i));
end

P_site = P*0;

% total pop plot
figure
hold
for i = 1:size(R,1);
    plot(t,P(i,:));
end
% 
% legend('1','2','3','4','5','6','7','8','9','10','11','12','13','14');
% title('Propogation - exiton');

% % figure
% % hold
% % 
% % for j = 1:size(R,1)
% %     for n = 1:size(R,1)
% %         P_site(j,:) = P_site(j,:) + Eigenv(n,j).^2 * P(n,:);
% %     end
% %     plot(t,P_site(j,:));
% % end
% % 
% % title('Propogation - site');

% PSI cluster
% c_1 = [1];
% c_2 = [2];
% c_3 = [3:95];
% c_3_exclusive = [5 85 96 81 87]; % c_3 exclude
% % remove c4 element in c3
% for i = 1:length(c_3_exclusive);
%     c_3(c_3 == c_3_exclusive(i)) = [];
% end
% c_4 = [5];
% c_5 = [85];
% c_6 = [96];

% Modified tree cluster
% m_1 = [96 85];
% m_2 = [87 81];
% m_3 = [68 19];
% m_4 = [67 64];
% m_5 = [63 54];
% m_6 = [52 51];
% m_7 = [61 59 57 55 50];
% m_8 = [49 48];
% m_9 = [58 38];
% m_10 = [53 32 29];
% m_11 = [33 27];
% m_12 = [26 22];
% m_13 = [46 42 37 36 16];
% rest_exclude = [m_1 m_2 m_3 m_4 m_5 m_6 m_7 m_8 m_9 m_10 m_11 m_12 m_13];
% rest = 1:96;
% for i = 1:length(rest_exclude)
%     rest(rest == rest_exclude(i)) = [];
% end
C1 = [96 85];
C2 = [5];
C3 = [93];
C4 = [3];
C5 = [4];
C6 = [89];
C7 = [6];
C8 = [8];
C9 = [83];
C10= [82];
C11= [10];
C12= [13];
C13= [74];
C14= [72];
C15= [71];
C16= [66];
C17= [65];
C18= [15];
C19= [42];
C20= [37 36 16];
C21= [33 27];
C22= [26 22];
C23= [24];
C24= [25];
C25= [39];
C26= [40];
C27= [49 48];
C28= [61 59 57];
C29= [55];

C97= [63 54 52 51 43];
% C112=[46 42 37 36 16];

% C113=[61 59 57 55 50 49 48 47 45 44 41 40 39 35 34 31 30 25 24 23 21 20 18];
% C114=[60 58 56 53 38 33 32 29 28 27 26 22 17];

RC = [1];
% rest_exclude = [C113 C114 C112 RC C1 C2];
rest_exclude=[C1 C2 C3 C4 C5 C6 C7 C8 C9 C10 C11 C12 C13 C14 C15 C16 C17 C18 C19 C20 C21 C22 C23 C24 C25 C26 C27 C28 C29 C97 RC];
rest = 1:96;
for i = 1:length(rest_exclude)
    rest(rest == rest_exclude(i)) = [];
end

% C = {m_1 m_2 m_3 m_4 m_5 m_6 m_7 m_8 m_9 m_10 m_11 m_12 m_13 rest};
C = {C1 C2 C3 C4 C5 C6 C7 C8 C9 C10 C11 C12 C13 C14 C15 C16 C17 C18 C19 C20 C21 C22 C23 C24 C25 C26 C27 C28 C29 C97 RC rest};

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
% plot_cluster = 2:13;

% for i = plot_cluster
%     plot(t,P_C(i,:),'LineWidth',4);
% end

% title('Propogation - exact');
% xlabel('ps-1');
%% cluster Ford Fulkerson pop
% figure
% hold

% init population
P_F = zeros(length(C),1);
P_F(:,1)=P_C(:,1);

for i = 1:t_step-1
    P_F(:,i+1) = (eye(size(R_F,1)) + R_F*dt)*P_F(:,i)/sumabs(P_F(:,i));
end

for i = 1:length(C)
    plot(t,P_F(i,:),'--','LineWidth',3);
end

% for i = plot_cluster
%     plot(t,P_F(i,:),'--','LineWidth',3);
% end

title('Propogation - reconstruct');
xlabel('ps');
legend('C1 ','C2 ','C3 ','C4 ','C5 ','C6 ','C7 ','C8 ','C9 ','C10','C11','C12','C13','C14','C15','C16','C17','C18','C19','C20','C21','C22','C23','C24','C25','C26','C27','C28','C29','RC','rest','C1 ','C2 ','C3 ','C4 ','C5 ','C6 ','C7 ','C8 ','C9 ','C10','C11','C12','C13','C14','C15','C16','C17','C18','C19','C20','C21','C22','C23','C24','C25','C26','C27','C28','C29','RC','rest');
% title('Propogation - cluster - 1 cut');

% legend('3','7','3','7');
% [xi,yi] = polyxpoly(t,P1,t,P2);
% mapshow(xi,yi,'DisplayType','point','Marker','o');
% str1 = num2str(xi,3);
% text(xi,yi+0.05,str1,'HorizontalAlignment','left');
% 
% [xi,yi] = polyxpoly(t,P_F(1,:),t,P_F(2,:));
% mapshow(xi,yi,'DisplayType','point','Marker','o');
% str2 = num2str(xi,3);
% text(xi,yi-0.05,str2,'HorizontalAlignment','left');
