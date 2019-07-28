function cost = cluster_cost(R_exact,R_FF,cut_off)
% compute the cost of exact dynamics and clutering dynamics
cost = 0.0;
cut_off_time = 1/cut_off; % ps
size_exact = size(R_exact,1);
size_FF = size(R_FF,1);
% time (ps)
t_i = 0;
t_f = 10*cut_off_time;
t_step = (t_f-t_i)*10000;
t = linspace(t_i,t_f,t_step);
dt = (t_f - t_i)/t_step;

% init Population
P = zeros(size_exact,t_step);
P_C = zeros

end