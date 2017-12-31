close all;
clear all;

x1=[0.42,-0.2,1.3,0.39,-1.6,-0.029,-0.23,0.27,-1.9,0.87;...
    -0.4,-0.31,0.38,-0.15,-0.35,0.17,-0.011,-0.27,-0.065,-0.12;...
    0.83,1.1,-0.44,0.047,0.28,-0.39,0.34,-0.3,1.1,0.18]';

x2=[-0.087,-3.3,-0.32,0.71,-5.3,0.89,1.9,-0.3,0.76,-1.0;...
    0.58,0.27,0.055,0.53,0.47,0.69,0.55,0.61,0.49,0.054;...
    1.6,1.6,-0.41,-0.45,0.35,-0.48,-0.079,-0.22,1.2,-0.11]';

x3=[0.58,-3.4,1.7,0.23,-0.15,-4.7,2.2,-0.87,-2.1,-2.6;...
     0.089,-0.04,-0.035,0.011,0.034,0.1,-0.18,0.12,0.0012,-0.063;...
     -0.014,0.48,0.32,1.4,3.1,0.11,0.14,2.2,-0.46,-0.49]';
 
x_all=[0.42,-0.2,1.3,0.39,-1.6,-0.029,-0.23,0.27,-1.9,0.87,-0.4,-0.31,0.38,-0.15,-0.35,0.17,-0.011,-0.27,-0.065,-0.12,0.83,1.1,-0.44,0.047,0.28,-0.39,0.34,-0.3,1.1,0.18;...
    -0.087,-3.3,-0.32,0.71,-5.3,0.89,1.9,-0.3,0.76,-1.0,0.58,0.27,0.055,0.53,0.47,0.69,0.55,0.61,0.49,0.054,1.6,1.6,-0.41,-0.45,0.35,-0.48,-0.079,-0.22,1.2,-0.11;...
    0.58,-3.4,1.7,0.23,-0.15,-4.7,2.2,-0.87,-2.1,-2.6,0.089,-0.04,-0.035,0.011,0.034,0.1,-0.18,0.12,0.0012,-0.063,  -0.014,0.48,0.32,1.4,3.1,0.11,0.14,2.2,-0.46,-0.49]';

% Calculate the mean and corvariance
% mean
mu=(sum(x_all,1)/30)';

% corvariance
sigma=(x_all(1,1:3)'-mu)*(x_all(1,1:3)'-mu)';
for i=2:30
    sigma=sigma+(x_all(i,1:3)'-mu)*(x_all(i,1:3)'-mu)'; 
%     if i==30
%         sigma=sigma/30;
%     end
end

% eigenvalue and eigenvector
[V,D] = eig(sigma);

% % sort 
D=eig(sigma);
[D_sort D_index]= sort(D,'descend');
V_sort=V(:, D_index);
% largest eigenvector corresponding to the largest eigenvalue
V_max=V_sort(:,1:2)';
for i=1:1:30
    for j=1:1:2
       x_all(i,j)=x_all(i,j)-mu(j);
    end     
end
% projection point
proj_x=(V_max)*(x_all)';

V_max=V_max';

[x11]=proj_x(1,1:10);
[y11]=proj_x(2,1:10);
[x12]=proj_x(1,11:20);
[y12]=proj_x(2,11:20);
[x13]=proj_x(1,21:30);
[y13]=proj_x(2,21:30);
plot(x11,y11,'ro',x12,y12,'gx',x13,y13,'b*');
legend('£s1', '£s2', '£s3');
xlabel('x');
ylabel('y');
%pause;

proj_x1=[x11' y11'];
proj_x2=[x12' y12'];
proj_x3=[x13' y13'];

% Calculate the mean and corvariance
% mean
mu11=(sum(proj_x1,1)/10)';
mu22=(sum(proj_x2,1)/10)';
mu33=(sum(proj_x3,1)/10)';

% corvariance
sigma11=(proj_x1(1,1:2)'-mu11)*(proj_x1(1,1:2)'-mu11)';
sigma22=(proj_x2(1,1:2)'-mu22)*(proj_x2(1,1:2)'-mu22)';
sigma33=(proj_x3(1,1:2)'-mu33)*(proj_x3(1,1:2)'-mu33)';
for i=2:10
    sigma11=sigma11+(proj_x1(i,1:2)'-mu11)*(proj_x1(i,1:2)'-mu11)';
    sigma22=sigma22+(proj_x2(i,1:2)'-mu22)*(proj_x2(i,1:2)'-mu22)';
    sigma33=sigma33+(proj_x3(i,1:2)'-mu33)*(proj_x3(i,1:2)'-mu33)';
    if i==10
        sigma11=sigma11/10;
        sigma22=sigma22/10;
        sigma33=sigma33/10;
    end
end

% Define the density function g1
count = 0;
for i=1:10
    x=proj_x1(i,1:2)';
    y1=(-1/3)*((x-mu11)'*inv(sigma11)*(x-mu11)+log(det(sigma11)));
    y2=(-1/3)*((x-mu22)'*inv(sigma22)*(x-mu22)+log(det(sigma22)));
    y3=(-1/3)*((x-mu33)'*inv(sigma33)*(x-mu33)+log(det(sigma33)));
    if y1 > y2 && y1 > y3
        count=count+1;
    end
    
end
% error_rate g1
error_rate_g1=1-count/10

% Define the density function g2
count = 0;
for i=1:10
    x=proj_x2(i,1:2)';
    y1=(-1/3)*((x-mu11)'*inv(sigma11)*(x-mu11)+log(det(sigma11)));
    y2=(-1/3)*((x-mu22)'*inv(sigma22)*(x-mu22)+log(det(sigma22)));
    y3=(-1/3)*((x-mu33)'*inv(sigma33)*(x-mu33)+log(det(sigma33)));
    if y2 > y1 && y2 > y3
        count=count+1;
    end
    
end
% error_rate g2
error_rate_g2=1-count/10

% Define the density function g3
count = 0;
for i=1:10
    x=proj_x3(i,1:2)';
    y1=(-1/3)*((x-mu11)'*inv(sigma11)*(x-mu11)+log(det(sigma11)));
    y2=(-1/3)*((x-mu22)'*inv(sigma22)*(x-mu22)+log(det(sigma22)));
    y3=(-1/3)*((x-mu33)'*inv(sigma33)*(x-mu33)+log(det(sigma33)));
    if y3 > y1 && y3 > y2
        count=count+1;
    end
    
end

% error_rate g3
error_rate_g3=1-count/10
