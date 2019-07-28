%% scan the parameter alpha and beta
% alpha: site energy
% beta: coupling
PSII_M1_H = load('M1H.dat');
site_energy = diag(PSII_M1_H);
coupling = PSII_M1_H - diag(site_energy);
alpha = 0.6:.1:1.1;
beta = 0.6:.1:1.1;
energy_gap = zeros(6,6);
for i = 1:6
    modified_site_energy = site_energy*alpha(i);
    for j = 1:6
        modified_coupling = coupling*beta(j);
        E = eig(diag(modified_site_energy) + modified_coupling);
        energy_gap(i,j) = max(E) - min(E);
    end
end