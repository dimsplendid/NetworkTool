for i = 1:96
    name = ['PSI_exciton_state' int2str(i) '.png'];
    print(PSI_plot_exciton_state(i),name,'-dpng');
    close all;
end