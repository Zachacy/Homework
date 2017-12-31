clear all;
close all;
 
x1=[0.42 -0.087 0.58;-0.2 -3.3 -3.4; 1.3 -0.32 1.7;0.39 0.71 0.23;-1.6 -5.3 -0.15;-0.029 0.89 -4.7;-0.23 1.9 2.2;0.27 -0.3 -0.87;-1.9 0.76 -2.1;0.87 -1.0 -2.6];
x2=[-0.4 0.58 0.089;-0.31 0.27 -0.04;0.38 0.055 -0.035;-0.15 0.53 0.011;-0.35 0.47 0.034;0.17 0.69 0.1;-0.011 0.55 -0.18;-0.27 0.61 0.12;-0.065 0.49 0.0012;-0.12 0.054 -0.063];
x3=[0.83 1.6 -0.014;1.1 1.6 0.48;-0.44 -0.41 0.32;0.047 -0.45 1.4;0.28 0.35 3.1;-0.39 -0.48 0.11;0.34 -0.079 0.14;-0.3 -0.22 2.2;1.1 1.2 -0.46;0.18 -0.11 -0.49];

% mean
n1 = size(x1,1);
n2 = size(x2,1);
n3 = size(x3,1);
mu1 = mean(x1); % /30
mu2 = mean(x2);
mu3 = mean(x3);

% Average of the mean of all classes
mu = (mu1+mu2+mu3)/3;

% within-class scatter matrices
d1 = x1-repmat(mu1, n1, 1);
d2 = x2-repmat(mu2, n2, 1);
d3 = x3 - repmat(mu3, n3, 1);
s1 = d1'*d1;
s2 = d2'*d2;
s3 = d3'*d3;
sw = s1+s2+s3;

% between-class scatter matrices
sb1     = n1*(mu1-mu)'*(mu1-mu);
sb2     = n2*(mu2-mu)'*(mu2-mu);
sb3     = n3*(mu3-mu)'*(mu3-mu);
SB      = sb1+sb2+sb3;

v = inv(sw)*SB;
% find eigen values and eigen vectors of the (v)
[V,D] = eig(v);

% % sort
D=eig(v);
[D_sort D_index]= sort(D,'descend');
V_sort=V(:, D_index);
% largest eigenvector corresponding to the largest eigenvalue
V_max=V_sort(:,1:2)';
V_max=V_max';

% projection point
proj_x1 = x1*V_max;
proj_x2 = x2*V_max;
proj_x3 = x3*V_max;
plot(-proj_x1(:,1), -proj_x1(:,2),'ro');
hold on;
plot(-proj_x2(:,1),-proj_x2(:,2),'gx');
plot( -proj_x3(:,1), -proj_x3(:,2), 'b*' );
hold off;
grid on
axis square
legend('�s1', '�s2', '�s3',1);
xlabel('x');
ylabel('y');

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
    proj_x11=-1/3*((x-mu11)'*inv(sigma11)*(x-mu11)+log(det(sigma11)));
    proj_x22=-1/3*((x-mu22)'*inv(sigma22)*(x-mu22)+log(det(sigma22)));
    proj_x33=-1/3*((x-mu33)'*inv(sigma33)*(x-mu33)+log(det(sigma33)));
    if proj_x11 > proj_x22 && proj_x11 > proj_x33
        count=count+1;
    end
    
end
% error_rate g1
error_rate_g1=1-count/10

% Define the density function g2
count = 0;
for i=1:10
    x=proj_x2(i,1:2)';
    proj_x11=-1/3*((x-mu11)'*inv(sigma11)*(x-mu11)+log(det(sigma11)));
    proj_x22=-1/3*((x-mu22)'*inv(sigma22)*(x-mu22)+log(det(sigma22)));
    proj_x33=-1/3*((x-mu33)'*inv(sigma33)*(x-mu33)+log(det(sigma33)));
    if proj_x22 > proj_x11 && proj_x22 > proj_x33
        count=count+1;
    end
    
end
% error_rate g2
error_rate_g2=1-count/10

% Define the density function g3
count = 0;
for i=1:10
    x=proj_x3(i,1:2)';
    proj_x11=-1/3*((x-mu11)'*inv(sigma11)*(x-mu11)+log(det(sigma11)));
    proj_x22=-1/3*((x-mu22)'*inv(sigma22)*(x-mu22)+log(det(sigma22)));
    proj_x33=-1/3*((x-mu33)'*inv(sigma33)*(x-mu33)+log(det(sigma33)));
    if proj_x33 > proj_x11 && proj_x33 > proj_x22
        count=count+1;
    end
    
end
% error_rate g3
error_rate_g3=1-count/10
