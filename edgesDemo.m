% Demo for Structured Edge Detector (please see readme.txt first).

%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=1;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=1;          % for top speed set nTreesEval=1
model.opts.nThreads=4;            % max number threads for evaluation
model.opts.nms=1;                 % set to trued to enable nms

%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
I = imread('Tiger.jpg');
tic, E=edgesDetect(I,model); toc
z = zeros(321,481);
X=1-E;
for i = 1:321
    for j = 1:481
        if (X(i,j))>0.5
            z(i,j) = 255;
        else
            z(i,j) = 0;
        end
    end
end
imwrite(X, 'tiger_prob_map.jpg');
imwrite(z, 'tiger_binary_map.jpg');

dfs = {'out','', 'thrs',0.1:0.1:1, 'maxDist',.0075, 'thin',1};
F = edgesEvalImgNew(X, 'Tiger.mat', dfs);
plot(0.1:0.1:1,F)
disp('Best F:')
disp(max(F))

G=load('Tiger.mat'); 
G=G.groundTruth; 
n=length(G);
Z=zeros(n,10); 
R=Z;P=Z;
mean_R = zeros(n,1); mean_P = zeros(n,1);
for g=1:n
    [R,P] = evalEdge(X,G{g},dfs);       
    mean_R(g) = mean(R(g,:));
    mean_P(g) = mean(P(g,:));
end
mean_F = 2*mean(mean_R)*mean(mean_P)/(mean(mean_R) + mean(mean_P));
