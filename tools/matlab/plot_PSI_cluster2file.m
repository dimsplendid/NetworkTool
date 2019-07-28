for i = 1:10
    name = ['PSI_cluster' int2str(i) '.png'];
    print(PSI_plot_cluster(i),name,'-dpng');
    close all;
end