% PSI summary

% define G
PSI_CGM;

R = dlmread('data/PSI_rate.dat');
H = load('data/PSI_Hamiltonial.dat');

% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end

%% eigenvector
[Eigenv,EVal] = eig(H);
eigen_energy=diag(EVal);

%%

cost={};
R_F={};
for i = 1:6
    for j=1:length(G{i})
        [cost{i}(j),R_F{i}{j}] = plot_LHCII_cluster(G{i}{j},H,R);
        close all;
    end
end
%%
figure;hold on;
for i = 1:6
    x_data = [];
    for j = 1:length(G{i})
        x_data(j) = length(G{i}{j});
    end
    plot(x_data,cost{i},'o-','Linewidth',3);

end
xlabel('number of clusters');
ylabel('cost');
legend('SC', 'SR', 'EC', 'norm-SC', 'norm-SR', 'norm-EC');
hold off;
%%
% figure;hold on;
% for i = 1:length(G)
%     x_data = [];
%     for j = 1:length(G{i})
%         x_data(j) = length(G{i}{j});
%     end
%     plot(x_data,cost{i},'Linewidth',2);
% end
% legend('000', '001', '002', '010', '011', '012', '100', '101', '102', '110', '111', '112');
% hold off;