% 1 d plot test
h1=figure(1);clf
subplot(1,4,1);
hold on
y_range = [-2,2];
xlim([-1,1]);ylim(y_range)

%arrow
[arrowX,arrowY]=dsxy2figxy([0,0],y_range);
annotation('arrow',arrowX,arrowY)

%crosses
% % normalized
% data=[0.0109
%     0.0742
%     0.0984
%     0.1277
%     0.6268
%     0.7820
%     0.8363
%     1.6416
%     1.7590
%     2.1110
%     4.5950
%    13.7220
%    38.9310];
% origin data
data = [
    38.9310
   27.4440
    8.2080
    4.6920
    4.5950
    4.2220
    2.5090
    2.5070
    2.2260
    1.7590
    1.2790
    1.1490
    0.1530
    ];
y_mod=linspace(-1.9,1.7,13);
plot(0,y_mod,'kx','markersize',10)
for i=1:length(data)
    text(0.5,y_mod(i),num2str(data(i),'%.3f'));
end

%pipes
% p=[0.5,0.65];
% text(p,[0,0],'$$\vert$$','interpreter','latex')

%text
% text([0.25,0.5,0.65],[1,-1,-1]/2,{'$$d_i$$','E[d]','$$\theta$$'},'interpreter','latex')

axis off
print('-depsc','arrowFigure')