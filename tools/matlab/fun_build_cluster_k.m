function cluster_k_ij = fun_build_cluster_k ( cluster_i,cluster_j, k_origin)
	% build cluster k_ij in uniform approximation
	% N_Si = length(cluster_i);
	ans = 0.0;
	N_Sj = length(cluster_j);
	for i = cluster_i
		for j = cluster_j
			ans = ans + k_origin(i,j);
		end
	end

	cluster_k_ij = ans / N_Sj;

end
