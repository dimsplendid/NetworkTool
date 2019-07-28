s_flow =0.3; t_flow = 0.3;
flw_mat=build_st_matrix(LHCII_r,s_flow,t_flow);
[m_flw,s,t]=st_cut(flw_mat);
s = s - 1
t = t - 1
