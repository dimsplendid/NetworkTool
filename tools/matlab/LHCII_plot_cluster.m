function pictures = LHCII_plot_cluster(cluster)
    % LHCII monomer
    c_1 = [1,2,5];
    c_2 = [4 7];
    c_3 = 3;
    c_4 = [6 11 12];
    % c_4 = 4;
    % % c_4 = [6,11,12];
    c_5 = [8 9 10 13];
    c_6 = 14;
    % c_7 = 7;
    % c_8 = 10;
    % c_9 = 7;
    % c_10 = 12;
    % c_11 = 13;
    C = {c_1 c_2 c_3 c_4 c_5 c_6};
    
    Eigenv = load('eigenvector.dat');
    rMg = [
            -18.158   14.460  114.324
             -5.599   12.900  116.078
              3.559   10.440  109.134
              7.259   26.531   98.559
             21.882   29.522   93.137
             14.148   22.318   99.857
              9.392   15.179   96.182
             13.446   24.623  115.521
             12.060   14.876  111.381
              2.164   27.506  116.347
            -15.551   25.638  113.532
             -7.366   29.125  109.632
            -11.311   15.961   97.821
            -19.152   20.675   95.871
        ];

    eigen_energy=[
    14996.2
    15049.1
    15096.3
    15155.1
    15247.5
    15250.3
    15314.5
    15367.5
    15539.5
    15557.3
    15618.8
    15688
    15719.4
    15769.6

    ];

    
%     site_coeff=Eigenv(:,energy_state);
    site_population = zeros(14,1);
    for energy_state = C{cluster}
        site_population = site_population + Eigenv(:,energy_state).^2;
    end
    pictures=figure(cluster);
    % Error = 0;
    % clf;
    hold on

    for j=1:1:14
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
    end

    % plot LHCII
    for j=[2,3,10,11,12]
        plot(rMg(j,1),rMg(j,2),'Marker','diamond','Color','k','MarkerSize',14);
        %text(rMg(j,1)+0.2,rMg(j,2)+0.2,int2str(600+j));
    end
    for j=[ 1 8 9]
        plot(rMg(j,1),rMg(j,2),'Marker','diamond','Color','b','MarkerSize',14);
        %text(rMg(j,1)+0.2,rMg(j,2)+0.2,int2str(600+j));
    end
    for j=[4 13 14]
        plot(rMg(j,1),rMg(j,2),'Marker','*','Color','k','MarkerSize',14);
        %text(rMg(j,1)+0.2,rMg(j,2)+0.2,int2str(600+j));
    end
    for j=5:7
        plot(rMg(j,1),rMg(j,2),'Marker','*','Color','b','MarkerSize',14);
        %text(rMg(j,1)+0.2,rMg(j,2)+0.2,int2str(600+j));
    end
    
    for i=1:length(C{cluster})
        energy_level = C{cluster}(i);
        text(10,10+1*i,[int2str(eigen_energy(energy_level)) ' cm^-1']);
    end
end
