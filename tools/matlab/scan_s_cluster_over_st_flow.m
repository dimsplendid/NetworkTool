function [s_cluster,t_cluster,o_cluster,scan_rng,flow]=scan_s_cluster_over_st_flow(rate,scan_length)
% clear;
% % initialize
% rate = load('LHCII_rate_1680.data');
% for i = 1:size(rate,1)
%     rate(i,i) = 0;
% end
% clear i;

% get min & max rate (non-zero)
get_extreme = rate;
max_stflow = max(max(get_extreme));
get_extreme(~get_extreme) = nan;
min_stflow = min(min(get_extreme));
clear get_extreme;

% scan_length = 200
scan_rng = linspace(min_stflow,max_stflow,scan_length);
s_cluster = cell(scan_length,1);
t_cluster = cell(scan_length,1);
o_cluster = cell(scan_length,1);
s_cluster_length = zeros(1,scan_length);
t_cluster_length = zeros(1,scan_length);
o_cluster_length = zeros(1,scan_length);
flow = zeros(1,scan_length);
% scan and calculate s t 
for i = 1:length(scan_rng)
    st_rate=build_st_matrix(rate,scan_rng(i));
    [f,s,t,o] = st_cut(st_rate);
    s_cluster{i} = s;
    s_cluster_length(i) = length(s);
    t_cluster{i} = t;
    t_cluster_length(i) = length(t);
    o_cluster{i} = o;
    o_cluster_length(i) = length(o);
    flow(i) = f;
end
hold off;
hold on;
xlabel('st flow');
ylabel('# of nodes');
STO=plot(scan_rng,s_cluster_length,'r-',scan_rng,t_cluster_length,'g-',scan_rng,o_cluster_length,'k:');
legend(STO,'s cluster','t cluster','other cluster');
figure;
ylabel('max flow');
xlabel('st flow');
plot(scan_rng,flow);
hold off;
end

% st_rate = build_st_matrix(rate,0.2);
% [~,s,~]=st_cut(st_rate);
% s_length = length(s)