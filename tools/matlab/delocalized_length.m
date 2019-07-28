function L =delocalized_length(energy_state,all_site_coeff)

site_coeff=all_site_coeff(:,energy_state);

L = 1/sum(site_coeff.^4);
end