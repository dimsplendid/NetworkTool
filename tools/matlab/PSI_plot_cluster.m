function pictures = PSI_plot_cluster(cluster)
    % PSI
    PSI_CGM;
    C = G_012{11};
    
    Hamiltonian = load('data/PSI_Hamiltonial.dat');
    [Eigenv,eigen_energy] = eig(Hamiltonian);
    eigen_energy=diag(eigen_energy);
    % PDB: 1JB0
    rMg = [
          99.336 121.925  87.514
          99.587 115.599  87.869
          90.235 119.779  80.454
         108.890 117.508  80.610
          88.434 120.740  71.952
         111.419 115.742  72.992
          92.078 144.328  63.710
         100.668 148.286  64.014
         111.956 147.338  64.947
         107.438 146.217  73.181
          95.856 154.049  85.214
          97.798 144.680  88.190
          87.334 143.176  87.920
         114.818 164.166  64.285
         101.131 157.709  59.225
         126.240 164.137  65.912
         120.195 156.979  67.351
         117.636 163.903  88.121
         128.718 167.634  89.389
         114.254 171.227  90.376
         137.815 155.406  89.740
         130.668 147.828  87.900
         123.151 151.485  85.294
         134.155 163.604  70.354
         132.622 147.616  70.776
         142.279 151.862  67.282
         142.134 141.820  65.673
         133.718 135.988  63.818
         126.013 145.343  66.240
         122.461 131.511  76.766
         128.287 137.268  86.259
         106.654 140.135  83.708
         113.324 145.346  82.801
         109.794 138.718  64.799
         132.805 124.283  62.668
         133.763 114.802  65.910
         128.205 107.639  80.371
         121.882 103.998  87.246
         136.075 135.483  88.783
         143.216 142.133  90.451
         124.632 128.826  88.450
         129.862 119.916  83.766
         128.354 123.348  74.250
          76.140 130.749  64.170
          82.012 135.002  60.639
          92.607 132.811  72.044
          99.490  88.877  65.812
          87.652  89.049  65.849
          91.604  90.741  74.081
         102.861  82.686  86.900
         100.709  91.750  89.827
         110.396  96.115  90.326
         118.410  95.801  86.617
          84.213  72.278  66.205
          73.019  72.622  67.346
          78.967  80.031  68.417
          80.103  74.386  89.160
          69.878  70.269  90.070
          60.048  82.180  89.428
          67.304  90.026  87.822
          75.012  86.533  85.649
          65.664  89.432  70.707
          65.037  73.599  70.804
          56.824  84.785  65.975
          56.908  94.664  63.003
          65.739 100.282  62.523
          73.107  91.215  66.017
          77.333 105.918  76.038
          70.499 100.826  85.955
          92.117  97.307  84.436
          85.249  92.613  83.668
          89.671  97.738  65.728
          67.070 112.353  61.335
          66.738 122.038  64.708
          72.794 131.018  77.144
          77.849 135.098  86.809
          62.707 103.450  87.799
          55.433  99.450  88.245
          48.349  95.832  86.526
          74.589 109.234  87.067
          69.808 118.761  82.339
          71.700 114.089  73.150
         124.157 105.110  66.249
         118.481 101.323  62.915
         108.309 105.403  74.095
          73.753 142.986  72.968
          78.466 156.069  84.329
          60.010 142.167  87.054
         147.409 152.034  85.433
         144.632 169.551  79.908
         147.351  97.015  69.713
         135.880 100.445  70.969
         139.001 101.304  83.832
         104.043  66.349  69.638
          60.378 117.189  78.178
         144.354 132.027  63.743
        ];
    
%     site_coeff=Eigenv(:,energy_state);
    site_population = zeros(96,1);
    for energy_state = C{cluster}
        site_population = site_population + Eigenv(:,energy_state).^2;
    end
    pictures=figure(cluster);
    % Error = 0;
    % clf;
    hold on

    for j=1:1:96
    %     if norm(site_coeff(j)) > 0
    %         if site_coeff(j) < 0
    %         plot(rMg(j,1),rMg(j,2),'.','MarkerSize',norm(site_coeff(j))*120+0.1,'MarkerEdgeColor','b');
    %         elseif site_coeff(j) > 0 
    %         plot(rMg(j,1),rMg(j,2),'.','MarkerSize',norm(site_coeff(j))*120+0.1,'MarkerEdgeColor','r');
    %         else
    %             Error = 1;
    %         end
    %     else
    %         Error = 0;
    %     end
        plot(rMg(j,1),rMg(j,2),'.','MarkerSize',site_population(j)*120+0.1,'MarkerEdgeColor','r');
        if site_population(j) > 0.5
%             text(rMg(j,1)+2,rMg(j,2)+5,num2str(j),'FontSize',18);
        end
    end

    % plot PSI
    for i = 1:96
        plot(rMg(i,1),rMg(i,2),'*b','MarkerSize',20);
        text(rMg(i,1)+2,rMg(i,2)+5,num2str(i),'FontSize',18);
    end
    
    for i=1:length(C{cluster})
        energy_level = C{cluster}(i);
%         text(10,10+1*i,[int2str(eigen_energy(energy_level)) ' cm^-1']);
    end
    plot(rMg(1,1),rMg(1,2),'*k','MarkerSize',20);
    text(rMg(1,1)+2,rMg(1,2)-5,'RC','FontSize',18);
end
