for i = 1:20
st_flow =0.1+0.03*i; t_flow = 0.02+0.03*i;
flw_mat=build_st_matrix(st1,s_flow,t_flow);
[m_flw,s,t]=st_cut(flw_mat);
s = s - 1
t = t - 1
end