function [ st_matrix ] = build_st_matrix( flow_matrix ,st_flow)
    st = st_flow;
    proto_size = size(flow_matrix,1);
    st_mat_size = proto_size+2;
    st_matrix = zeros(st_mat_size);
    st_matrix(2:st_mat_size-1,2:st_mat_size-1) = flow_matrix;
    for i = 2:st_mat_size-1;
        st_matrix(i,1) = st;
    end
    for j = 2:st_mat_size-1;
        st_matrix(st_mat_size,j) = st;
    end
end

