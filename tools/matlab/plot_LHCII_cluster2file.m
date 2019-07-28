for i = 1:6
    name = ['LHCII_cluster' int2str(i) '.png'];
    print(LHCII_plot_cluster(i),name,'-dpng');
    close all;
end