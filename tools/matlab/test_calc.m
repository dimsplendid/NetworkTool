% load test_all_1.dat;
% load test_all_2.dat;
% load test_all_3.dat;
test_all_1 = randn(100,3);
test_all_2 = randn(100,3);
test_all_3 = randn(100,3);

for c = 1:size(test_all_1(:,1));
    
    E(1) = test_all_1(c,1);
    E(2) = test_all_1(c,2);
    E(3) = test_all_1(c,3);
    totE = E(1) + E(2) + E(3);
    avgE = totE / 3;
    dE = E - avgE;

    D = 1.5 * (dE(1)^2 + dE(2)^2 + dE(3)^2) / totE^2;
    S = 27 * dE(1) * dE(2) * dE(3) / totE^3;

    out1(c,:) = [c D S E(1) E(2) E(3)];
end

for c = 1:size(test_all_2(:,1));

    E(1) = test_all_2(c,1);
    E(2) = test_all_2(c,2);
    E(3) = test_all_2(c,3);
    totE = E(1) + E(2) + E(3);
    avgE = totE / 3;
    dE = E - avgE;

    D = 1.5 * (dE(1)^2 + dE(2)^2 + dE(3)^2) / totE^2;
    S = 27 * dE(1) * dE(2) * dE(3) / totE^3;

    out2(c,:) = [c D S E(1) E(2) E(3)];

end

for c = 1:size(test_all_3(:,1));
    
    E(1) = test_all_3(c,1);
    E(2) = test_all_3(c,2);
    E(3) = test_all_3(c,3);
    totE = E(1) + E(2) + E(3);
    avgE = totE / 3;
    dE = E - avgE;

    D = 1.5 * (dE(1)^2 + dE(2)^2 + dE(3)^2) / totE^2;
    S = 27 * dE(1) * dE(2) * dE(3) / totE^3;

    out3(c,:) = [c D S E(1) E(2) E(3)];
end

plot(out1(:,2),out1(:,3),'g.', out2(:,2),out2(:,3),'r*',out3(:,2),out3(:,3),'bO');

xlabel('delta','fontsize',10)
ylabel('S','fontsize',10)
xlim([0,1])
ylim([-0.25,2])
set(gca,'xtick',0:0.25:1,'ytick',-0.25:0.25:2)

set(gcf,'PaperPositionMode','auto')
print('-depsc2','-zbuffer','-r300','ds_range.eps')

save test_ds_1.dat -ascii out1;
save test_ds_2.dat -ascii out2;
save test_ds_3.dat -ascii out3;
