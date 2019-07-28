function [P] = plot_dynamic(R)
% modified Rate matrix
for i = 1:size(R,1)
    R(i,i) = 0;
end
for i = 1:size(R,1)
    R(i,i) = -sum(R(:,i));
end
    t_i = 0;
    t_f = 60;
    t_step = (t_f-t_i)*100;
    t = linspace(t_i,t_f,t_step);
    dt = (t_f - t_i)/t_step;
    % init
    P = zeros(size(R,1),length(t));
    P(:,1) = 1/size(R,1);
%     P_test = P;
    % P(13,1) = 1;
    for i = 1:1:t_step-1
        P(:,i+1) = (eye(size(R,1)) + R*dt )*P(:,i)/sum(abs(P(:,i)));
    end
%     for i = 1:1:t_step-1
%         P_test(:,i+1) = expm(R.*t(i+1))*P_test(:,1);
%     end
    fig = figure;
    set(gca,'fontsize',18)
    hold on;
    for i = 1:size(P,1)
        plot(t,P(i,:),'LineWidth',3);
%         plot(t,P_test(i,:),'--','LineWidth',2);
    end
end
