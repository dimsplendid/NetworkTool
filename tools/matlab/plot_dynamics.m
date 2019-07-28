function [P] = plot_dynamics(R,P0)

size_t = size(R,1);

% modified Rate matrix
for i = 1:size_t
    R(i,i) = 0;
end

R(R<0) = 0;

for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end

% set up time interval
t_i = 0;
t_f = 6;
t_step = (t_f-t_i)*100;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% init population
P = zeros(size(R,1),length(t));
P(:,1) = P0;

% calculate the dynamics
for i = 1:1:t_step-1
    P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sum(abs(P(:,i)));
end

% plot the fiugre of dynamics
fig = figure;
set(gca,'fontsize',18)
hold on;
for i = 1:size(P,1)
    plot(t,P(i,:),'LineWidth',3);
end

end
