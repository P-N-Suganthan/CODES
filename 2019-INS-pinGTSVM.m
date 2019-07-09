function [acc,err,time1,Predict_Y,A,B,w1,b1,w2,b2] = pinGTSVM(TestX,DataTrain,FunPara)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% pinGTSVM: Gerneral Twin Support Vector Machine With Pinball Loss
%
% Predict_Y = pinGTSVM(TestX,DataTrain,FunPara)
%
% Input:
%    TestX - Test Data Matrix. Each row vector of fea is a data point.
%
% DataTrain - Struct value in Matlab(Training data).
%                DataTrain.A: Positive input of Data matrix.
%                DataTrain.B: Negative input of Data matrix.
%
% FunPara - Struct value in Matlab. The fields in options that can be set: 
%              c1: [0,inf] Paramter to tune the weight. 
%              c2: [0,inf] Paramter to tune the weight. 
%              c3: [0,inf] Paramter to tune the weight. 
%              c4: [0,inf] Paramter to tune the weight. 
%              kerfPara:Kernel parameters. See kernelfun.m.
%
%
% Output:
%    Predict_Y - Predict value of the TestX.
%
% Examples:
%    DataTrain.A = rand(50,10);
%    DataTrain.B = rand(60,10);
%    TestX=rand(20,10);
%    FunPara.c1=0.1;
%    FunPara.c2=0.1;
%    FunPara.c3=0.1;
%    FunPara.c4=0.1;
%    FunPara.kerfPara.type = 'lin';
%    Predict_Y = pinGTSVM(TestX,DataTrain,FunPara);
%
% Reference:
%      M. Tanveer, A. Sharma, P.N. Suganthan, General twin 
%      support vector machine with pinball loss function, Information
%      Sciences 494 (2019) 311–327.
%
%
%  Written by: 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % L=load('fertility.txt');
% % %K=L(randperm(100),:);
% % DataTrain=L(1:70,:);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Initailization
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tic;
[no_input,no_col]=size(DataTrain);
 obs =DataTrain(:,no_col);    
 A = zeros(1,no_col-1);
 B = zeros(1,no_col-1);

for i = 1:no_input
    if(obs(i) == 1)
        A = [A;DataTrain(i,1:no_col-1)];
    else
        B = [B;DataTrain(i,1:no_col-1)];
    end
end
% % FunPara=struct('c1',0.5,'c2',0.5,'kerfPara',struct('type','rbf','pars',2^2));
c1 = FunPara.c1;
c2 = FunPara.c2;
kerfPara = FunPara.kerfPara;
eps1 = 0.05;
eps2 = 0.05;
t1 = 0.5;%%%%%%%%%%%%change value of tau 
t2 = 0.5;
m1=size(A,1);
m2=size(B,1);
e1=ones(m1,1);
e2=ones(m2,1);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Compute Kernel 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if strcmp(kerfPara.type,'lin')
    H=[A,e1];
    G=[B,e2];
else
    X=[A;B];
    H=[kernelfun(A,kerfPara,X),e1];
    G=[kernelfun(B,kerfPara,X),e2];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Compute (w1,b1) and (w2,b2)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%DpinGTSVM1
HH=H'*H;
HH = HH + eps1*eye(size(HH));%regularization
HHG = inv(HH)*G';
kerH1=G*HHG;
kerH1=(kerH1+kerH1')/2;
e3=ones(size(kerH1,1),1);
alpha1=quadprog(kerH1,-e3,[],[],[],[],-t2*c1*e2,[]); %SOR
%alpha1=(alpha-beta);
vpos=-HHG*alpha1;
	
%%%%DpinGTSVM2
QQ=G'*G;
QQ=QQ + eps2*eye(size(QQ));%regularization
QQP=inv(QQ)*H';
kerH1=H*QQP;
kerH1=(kerH1+kerH1')/2;
e4=ones(size(kerH1,1),1);
gamma1=quadprog(kerH1,-e4,[],[],[],[],-t1*c2*e1,[]);
%gamma1=(gamma-delta)
vneg=QQP*gamma1;
% % clear kerH1 H G HH HHG QQ QQP;
w1=vpos(1:(length(vpos)-1));
b1=vpos(length(vpos));
w2=vneg(1:(length(vneg)-1));
b2=vneg(length(vneg));   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Predict and output
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % TestX=K(71:100,:);
[no_test,l1]=size(TestX);
if strcmp(kerfPara.type,'lin')
    P_1=TestX(:,1:l1-1);
    y1=P_1*w1+b1;
    y2=P_1*w2+b2;    
else
    C=[A;B];
    P_1=kernelfun(TestX(:,1:l1-1),kerfPara,C);
    y1=P_1*w1+b1;
    y2=P_1*w2+b2;
end
Predict_Y=zeros(size(y1,1),1);
for i=1:size(y1,1)
    if (min(abs(y1(i)),abs(y2(i)))==abs(y1(i)))
        Predict_Y(i) = 1;
    else
        Predict_Y(i) =-1;
    end
end
[no_test,no_col] = size(TestX);
x1=[]; x2 =[];err = 0.;
Predict_Y = Predict_Y';
obs1 = TestX(:,no_col);
for i = 1:no_test
    if(sign(Predict_Y(1,i)) ~= sign(obs1(i)))
        err = err+1;
    end
end  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% classification performance
% acc=Accuracy
%%%%%%%%%%%%%%%%%%%%%%%%%%      
 acc=((size(TestX,1)-(err))/(size(TestX,1)))*100;
time1=toc;
end