cd% Drude-Lorentz model time correlation funciton
% Bi-exponetial fitting
% J(w) = 2*lambda*gamma*w/(w^2+gamma^2)
% all unit are cm

% trans unit factor
CM2FS = 5308.8;

% init params
T = 300; % K
k = 1.38064852E-23; % Boltzmann constant (m2 kg s-2 K-1)
beta =  1/k/T/5.03445e22 ;% unit: cm
% Wu, J.,etc J. Phys. Chem. Lett. 2015, 1240–1245. parameter for LHCII
% monomer
% lambda = 85; % bath reorganization energy(cm-1)
% gamma  = 100.02; % cut-off frequency (cm-1), gamma^-1 = 53.08 fs

% Vladimir I. Novoderezhkin, Jan P. Dekker,and Rienk van Grondelle
% Biophys J. 2007 Aug 15; 93(4): 1293–1311.
% PSII reaction center
lambda = 70;
gamma = 40;

%% analytic form

%% bi-exponential aproximation
% Tian, B. L.; Ding, J. J.; Xu, R. X.; Yan, Y. 
% Biexponential Theory of Drude Dissipation via Hierarchical Quantum Master Equation. J. Chem. Phys. 2010, 133 (11), 1–6.
% C(t>0) ~ c1 exp(-r1 t) + c2 exp(-r2 t) + 2Δδ(t)
r1 = gamma;
r2 = sqrt(42)/beta;
Delta = beta*lambda*gamma/20;
c1 = (2*lambda/beta)*(1-10*(Delta/lambda)^2-2.45/((r2/r1)^2-1))-lambda*r1*1i;
c2 = 98*r2*Delta/(beta^2)/(r2^2-r1^2);

t = linspace(0,2000*(1/CM2FS),2000);
C = c1*exp(-r1*t)+c2*exp(-r2*t);
figure; hold on;
plot(t*CM2FS/1000,real(C)/real(C(1)),'r-','LineWidth',2);
plot(t*CM2FS/1000,imag(C)/real(C(1)),'b-','LineWidth',2);
xlabel('t(ps)');
ylabel('Time-correlation function C(cm^{-2})');
legend('Cr','Ci');
hold off;
%% H
H = (c1-c1*exp(-r1*t))/r1+(c2-c2*exp(-r2*t))/r2+Delta;

figure; hold on;
plot(t*CM2FS/1000,real(H)/real(C(1))/1000*CM2FS,'r-','LineWidth',2);
plot(t*CM2FS/1000,imag(H)/real(C(1))/1000*CM2FS,'b-','LineWidth',2);
xlabel('ps');
ylabel('cm^{-1}');
legend('Hr','Hi');
hold off;
%% G
G = c1*(-1+exp(-r1*t))/r1^2+c2*(-1+exp(-r2*t))/r2^2+c1*t/r1+c2*t/r2+Delta*t;

figure; hold on;
plot(t*CM2FS/1000,real(G)/real(C(1))/1e6*CM2FS*CM2FS,'r-','LineWidth',2);
plot(t*CM2FS/1000,imag(G)/real(C(1))/1e6*CM2FS*CM2FS,'b-','LineWidth',2);
xlabel('ps');
ylabel('unitless');
legend('Gr','Gi');
hold off;