function [ order_flw_mat ] = order_flow( flow_mat )
    size_t = size(flow_mat,1);
    order_flw_mat = cell(1,size_t);
    for i = 1:size_t;
        [~,order] = sort(flow_mat(:,i));
        order_flw_mat{i} = order;
    end
end

