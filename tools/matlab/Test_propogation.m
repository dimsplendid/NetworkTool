%% Test another way to the population propogation

% Using the rate matrix of LHCII
R = dlmread('LHCII_rate_mat.data');
% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end
R = -R;

% set initial state
size_t = size(R,1);
P = zeros(size_t,1);
% P(size_t,1) = 1;
for i=1:size_t
    P(i,1)=1/size_t;
end

% time (ps)
t_i = 0;
t_f = 20;
dt = 5;
t_step = (t_f-t_i)/dt;
t = linspace(t_i,t_f,t_step);

%% Method 1, simplest first order: Cranck-Nicoson
P_1 = P;
for i = 1:1:t_step-1
    P_1(:,i+1) = (eye(size(R,1)) + R*dt )*P_1(:,i);
    P_1(:,i+1) = P_1(:,i+1)/sumabs(P_1(:,i+1));
end

%% Method 2, using the eigenvector of rate matrix
P_2 = P;
[Evec,Eval]=eig(R);
for i = 1:1:t_step-1
    P_2(:,i+1)=zeros(size_t,1);
    for j=1:size_t
        P_2(:,i+1) = P_2(:,i+1)+Evec(:,j)'*P_2(:,1)*exp(Eval(j,j)*t(i))*Evec(:,j);
    end
end

% total pop plot
figure
hold
for i = 1:size(R,1);
%     plot(t,P_1(i,:),'b','LineWidth',2);
    plot(t,P_2(i,:),'r','LineWidth',2);
end
xlabel('ps');
ylabel('Populations');
legend('Cranck-Niconson','exponetial');