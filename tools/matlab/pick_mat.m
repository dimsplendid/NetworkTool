function [chs_mat] = pick_mat(mat,choose)
% choose the matrix by 'choose' array
size_t = length(choose);
chs_mat = zeros(size_t);
chs_energy = zeros(size_t,1);
for i = 1:size_t
    for j = 1:size_t
        chs_mat(i,j) = mat(choose(i),choose(j));
    end
end
% % modified Rate matrix
% for i = 1:size(chs_mat,1)
%     chs_mat(i,i) = 0;
% end
% for i = 1:size(chs_mat,1)
%     chs_mat(i,i) = -sum(chs_mat(:,i));
% end
% for i = 1:size_t
%     chs_energy(i) = site_energy(choose(i));
% end
% end