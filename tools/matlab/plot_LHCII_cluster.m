function [cost,R_F] = plot_LHCII_cluster(Gn,H,R)
eigen_energy= eig(H);
R_F = zeros(length(Gn),length(Gn));
for i = 1:length(Gn)
    for j = 1:length(Gn)
        R_F(i,j) = fun_build_cluster_k_adv(Gn{i},Gn{j},R,eigen_energy);  
    end
end

% time (ps)
t_i = 0;
t_f = 60;
t_step = (t_f-t_i)*1000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;
% init
P = zeros(length(eigen_energy),length(t));
P(:,1) = 1/length(eigen_energy);
% P(14,1) = 1;

% for i = 1:length(Gn)
%     for j = Gn{i}
%         P(j,1) = 1/length(Gn{i})/length(Gn);
%     end
% end

for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sum(abs(P(:,i)));
%     P(:,i+1) = expm(R.*t(i+1))*P(:,1);
end
P_C = zeros(length(Gn),length(t));
for i = 1:length(Gn)
    for j = Gn{i}
        P_C(i,:) = P_C(i,:) + P(j,:);
    end
end

P_F = zeros(length(Gn),1);
P_F(:,1) = P_C(:,1);
for i = 1:t_step-1
    P_F(:,i+1) = (eye(size(R_F,1)) + R_F.*dt )*P_F(:,i)/sum(abs(P_F(:,i)));
%     P_F(:,i+1) = expm(R_F.*t(i+1))*P_F(:,i);
end

% plot dynamics

figure;
set(gca,'fontsize',18)
hold on;
for i = 1:length(Gn)
    plot(t,P_C(i,:),'LineWidth',4);
end

ax = gca;
ax.ColorOrderIndex=1;

for i = 1:length(Gn)
    plot(t,P_F(i,:),'--','LineWidth',4);
end
hold off;
xlabel('time(ps)');
ylabel('population');
% compute the cost
cost = sum(sum((P_C-P_F).^2))/size(P_C,1)/size(P_C,2);
legend_index = 1:size(P_C,1);
legend_cell = cellstr(num2str(legend_index','C%-d'));
legend(legend_cell);
% P(:,end)
end
