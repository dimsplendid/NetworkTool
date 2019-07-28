function pictures = LHCII_trimer_cluster(cluster)

Eigenv = load('LHCII_tri_EVec.dat');
rMg = [
        62.787 43.645 26.989
51.178 45.110 27.017
45.057 58.963 37.129
60.157 59.673 25.661
64.440 59.196 36.678
56.985 42.870 44.507
47.692 51.282 20.292
36.589 57.174 33.476
49.118 62.660 20.242
60.575 52.914 49.650
72.053 42.583 33.803
71.088 52.281 38.490
64.508 44.536 47.591
65.134 35.033 55.856
90.534 77.701 45.168
89.879 68.677 52.531
73.210 62.306 55.789
79.847 67.215 38.688
72.553 75.850 41.966
79.307 82.443 59.063
90.240 59.862 48.474
77.330 55.354 60.512
81.912 55.208 42.116
68.448 82.501 54.691
86.273 88.254 42.893
76.099 85.005 41.585
75.663 88.554 54.746
76.872 97.736 63.236
52.121 89.467 19.676
53.637 97.125 28.406
56.809 89.839 44.763
64.719 83.894 28.559
56.490 75.868 31.315
39.684 85.754 32.809
62.869 99.794 30.095
58.334 98.492 47.845
71.049 93.019 34.567
43.109 75.667 37.913
46.231 80.042 16.496
49.964 73.674 24.362
38.349 77.963 30.122
25.777 78.354 29.507
    ];

eigen_energy=load('LHCII_tri_EVal.dat');
C1 = [42];
C2 = [41];
C3 = [37 36 35 34 33 32 31 30 29 28 27 26 25 24];
C4 = [23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1];
C = {C1 C2 C3 C4};

site_population = zeros(42,1);
    for energy_state = C{cluster}
        site_population = site_population + Eigenv(:,energy_state).^2;
    end
pictures=figure(cluster);
hold on;
for j=1:42
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
for j=1:42
    plot(rMg(j,1),rMg(j,2),'*b');
end

title(['cluster ' int2str(cluster)]);
xlabel('Angstrom');
ylabel('Angstrom');
axis equal;
set(gca,'FontSize',20);
% Error
end