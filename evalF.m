dfs = {'out','', 'thrs',0.1:0.1:1, 'maxDist',.0075, 'thin',1};
img = imread('pig_prob_map.jpg');
G=load('Pig.mat'); 
G=G.groundTruth; 
n=length(G);
% Z=zeros(n,10); 
% R=Z;P=Z;
% mean_R = zeros(n,1); mean_P = zeros(n,1);
% for g=1:n
%     [R,P] = evalEdge(img,G{g},dfs);       
%     mean_R(g) = mean(R(g,:));
%     mean_P(g) = mean(P(g,:));
% end
% mean_F = 2*mean(mean_R)*mean(mean_P)/(mean(mean_R) + mean(mean_P));

F = edgesEvalImgNew(img, 'Pig.mat', dfs);
plot(0.1:0.1:1,F)
disp('Best F:')
disp(max(F))