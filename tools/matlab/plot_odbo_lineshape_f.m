%% plot C, H, G from CMRT kernel function
% lambda = 70 cm-1 gamma = 40 cm-1
C=dlmread('odbo_C.dat');
H=dlmread('odbo_H.dat');
G=dlmread('odbo_G.dat');

%% C
figure;hold on;
plot(real(C(:,1))/1000,real(C(:,2)),'r-','linewidth',2);
plot(real(C(:,1))/1000,imag(C(:,2)),'b-','linewidth',2);
legend('Cr','Ci');xlabel('ps');ylabel('C(cm^{-2})');
title('C - Time Correlation function - Over-damped Brownian Oscillator');

%% H
figure;hold on;
plot(H(:,1)/1000,real(H(:,2)),'r-','linewidth',2);
plot(H(:,1)/1000,imag(H(:,2)),'b-','linewidth',2);
legend('Hr','Hi');xlabel('ps');ylabel('H(cm^{-1})');
title('H - Over-damped Brownian Oscillator');

%% G
figure;hold on;
plot(G(:,1)/1000,real(G(:,2)),'r-','linewidth',2);
plot(G(:,1)/1000,imag(G(:,2)),'b-','linewidth',2);
legend('Gr','Gi');xlabel('ps');ylabel('G(unitless)');
title('G - Lineshape function - Over-damped Brownian Oscillator');