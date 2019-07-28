function picture = plot_coupling(H)

picture = figure;
size_t = length(diag(H));
%Generate the X and Y grid arrays using the MESHGRID function.
x = [1:size_t];
y = [1:size_t];
[X,Y] = meshgrid(x,y);
%Note that size(Z) is the same as size(x) and size(y)
for i=1:size_t
    H(i,i) = 0;
end

color_bar_range = [0,1.5];
colormap jet;

%plot the figure
pcolor(X,Y,H);
% image(H,'CDataMapping','scaled');
colorbar;
caxis(color_bar_range);
xlabel('j');
ylabel('i');
% axis equal;
end