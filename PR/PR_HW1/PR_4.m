clear all;
x = -8:0.1:8;
len = length(x);
omega_1=[-8.1,-3.48,-5.52,-3.78,0.63,3.29,2.09,-2.13,2.86,-3.33];
omega_2=[5.35,5.12,-1.34,4.48,7.11,7.17,5.75,0.77,0.90,3.52];
x_axis = zeros(1,len);
y_axis = zeros(1,len);

for i=1:len
  x_start=double([(x(1,i)*ones(1,length(omega_2)))>=omega_2]);
  sum_x=sum(x_start);
  x_axis(1,i)=sum_x/length(omega_2);

  y_start=double([(x(1,i)*ones(1,length(omega_1)))>=omega_1]);
  sum_y=sum(y_start);
  y_axis(1,i)=sum_y/length(omega_1);  
end
plot(x_axis,y_axis,'-');
hold off;
saveas(gcf,'result.png','png');
xlabel('false alarm rate');
ylabel('sensitive');


auc_area = 0.0;

for i=2:length(y_axis)
    auc_area = ((y_axis(1,i)+y_axis(1,i-1)) * (x_axis(1,i)-x_axis(1,i-1))/2)+auc_area;
end

auc_area=abs(auc_area);

disp('AUC=');
disp(auc_area);
