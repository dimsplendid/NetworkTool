function cluster_energy = cluster_energy(cluster,eigen_energy)
    % 300 K
	beta = 0.00478683;
    Z = 0;
    for i = cluster
        Z = Z + exp(-beta * eigen_energy(i));
    end
    cluster_energy = 0;
    for i = cluster
        cluster_energy = cluster_energy + eigen_energy(i)*exp(-beta * eigen_energy(i))/Z;
    end
end
