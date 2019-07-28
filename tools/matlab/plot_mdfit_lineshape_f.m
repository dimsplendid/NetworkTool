%% plot for MD fitting lineshape function
CM2FS=5309.1;
C0=1;

%% C
Cr=load('PSII_MD_Cr.dat');
Ci=load('PSII_MD_Ci.dat');
figure;hold on;
plot(Cr(:,1),Cr(:,2),'r-','linewidth',2);
plot(Ci(:,1),Ci(:,2),'b-','linewidth',2);
xlabel('ps');
ylabel('C (cm^{-2}/C(0))');
legend('Cr','Ci');
title('C - Time Correlation function - MD fitting');
%% H
Hr=load('PSII_MD_Hr.dat');
Hi=load('PSII_MD_Hi.dat');
figure;hold on;
plot(Hr(:,1),Hr(:,2)*1e3/CM2FS,'r-','linewidth',2);
plot(Hi(:,1),Hi(:,2)*1e3/CM2FS,'b-','linewidth',2);
xlabel('ps');
ylabel('H (cm^{-1}/C(0))');
legend('Hr','Hi');
title('H - MD fitting');
%% G
Gr=load('PSII_MD_Gr.dat');
Gi=load('PSII_MD_Gi.dat');
figure;hold on;
plot(Gr(:,1),Gr(:,2)*1e6/CM2FS/CM2FS,'r-','linewidth',2);
plot(Gi(:,1),Gi(:,2)*1e6/CM2FS/CM2FS,'b-','linewidth',2);
xlabel('ps');
ylabel('G (1/C(0))');
legend('Gr','Gi');
title('G - Lineshape function - MD fitting');