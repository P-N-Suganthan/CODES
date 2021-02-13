function [acc,err,Predict_Y,A,B,w1,b1,w2,b2] = LSKWMTSVM(TestX,DataTrain,FunPara)
% % clear all;
% % close all;
% % K=load('tae.txt');
% % % % X=K(randperm(297),:);
% % DataTrain=K(1:106,:);
tic;
[input col]=size(DataTrain);
obs=DataTrain(:,col);
nclass=3;
A=cell(1,nclass);
eplsion=0.2;
for i=1:input
    for j=1:nclass
    if(obs(i)==j)
        A{j}=[A{j};DataTrain(i,1:col-1)];
    end
    end
end
B=cell(nclass,nclass);
   for i=1:nclass
      for j=1:nclass
          if i==j
              B{i,j}=zeros(size(A{j}));
          else
              B{i,j}=A{j};
          end
      end
   end
   c=cell(nclass,nclass);
  for i=1:nclass
    for j=1:nclass
      if i==j
        c{i,j}=zeros(input-2*size(A{i},1),col-1);
        c{1,1}=zeros(input-2*size(A{2},1),col-1);
      else
              for k=1:nclass
                  if (k~=i)&&(k~=j)
                  c{i,j}=[c{i,j};A{k}];
                  end
              end
          end
      end
  end
D3=cell(nclass,1);
F3=cell(nclass);
H3=cell(nclass);
for i=1:nclass
    for j=1:nclass
    D3{i}=Knnweigmatrix(A{i});
    if(i==j)
        F3{i,j}=zeros(size(B{i,j},1));
        H3{i,j}=zeros(size(c{i,j},1));
    else
        F3{i,j}=Knnweigmatrix(B{i,j});
        H3{i,j}=Knnweigmatrix(c{i,j});
      end
    end
end
for i=1:nclass
    for j=1:nclass
        Fv{i,j}=diag(F3{i,j});
        Hv{i,j}=diag(H3{i,j});
    end
end
% % FunPara=struct('c1',32,'c2',0.03125,'eplsion',0.2,'kerfPara',struct('type','rbf','pars',[1,2^4]));
c1 = FunPara.c1;
c2 = FunPara.c2;
% % eplsion= FunPara.eplsion;
kerfPara = FunPara.kerfPara;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Compute Kernel 
% Compute (w1(i),b1(i)) and (w2(i),b2(i))
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % eplison=0.2;
if strcmp(kerfPara.type,'lin')
    for i=1:nclass
        E{i}=[A{i},ones(size(A{i},1),1)];
        for j=1:nclass
                F{i,j}=[B{i,j},ones(size(B{i,j},1),1)];
                G{i,j}=[c{i,j},ones(size(c{i,j},1),1)];
        end
    end
for i=1:nclass
        for j=1:nclass
            if i==j
                kerH1{i,j}=zeros(col,1);
                kerH2{i,j}=zeros(col,1);
            else    
            kerH1{i,j}=-(inv(E{i}'*D3{i}'*D3{i}*E{i}+c1*F{i,j}'*F3{i,j}'*F3{i,j}*F{i,j}+c2*G{i,j}'*H3{i,j}'*H3{i,j}*G{i,j}))*(c1*F{i,j}'*F3{i,j}*Fv{i,j}+c2*G{i,j}'*H3{i,j}*Hv{i,j}*(1-eplsion));
            end
        end
end
for i=1:nclass
         for j=1:nclass
              kerH2{i,j}=-kerH1{j,i}; 
              w1{i,j}=kerH1{i,j}(1:size(kerH1{i,j},1)-1,:);
              b1{i,j}=kerH1{i,j}(size(kerH1{i,j},1));
              w2{i,j}=kerH2{i,j}(1:size(kerH2{i,j},1)-1,:);
              b2{i,j}=kerH2{i,j}(size(kerH2{i,j},1));
         end
    end
else
    for i=1:nclass
        for j=1:nclass
           x{i,j}=[A{i};B{i,j};c{i,j}];
           L{i,j}=[kernelfun(A{i},kerfPara,x{i,j}),ones(size(kernelfun(A{i},kerfPara,x{i,j}),1),1)];
           N{i,j}=[kernelfun(B{i,j},kerfPara,x{i,j}),ones(size(kernelfun(B{i,j},kerfPara,x{i,j}),1),1)];
           O{i,j}=[kernelfun(c{i,j},kerfPara,x{i,j}),ones(size(kernelfun(c{i,j},kerfPara,x{i,j}),1),1)];
        end
    end
 for i=1:nclass
        for j=1:nclass
              kerH11{i,j}=-(inv(L{i,j}'*D3{i}'*D3{i}*L{i,j}+c1*N{i,j}'*F3{i,j}'*F3{i,j}*N{i,j}+c2*O{i,j}'*H3{i,j}'*H3{i,j}*O{i,j}+eplsion*eye(length(O{i,j}'*H3{i,j}'*H3{i,j}*O{i,j}))))*(c1*N{i,j}'*F3{i,j}*Fv{i,j}+c2*O{i,j}'*H3{i,j}*Hv{i,j}*(1-eplsion));  
        end
 end
 for i=1:nclass
         for j=1:nclass
              kerH22{i,j}=-kerH11{j,i}; 
              w1{i,j}=kerH11{i,j}(1:size(kerH11{i,j},1)-1,:);%%%%%%%my 
              b1{i,j}=kerH11{i,j}(size(kerH11{i,j},1));
              w2{i,j}=kerH22{i,j}(1:size(kerH22{i,j},1)-1,:);
              b2{i,j}=kerH22{i,j}(size(kerH22{i,j},1));
         end
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Predict and output
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % TestX=K(106:150,:);
[no_test,m1]=size (TestX);
P_1=TestX(:,1:m1-1);
if strcmp(kerfPara.type,'lin')
for i=1:nclass
    for j=1:nclass
        for k=1:no_test
y1{i,j}(k,1)=P_1(k,:)*w1{i,j}+b1{i,j};
y2{i,j}(k,1)=P_1(k,:)*w2{i,j}+b2{i,j};
    end
    end
end
else
   for i=1:nclass
    for j=1:nclass
        for k=1:no_test
          P_2{i,j}=kernelfun(TestX(:,1:m1-1),kerfPara,x{i,j});
          y1{i,j}(k,1)=P_2{i,j}(k,:)*w1{i,j}+b1{i,j};
          y2{i,j}(k,1)=P_2{i,j}(k,:)*w2{i,j}+b2{i,j};
        end
    end
   end
end
for i=1:nclass
    for j=1:nclass
        for k=1:size(y1{i,j},1)
        if(y1{i,j}(k,1)>-1+eplsion)
            Predict_Y{i,j}(k,1)=1;
        elseif(y2{i,j}(k,1)<1-eplsion)
            Predict_Y{i,j}(k,1)=-1;
        else
             Predict_Y{i,j}(k,1)=0;
        end
        end
    end
end
%%%%%%%%%%%%%%%%%%
%vote counting
%%%%%%%%%%%%%%%%%%
 classvote=cell(1,nclass);
 for i=1:nclass
      classvote{1,i}=zeros(size(y1{1,2},1),1);
 end
for i=1:nclass
    for j=1:nclass
        for k=1:size(y1{i,j},1)
            if (Predict_Y{i,j}(k,1)==1)
                classvote{1,i}(k,1)=classvote{1,i}(k,1)+1;
             elseif (Predict_Y{i,j}(k,1)==-1)
                 classvote{1,j}(k,1)=classvote{1,j}(k,1)+1;
             else
                 classvote{1,i}(k,1)=classvote{1,i}(k,1)+0;
                 classvote{1,j}(k,1)=classvote{1,j}(k,1)+0;
             end
        end
    end
end  
result=max(cat(2,classvote{:}),[],2);
for k=1:size(TestX,1)
    for i=1:nclass
    if(result(k)==classvote{i}(k,1))
        Prediclass(k)=i;
    end
    end
end  
err=0;
 for i=1:size(TestX,1)
     if(Prediclass(i)~=TestX(i,size(TestX,2)))
         err=err+1;
     end
 end
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%classification performance
%ACC=Accuracy
%SEN=Sensitivity
%SPF=Specificity
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%      
acc=(size(TestX,1)-err)*100/size(TestX,1);
toc;
end