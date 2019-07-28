function [ max_flow, s_cluster, t_cluster, other_cluster ] = st_cut(flow_matrix)
    max_flow = 0;
    size_t = size(flow_matrix,1);
    total_set = 1:size_t;
    s_cluster = 1; % [1]
    t_cluster = size_t;
    out_flow = order_flow(flow_matrix);
    path = 1; % [1]
    count = 0;
    is_path = 1;
    while is_path  && count < 1000000%flow_matrix(out_flow{1}(end),1)~=0; % there is a way
         count = count +1 ;%if inf loop
%         out_flow = order_flow(flow_matrix);
        local_max_flw = inf;
        pre = path(end);
        while flow_matrix(out_flow{pre}(end),pre) ~= 0;
            
            next = out_flow{pre}(end);
            out_flow{pre}(end) = [];

            path = [path,next];
            pre = next;
            local_max_flw = min(local_max_flw,flow_matrix(path(end),path(end-1)));
            if flow_matrix(out_flow{pre}(end),pre) <= 1e-6 && pre ~= size_t;
                path(end) = [];
                pre = path(end);
                is_path = 0;
            else % this way can go to and we don't want the path go back
                for i = 1 : size(out_flow,2)
                    out_flow{i} = out_flow{i}(out_flow{i}~=next);
                end         
                is_path = 1;
            end
        end
%         path % print path
        % update the flow_matrix and order flow mat
        for j = 1:length(path)-1;
            
            flow_matrix(path(j+1),path(j)) = flow_matrix(path(j+1),path(j)) - local_max_flw;
        end
        if is_path == 1
            max_flow = max_flow + local_max_flw;
        end
        out_flow = order_flow(flow_matrix);
%         is_path
%         pre
%         out_flow{1}(end)
%         flow_matrix
    % re-inital path
        path = 1;
    end
    % start to clustering
    % s-cluster, nodes that can go back to s
    out_flow = order_flow(flow_matrix);
    pre = 1;
    while flow_matrix(out_flow{pre}(end),pre) ~= 0;
        % pop out_flow{pre}
        next = out_flow{pre}(end);
        out_flow{pre}(end)=[];
        % s_cluster.push(id)
        s_cluster = [s_cluster, next];
        if flow_matrix(out_flow{next}(end),pre) ~= 0;
            pre = next;
        else
            pre = 1;
        end
    end
    
    % remove the duplicate number in s
    tmp_cluster = total_set;
    for s = s_cluster;
        tmp_cluster = tmp_cluster(tmp_cluster ~= s);
    end
    s_cluster = total_set;
    for t = tmp_cluster;
        s_cluster =s_cluster(s_cluster ~= t);
    end
    
    % t-cluster, nodes connect to t
    %
    % back_flow : reverse s and t, that I can use the same alogorithms
    % from s_cluster
    %
    back_flow = flow_matrix';
    out_flow = order_flow(back_flow);
    pre = size_t;
    while back_flow(out_flow{pre}(end),pre) ~= 0;
        % pop out_flow{pre}
        next = out_flow{pre}(end);
        out_flow{pre}(end)=[];
        % t_cluster.push(id)
        t_cluster = [t_cluster, next];
        if back_flow(out_flow{next}(end),pre) ~= 0;
            pre = next;
        else
            pre = size_t;
        end
    end
    % remove the duplicate number in t
    tmp_cluster = total_set;
    for t = t_cluster;
        tmp_cluster = tmp_cluster(tmp_cluster ~= t);
    end
    t_cluster = total_set;
    for t = tmp_cluster;
        t_cluster =t_cluster(t_cluster ~= t);
    end
    % others, not in both s and t cluster
    other_cluster = total_set;
    for t = t_cluster;
        other_cluster = other_cluster(other_cluster ~= t);
    end
    for s = s_cluster;
        other_cluster = other_cluster(other_cluster ~= s);
    end
        
%     loop_times = count 
end

