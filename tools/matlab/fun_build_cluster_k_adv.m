function cluster_k_ij = fun_build_cluster_k_adv ( cluster_i,cluster_j, k_origin, site_energy)
	% build cluster k_ij in Boltzmann semi-equilibrium approximation
	% N_Si = length(cluster_i);
	% N_Sj = length(cluster_j);
	Z = 0.0;
	SUM = 0.0;
	% 300 K
	beta = 0.00478683;
    % 77 K
%     beta = 0.01865;
	%     --- N_Si
	% Z =  >          exp( - beta* E_j)
	%     --- j <- Si

	for i = cluster_j
		Z = Z + exp(-beta * site_energy(i));
    end
    
   	for i = cluster_i
		for j = cluster_j
			SUM = SUM + k_origin(i,j) * exp(-beta*site_energy(j));
		end
	end

	cluster_k_ij = SUM * Z^-1;

end
