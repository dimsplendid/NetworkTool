for i = 1:14
    name = ['LHCII_state' int2str(i) '.png'];
    print(LHCII_coeff(i),name,'-dpng');
    close all;
end
